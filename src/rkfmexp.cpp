#include "rktype.h"
#include "rkusb.h"
#include "rkfmexp.h"
#include "rkclr.h"
#include <qlayout.h>
#include <sys/mount.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qmessagebox.h>
#include <stdio.h>
#include <unistd.h>

extern struct context ctx;

rkFmExp::rkFmExp(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("数据导出");

	QFont font = this->font();
	font.setPointSize(24);
	this->setFont(font);

	QString labelText = "导出方式,数据类型,起始时间,截止时间";
	QStringList txtlist = QStringList::split(",", labelText, false);

	/* Grid Layout */
	glayout = new QGridLayout(this);
	glayout->setSpacing(8);
	glayout->setRowStretch(0, 20);
	glayout->setRowStretch(9, 10);
	glayout->setColStretch(0, 1);
	glayout->setColSpacing(2, 360);
	glayout->setColStretch(3, 1);

	for(unsigned int i = 0; i < txtlist.count(); i++) {
		label = new QLabel(this);
		label->setText(txtlist[i]);
		glayout->addWidget(label, i + 1, 1);
	}

	int row = 1, col = 2;
	combox[0] = new QComboBox(this);
	combox[0]->insertItem("USB", 0);
	glayout->addWidget(combox[0], row++, col);

	combox[1] = new QComboBox(this);
	combox[1]->insertItem("实时数据", 0);
	combox[1]->insertItem("实时报文", 1);
	combox[1]->insertItem("分钟报文", 2);
	combox[1]->insertItem("小时报文", 3);
	combox[1]->insertItem("日报文", 4);
	glayout->addWidget(combox[1], row++, col);

	stm = new QDateTimeEdit(this);
	glayout->addWidget(stm, row++, col);

	etm = new QDateTimeEdit(this);
	glayout->addWidget(etm, row++, col);

	label = new QLabel(this);
	label->setAlignment(Qt::AlignRight);
	glayout->addMultiCellWidget(label, row, row, 1, 2);
	row++;

	layout = new QHBoxLayout;
	glayout->setRowSpacing(row++, 10);
	glayout->addMultiCellLayout(layout, row, row, 1, 2);

	layout->setSpacing(10);
	layout->addStretch(10);

	btn = new QPushButton(this, "0");
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	layout->addWidget(btn);

	btn = new QPushButton(this, "1");
	btn->setPixmap(QPixmap::fromMimeSource("btn_exp_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	layout->addWidget(btn);

	hide();
}

int export_callback(void *arg, int colnum, char **col, char **)
{
	int i;

	for (i = 0; i < colnum; i++) {
		if (i == colnum - 1) {
			fprintf((FILE *)arg, "%s\n", col[i]);
		} else if (i == 1) {
			fprintf((FILE *)arg, "%s", ctime((time_t *)col[i]));
		} else {
			fprintf((FILE *)arg, "%s,", col[i]);
		}
	}

	return 0;
}

void rkFmExp::procBtnPressed()
{
	btn = (QPushButton *)this->sender();
	sqlite3 *db;
	FILE *fp;
	char *sql, *errmsg;
	int rc;
	int num = QString(btn->name()).toInt();
	struct usbdisk dev;
	QString path;

	if (num == 0) {
		close();
	} else if (num == 1) {
		label->setText("");
		btn->setEnabled(false);
		QStringList tbl = QStringList::split(",", "RtdTable,RtmTable,MomTable,HomTable,DomTable");

		rc = rkUsbDiskCheck(&dev);
		if (rc == -1) {
			btn->setEnabled(true);
			QMessageBox::critical(this, "错误", "请插入U盘!", "确定");
			return;
		}
		rc = rkUsbDiskMnt(&dev);
		if (rc == -1) {
			btn->setEnabled(true);
			QMessageBox::critical(this, "错误", "挂载U盘失败!", "确定");
			return;
		}

		path = "/mnt/" + tbl[combox[1]->currentItem()].lower() + ".txt";
		fp = fopen(path.data(), "w+");
		if (!fp) {
			btn->setEnabled(true);
			QMessageBox::critical(this, "错误", "创建文件失败!", "确定");
			return;
		}

		if (combox[1]->currentItem() == 0) {
			rc = sqlite3_open_v2(ctx.cliarg[ARG_RTD_DB_PATH_OFFSET], &db, SQLITE_OPEN_READONLY, NULL);
		} else {
			rc = sqlite3_open_v2(ctx.cliarg[ARG_MSG_DB_PATH_OFFSET], &db, SQLITE_OPEN_READONLY, NULL);
		}
		if (rc) {
			btn->setEnabled(true);
			sqlite3_close(db);
			QMessageBox::critical(this, "错误", "打开数据库失败!", "确定");
			return;
		}

		label->setText("正在导出数据...");
		sql = sqlite3_mprintf("SELECT * FROM %q;", tbl[combox[1]->currentItem()].data());
		rc = sqlite3_exec(db, sql, export_callback, fp, &errmsg);
		if (rc != SQLITE_OK) {
			label->setText("");
			btn->setEnabled(true);
			sqlite3_free(sql);
			sqlite3_close(db);
			QMessageBox::critical(this, "错误", "导出失败!", "确定");
			return;
		}
		fclose(fp);
		sqlite3_free(sql);
		sqlite3_close(db);
		QMessageBox::critical(this, "成功", "导出成功,请拔出U盘!", "确定");
		label->setText("导出数据成功!");

		rkUsbDiskRemove(&dev);
		btn->setEnabled(true);
	}
}

void rkFmExp::keyPressEvent(QKeyEvent *e) 
{
}

void rkFmExp::keyReleaseEvent(QKeyEvent *e) 
{
	if (e->key() == Key_Escape) {
		close();
	}
}  
