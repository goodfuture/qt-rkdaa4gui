#include "rktype.h"
#include "rksetcom.h"
#include "rklnedit.h"
#include "rkxmloper.h"
#include <dirent.h>
#include <dlfcn.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

extern struct context ctx;

rkSetCom::rkSetCom(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("串口参数设置");

	/* Set Global Variables */
	xmlRootName = "uart";

	QString labelTextPool = "串口号,波特率,数据位,停止位,校验位,使用协议";
	labelText = QStringList::split(",", labelTextPool, false);
	pcode = QStringList::split(",", ",BAUD,DB,SB,PARITY,PROTO" , true);

	/* Mainframe */
	mainfrm = createMainframe(this);
	glayout = new QGridLayout(mainfrm);
	glayout->setSpacing(5);
	glayout->setName("glayout");

	glayout->setRowSpacing(0, 30);
	glayout->setRowStretch(8, 10);
	glayout->setColStretch(0, 10);
	glayout->setColStretch(3, 10);

	QLabel *label;
	QString val;
	int row, col;
	for(int i = 0; i < labelText.count(); i++) {
		row = i + 1;
		col = 1;

		label = new QLabel(mainfrm);
		label->setText(labelText[i]);
		glayout->addWidget(label, row, col++);

		/* ComboBox */
		combox[i] = new QComboBox(mainfrm);
		if (i == 0) {
			for (int j = 0; j < COM_NUM; j++) {
				combox[i]->insertItem("COM" + QString::number(j + 1), j);
			}
			combox[i]->setCurrentItem(0);
			connect(combox[i], SIGNAL(activated(int)), this, SLOT(changeChl(int)));
		} else if (i == 1) {
			combox[i]->insertItem("300");
			combox[i]->insertItem("1200");
			combox[i]->insertItem("2400");
			combox[i]->insertItem("4800");
			combox[i]->insertItem("9600");
			combox[i]->insertItem("19200");
			combox[i]->insertItem("38400");
			combox[i]->insertItem("115200");
		} else if (i == 2) {
			combox[i]->insertItem("7", 0);
			combox[i]->insertItem("8", 1);
		} else if (i == 3) {
			combox[i]->insertItem("1", 0);
			combox[i]->insertItem("2", 1);
		} else if (i == 4) {
			combox[i]->insertItem("ODD", 0);
			combox[i]->insertItem("EVEN", 1);
			combox[i]->insertItem("NONE", 2);
		} else if (i == 5) {
			/* Scan Protocol Library */
			scanPlib(ctx.cliarg[ARG_DYN_LIB_OFFSET]);

			for (int j = 0; j < plib.count(); j++) {
				combox[i]->insertItem(plib[j]);
				if (plib[j].contains("modbus", false) == 1) {
					val = plib[j];
				}
			}
			combox[i]->insertItem("none");
			val = val.isEmpty() ? "none" : val;
			combox[i]->setCurrentText(val);
		}
		glayout->addWidget(combox[i], row, col);
	}

	/* Icon Button */
	int pos_x[2] = { 470, 560 };
	int pos_y = 260;
	QString imagePool = "btn_ret,btn_sv";
	QStringList imageList = QStringList::split(",", imagePool, false);
	for(int i = 0; i < 2; i++) {
		QPushButton *iconBtn = new QPushButton(mainfrm, QString::number(i));
		QPixmap pixmap = QPixmap::fromMimeSource(imageList[i] + "_arm.png");
		iconBtn->setPixmap(pixmap);
		iconBtn->setFixedSize(80, 80);
		iconBtn->move(pos_x[i], pos_y);
		connect(iconBtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	}
	hide();
}

void rkSetCom::showEvent(QShowEvent *)
{
	//rkXmlOper xmlOper(SER_CFG_FILE, xmlRootName);
	changeChl(combox[0]->currentItem());
}

void rkSetCom::popupKeyboard()
{
	focusedLineEdit = (rkLnEdit *)this->sender();
	showKeyboard(BottomRight);
}

void rkSetCom::procBtnPressed()
{
	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();
	hideKeyboard();
	if (index == 0) {
		close();
	} else if (index == 1) {
		savePvalue();
	}
}

void rkSetCom::changeChl(int channel)
{
	rkXmlOper xmlOper(SER_CFG_FILE, xmlRootName);

	QString val;
	QString gid = "COM" + QString::number(channel);

	val = xmlOper.getGroupParamVal(gid, pcode[1]);
	val = val.isEmpty() ? "9600" : val;
	combox[1]->setCurrentText(val);

	val = xmlOper.getGroupParamVal(gid, pcode[2]);
	val = val.isEmpty() ? "8" : val;
	combox[2]->setCurrentText(val);

	val = xmlOper.getGroupParamVal(gid, pcode[3]);
	val = val.isEmpty() ? "1" : val;
	combox[3]->setCurrentText(val);

	val = xmlOper.getGroupParamVal(gid, pcode[4]);
	val = val.isEmpty() ? "NONE" : val;
	combox[4]->setCurrentText(val);

	val = xmlOper.getGroupParamVal(gid, pcode[5]);
	val = val.isEmpty() ? "none" : val;
	combox[5]->setCurrentText(val);

	xmlOper.save();
}

/* Save Parameter Value */
void rkSetCom::savePvalue()
{
	rkXmlOper xmlOper(SER_CFG_FILE, xmlRootName);

	int channel = combox[0]->currentItem();
	for(int i = 1; i < 6; i++) {
		xmlOper.setGroupParamVal("COM" + QString::number(channel), pcode[i], combox[i]->currentText());
	}

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n是否使设置生效？", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}   
}

/* Scan Protocol Library */
int rkSetCom::scanPlib(const QString & path)
{
	DIR *dirp;
	void *lib;
	char lpath[128], *lname;
	struct dirent *dir;
	Func func;

	dirp = opendir(path.data());
	if (!dirp) {
		fprintf(stderr, "%s : %s.\n", __func__, strerror(errno));
		return -1;
	}

	while(dir = readdir(dirp)) {
		if (!strstr(dir->d_name, ".so")) {
			continue;
		}

		if (path.endsWith("/")) {
			sprintf(lpath, "%s%s", path.data(), dir->d_name);
		} else {
			sprintf(lpath, "%s/%s", path.data(), dir->d_name);
		}
		lib = dlopen(lpath, RTLD_LAZY);
		if (!lib) {
			fprintf(stderr, "%s.\n", dlerror());
			continue;
		}

		func = (Func)dlsym(lib, "name");
		if (!func) {
			fprintf(stderr, "%s.\n", dlerror());
			dlclose(lib);
			continue;
		}

		if (lname = func()) {
			plib += lname;
		}
		dlclose(lib);
	}
	closedir(dirp);
}
