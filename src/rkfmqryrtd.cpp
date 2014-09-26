#include "rkclr.h"
#include "rktype.h"
#include "rktbl.h"
#include "rkfmqryrtd.h"
#include "rkdlgsel.h"
#include "rklnedit.h"
#include "rkckbox.h"
#include "rkxmloper.h"
#include <qframe.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qtoolbutton.h>
#include <qwidgetstack.h>
#include <qmessagebox.h>

#define TBL_MAX_ROW 20

extern struct context ctx;

rkFmQryRtd::rkFmQryRtd(QWidget *parent, const char *name)
	:QWidget(parent, name, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{ 
	pagenum = 0;
	pagecnt= 0;
	setFixedSize(800, 480);
	setPaletteForegroundColor(rkClr(255, 255, 255));
	setPaletteBackgroundColor(rkClr(47, 63, 95));
	move(0, 0); 

	gfont = this->font();
#ifdef __ARM__
	gfont.setPointSize(28);
#else
	gfont.setPointSize(18);
#endif
	this->setFont(gfont);

	wstack = new QWidgetStack(this);
	wstack->setFixedSize(800, 480);

	qrywgt = new QWidget(wstack);
	selwgt = new QWidget(wstack);
	dspwgt = new QWidget(wstack);
	wstack->addWidget(qrywgt, 0);
	wstack->addWidget(selwgt, 1);
	wstack->addWidget(dspwgt, 2);
	wstack->raiseWidget(0);

	gfont = qrywgt->font();
#ifdef __ARM__
	gfont.setPointSize(28);
#else
	gfont.setPointSize(18);
#endif 

	qrywgt->setFont(gfont);
	gfont = selwgt->font();

#ifdef __ARM__
	gfont.setPointSize(28);
#else
	gfont.setPointSize(18);
#endif
	selwgt->setFont(gfont);

	gfont = dspwgt->font();
#ifdef __ARM__
	gfont.setPointSize(20);
#else
	gfont.setPointSize(18);
#endif
	dspwgt->setFont(gfont);

	glayout = new QGridLayout(qrywgt);
	glayout->setSpacing(8);
	glayout->setRowStretch(0, 10);
	glayout->setRowSpacing(4, 10);
	glayout->setRowStretch(9, 10);
	glayout->setColStretch(0, 1); 
	glayout->setColSpacing(2, 360);
	glayout->setColStretch(3, 1); 

	label = new QLabel(qrywgt);
	label->setText("污染物类型");
	glayout->addWidget(label, 1, 1);
	label = new QLabel(qrywgt);
	label->setText("起始时间");
	glayout->addWidget(label, 2, 1);
	label = new QLabel(qrywgt);
	label->setText("终止时间");
	glayout->addWidget(label, 3, 1);

	layout = new QHBoxLayout;
	lnedit = new rkLnEdit(qrywgt);
	layout->addWidget(lnedit);
	QToolButton *tbtn = new QToolButton(qrywgt, "0");
	tbtn->setText(" ... ");
	connect(tbtn, SIGNAL(clicked()), this, SLOT(clickProc()));
	layout->addWidget(tbtn);
	glayout->addLayout(layout, 1, 2);

	tmedit[0] = new QDateTimeEdit(QDateTime::currentDateTime(), qrywgt);
	tmedit[0]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[0], 2, 2);

	tmedit[1] = new QDateTimeEdit(QDateTime::currentDateTime(), qrywgt);
	tmedit[1]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[1], 3, 2);

	layout = new QHBoxLayout;
	glayout->addMultiCellLayout(layout, 5, 5, 1, 2);

	layout->setSpacing(10);
	layout->addStretch(10);
	btn = new QPushButton(qrywgt, "1"); 
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	layout->addWidget(btn);
	btn = new QPushButton(qrywgt, "2"); 
	btn->setPixmap(QPixmap::fromMimeSource("btn_qry_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	layout->addWidget(btn);

	cmap.insert("01", "烟(粉)尘");
	cmap.insert("02", "二氧化硫");
	cmap.insert("03", "氮氧化物");
	cmap.insert("04", "一氧化碳");
	cmap.insert("05", "硫化氢");
	cmap.insert("12", "二氧化碳");
	cmap.insert("S01", "氧气含量");
	cmap.insert("S02", "烟气流速");
	cmap.insert("S03", "烟气温度");
	cmap.insert("S04", "烟气动压");
	cmap.insert("S05", "含湿量");
	cmap.insert("S08", "烟气压力");
	cmap.insert("B02", "废气流量");
	cmap.insert("001", "PH值");
	cmap.insert("011", "COD");
	cmap.insert("060", "氨氮");
	cmap.insert("101", "总磷");
	cmap.insert("B01", "污水流量");
	cmap.insert("Pfk", "总排放量");

	glayout = new QGridLayout(selwgt);
	glayout->setSpacing(10);
	glayout->setRowStretch(0, 30);
	glayout->setRowStretch(11, 30);
	glayout->setColStretch(0, 30);
	glayout->setColStretch(9, 30);

	label = new QLabel(selwgt);
	gfont = label->font();
#ifdef __ARM__
	gfont.setPointSize(28);
#else
	gfont.setPointSize(18);
#endif
	label->setFont(gfont);
	label->setAlignment(Qt::AlignCenter);
	label->setText("污染物类型选择");
	glayout->addMultiCellWidget(label, 0, 0, 0, 9);

	int mapcnt;
	QMap<QString, QString>::Iterator it;
	for (it = cmap.begin(), mapcnt = 0; it != cmap.end(); ++it, mapcnt++) {
		int row = mapcnt / 4 + 1, col = (mapcnt % 4) * 2  + 1;
		label = new QLabel(selwgt);
		gfont = label->font();
#ifdef __ARM__
		gfont.setPointSize(20);
#else
		gfont.setPointSize(10);
#endif
		label->setFont(gfont);
		label->setText(it.data());
		glayout->addWidget(label, row, col);

		ckbox = new rkCkBox(selwgt);
		glayout->addWidget(ckbox, row, col + 1);
		map.insert(ckbox, it.key());
	}

	layout = new QHBoxLayout;
	glayout->addMultiCellLayout(layout, 8, 8, 1, 8);
	layout->addStretch(10);

	btn = new QPushButton(selwgt, "3");
	btn->setText("确定");
	layout->addWidget(btn);
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));

	glayout = new QGridLayout(dspwgt);
	glayout->setSpacing(10);
	glayout->setRowStretch(0, 30);
	glayout->setRowSpacing(4, 50);
	glayout->setColSpacing(2, 10);
	tbl = new rkTbl(dspwgt);
	tbl->setLeftMargin(0);
	tbl->setNumRows(TBL_MAX_ROW);
	tbl->setNumCols(4);
	QStringList clabel = QStringList::split(",", "时间,通道,代码,值");
	for(int i = 0; i < clabel.count(); i++) {
		tbl->setColumnLabels(clabel);
	}
	tbl->setColumnWidth(0, 200);
	tbl->setColumnStretchable(1, true);
	tbl->setColumnStretchable(2, true);
	tbl->setColumnStretchable(3, true);
	glayout->addMultiCellWidget(tbl, 0, 5, 0, 0);

	btn = new QPushButton(dspwgt, "4");
	btn->setText("上翻");
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	glayout->addWidget(btn, 1, 1);

	btn = new QPushButton(dspwgt, "5");
	btn->setText("下翻");
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	glayout->addWidget(btn, 2, 1);

	btn = new QPushButton(dspwgt, "6");
	btn->setText("返回");
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	glayout->addWidget(btn, 3, 1);

	hide();
}

void rkFmQryRtd::clickProc()
{
	QPushButton *btn = (QPushButton *)this->sender();
	int id = QString(btn->name()).toInt();
	int rc;
	char *sql, *err, tmp[128];
	time_t stm, etm;

	if (id == 0) { /* 污染物选择 */
		wstack->raiseWidget(selwgt);
	} else if (id == 1) { /* 返回 */
		close();
	} else if (id == 2) { /* 查询 */
		QStringList list = QStringList::split(";", lnedit->text());
		if (!list.count()) {
			QMessageBox::critical(this, "查询失败", "请输入污染物类型", "确定");
			return;
		}

		sprintf(tmp, "ChlCode=\"%s\"", list[0].data());
		for (int i = 1; i < list.count(); i++) {
			sprintf(tmp + strlen(tmp), " OR ChlCode=\"%s\"", list[i].data());
		}

		if (tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 1) {
			QMessageBox::warning(this, "警告", "查询时间段不能超过1天！", "确定");
			return;
		}

		stm = tmedit[0]->dateTime().toTime_t();
		etm = tmedit[1]->dateTime().toTime_t();
		rc = sqlite3_open_v2(ctx.cliarg[ARG_RTD_DB_PATH_OFFSET], &db, SQLITE_OPEN_READONLY, NULL);
		if (rc) {
			sqlite3_close(db);
			QMessageBox::critical(this, "查询失败", "打开数据库失败!", "确定");
			return;
		}

		sql = sqlite3_mprintf("SELECT * FROM RtdTable WHERE TimeStamp >= %ld AND TimeStamp <= %ld AND (%q);", stm, etm, tmp);
		rc = sqlite3_get_table(db, sql, &qryRst, &row, &col, &err);
		if (rc || !row) {
			sqlite3_free(sql);
			sqlite3_free_table(qryRst);
			sqlite3_close(db);
			QMessageBox::critical(this, "查询失败", "查询失败!", "确定");
			return;
		}
		sqlite3_free(sql);
		pagenum = (row + TBL_MAX_ROW - 1) / TBL_MAX_ROW;
		pagecnt = 0;
		for (int i = 0; i < row && i < TBL_MAX_ROW; i++) {
			QDateTime dt;
			dt.setTime_t(atol(qryRst[col * (i + 1) + 1]));
			tbl->setText(i, 0, dt.toString("yy-MM-dd hh:mm:ss"));
			tbl->setText(i, 1, qryRst[col * (i + 1) + 2]);
			tbl->setText(i, 2, qryRst[col * (i + 1) + 3]);
			tbl->setText(i, 3, qryRst[col * (i + 1) + 4]);
		}
		wstack->raiseWidget(dspwgt);
	} else if (id == 3) {/* 确定选择 */
		wstack->raiseWidget(qrywgt);
		ckMap::Iterator it;
		QString val;
		for (it = map.begin(); it != map.end(); it++) {
			ckbox = it.key();
			if (ckbox->isChecked()) {
				val += it.data();
				val += ";";
			}
		}
		lnedit->setText(val);
	} else if (id == 4) { /* 上翻 */
		pagecnt--;
		if (pagecnt < 0) {
			pagecnt = 0;
		}
		//qDebug("pagenum = %d, pagecnt = %d\n", pagenum, pagecnt);
		for (int i = pagecnt * TBL_MAX_ROW; i < row && i < (pagecnt + 1) * TBL_MAX_ROW; i++) {
			int index = col * (i + 1) + 1;
			//qDebug("index = %d", index);
#if 1
			QDateTime dt;
			dt.setTime_t(atol(qryRst[index++]));
			tbl->setText(i % TBL_MAX_ROW, 0, dt.toString("yy-MM-dd hh:mm:ss"));
			tbl->setText(i % TBL_MAX_ROW, 1, qryRst[index++]);
			tbl->setText(i % TBL_MAX_ROW, 2, qryRst[index++]);
			tbl->setText(i % TBL_MAX_ROW, 3, qryRst[index++]);
#endif
		}
	} else if (id == 5) { /* 下翻 */
		pagecnt++;
		if (pagecnt >= pagenum - 1) {
			pagecnt = pagenum - 1;
		}
		//printf("pagenum = %d, pagecnt = %d\n", pagenum, pagecnt);
		for (int i = (pagecnt - 1)* TBL_MAX_ROW; i < row && i < TBL_MAX_ROW * pagecnt; i++) {
			int index = col * (i + 1) + 1;
			//qDebug("index = %d", index);
#if 1
			QDateTime dt;
			dt.setTime_t(atol(qryRst[index++]));
			tbl->setText(i % TBL_MAX_ROW, 0, dt.toString("yy-MM-dd hh:mm:ss"));
			tbl->setText(i % TBL_MAX_ROW, 1, qryRst[index++]);
			tbl->setText(i % TBL_MAX_ROW, 2, qryRst[index++]);
			tbl->setText(i % TBL_MAX_ROW, 3, qryRst[index++]);
#endif
		}
	} else if (id == 6) { /* 返回 */
		sqlite3_free_table(qryRst);
		sqlite3_close(db);
		wstack->raiseWidget(qrywgt);
	}
}

void rkFmQryRtd::keyPressEvent(QKeyEvent *e) 
{
}

void rkFmQryRtd::keyReleaseEvent(QKeyEvent *e) 
{
	if (e->key() == Key_Escape) {
		close();
	}
}
