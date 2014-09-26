/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rksetsys.cpp
 * Created At       : 2013-09-27 10:43
 * Last Modified    : 2013-11-27 14:06
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rktype.h"
#include "rksetsys.h"
#include "rklnedit.h"
#include "rkdlgsel.h"
#include "rkxmloper.h"
#include "rkckbox.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qdatetime.h>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qvalidator.h>

rkSetSys::rkSetSys(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("系统参数设置");
	xmlRootName = "system";

	QString labeltxtpool = "SIM卡号,设备标识,系统类型,系统密码,数据存储间隔(秒),数据上传方式,数据上报间隔(秒),本地时间设置,实时数据使能,分钟数据使能,小时数据使能,日数据使能,开关数据使能,报警数据使能";
	labeltxt = QStringList::split(",", labeltxtpool, false);

	QString codepool = "SIM,MN,ST,PW,DSI,DUM,DUI,,RDUEN,MDUEN,HDUEN,DDUEN,SDUEN,ALARMEN";
	codelist = QStringList::split(",", codepool, true);

	/* Mainframe */
	mainfrm = createMainframe(this);
	glayout = new QGridLayout(mainfrm);
	glayout->setSpacing(5);

	/* Spacing */
	glayout->setRowSpacing(0, 30);
	glayout->setRowStretch(9, 0);
	glayout->setColStretch(0, 10);
	glayout->setColSpacing(3, 30);
	glayout->setColStretch(6, 10);

	int row, col, i;
	for(i = 0; i < labeltxt.count(); i++) {
		row = i < 8 ? i + 1 : i - 7;
		col = i < 8 ? 1 : 4;
		label = new QLabel(mainfrm);
		label->setText(labeltxt[i]);
		glayout->addWidget(label, row, col);
	}

	row = 1;
	col = 2;

	QRegExp regexp("[0-9]*");
	validator = new QRegExpValidator(regexp, this);

	lnedit[0] = new rkLnEdit(mainfrm, "0");
	lnedit[0]->setMaxLength(11);
	lnedit[0]->setValidator(validator);
	lnedit[0]->clear();
	//lnedit[0]->setMaxLength(11);
	connect(lnedit[0], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[0], row++, col);

	lnedit[1] = new rkLnEdit(mainfrm, "1");
	lnedit[1]->setValidator(validator);
	lnedit[1]->setMaxLength(14);
	connect(lnedit[1], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[1], row++, col);

	hlayout = new QHBoxLayout;
	hlayout->setSpacing(5);
	glayout->addLayout(hlayout, row++, col);

	lnedit[2] = new rkLnEdit(mainfrm);
	//lnedit[2]->setReadOnly(true);
	lnedit[2]->setMaxLength(2);
	hlayout->addWidget(lnedit[2]);

	btn = new QPushButton(mainfrm, "0");
	btn->setText("选择");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);

	seldlg = new rkDlgSel(this);
	seldlg->setTitle("系统类型选择");
	seldlg->setMaxSelect(1);
	seldlg->addItem("地表水监测", "21", 0);
	seldlg->addItem("空气质量监测", "22", 1);
	seldlg->addItem("区域环境噪声监测", "23", 2);
	seldlg->addItem("大气环境污染源", "31", 3);
	seldlg->addItem("地表水体环境污染源", "32", 4);
	seldlg->addItem("地下水体环境污染源", "33", 5);
	seldlg->addItem("海洋环境污染源", "34", 6);
	seldlg->addItem("土壤环境污染源", "35", 7);
	seldlg->addItem("声环境污染源", "36", 8);
	seldlg->addItem("振动环境污染源", "37", 9);
	seldlg->addItem("放射性环境污染源", "38", 10);
	seldlg->addItem("电磁环境污染源", "41", 11);
	seldlg->addItem("系统交互", "91", 12);
	seldlg->setFitSize();
	connect(seldlg, SIGNAL(selectedItems(QString)), this, SLOT(recvItems(QString)));

	lnedit[3] = new rkLnEdit(mainfrm, "3");
	lnedit[3]->setMaxLength(6);
	connect(lnedit[3], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[3], row++, col);

#if 0
	lnedit[4] = new rkLnEdit(mainfrm, "4");
	lnedit[4]->setValidator(new QIntValidator(lnedit[4]));
	connect(lnedit[4], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[4], row++, col);
#else
	combox[0] = new QComboBox(mainfrm);
	combox[0]->insertItem("30", 0);
	combox[0]->insertItem("60", 1);
	combox[0]->insertItem("120", 2);
	combox[0]->insertItem("180", 3);
	combox[0]->insertItem("300", 4);
	glayout->addWidget(combox[0], row++, col);
#endif

	combox[1] = new QComboBox(mainfrm);
	combox[1]->insertItem("DTU", 0);
	combox[1]->insertItem("ETH", 1);
	combox[1]->insertItem("DTU+ETH", 2);
	glayout->addWidget(combox[1], row++, col);

#if 0
	lnedit[5] = new rkLnEdit(mainfrm, "5");
	lnedit[5]->setMaxLength(3);
	lnedit[5]->setValidator(validator);
	connect(lnedit[5], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[5], row++, col);
#else
	combox[2] = new QComboBox(mainfrm);
	combox[2]->insertItem("30", 0);
	combox[2]->insertItem("60", 1);
	combox[2]->insertItem("120", 2);
	combox[2]->insertItem("180", 3);
	combox[2]->insertItem("300", 4);
	glayout->addWidget(combox[2], row++, col);
#endif

	hlayout = new QHBoxLayout;
	hlayout->setSpacing(5);
	glayout->addMultiCellLayout(hlayout, 8, 8, 2, 5);

	timeEdit = new QDateTimeEdit(mainfrm);
	timeEdit->dateEdit()->setOrder(QDateEdit::YMD);
	timeEdit->setDateTime(QDateTime::currentDateTime());
	hlayout->addWidget(timeEdit);

	btn = new QPushButton(mainfrm, "1");
	btn->setFixedWidth(80);
	btn->setText("设置");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);

	row = 1;
	col = 5;
	for (i = 8; i < codelist.count(); i++) {
		checkBox[i - 8] = new rkCkBox(mainfrm);
		glayout->addWidget(checkBox[i - 8], row++, col);
	}

	btn = new QPushButton(mainfrm, "2");
	btn->setText("重启系统");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	glayout->addMultiCellWidget(btn, 7, 7, 4, 5, Qt::AlignRight);

	/* Right Frame */
	rightfrm = createRightFrame(this);

	btn = new QPushButton(rightfrm, "3");
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(0, 180);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	btn = new QPushButton(rightfrm, "4");
	btn->setPixmap(QPixmap::fromMimeSource("btn_sv_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(0, 280);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	hide();
}

void rkSetSys::showEvent(QShowEvent *)
{

	timeEdit->setDateTime(QDateTime::currentDateTime());

	rkXmlOper xmlOper(SYS_CFG_FILE, xmlRootName);

	QString val = xmlOper.getParamVal(codelist[0]);
	lnedit[0]->setText(val);

	val = xmlOper.getParamVal(codelist[1]);
	lnedit[1]->setText(val);

	val = xmlOper.getParamVal(codelist[2]);
	lnedit[2]->setText(val);

	val = xmlOper.getParamVal(codelist[3]);
	lnedit[3]->setText(val);

	val = xmlOper.getParamVal(codelist[4]);
	//lnedit[4]->setText(val);
	if (val == "30" || val == "60" || val == "120" || val == "180" || val == "300") {
		combox[0]->setCurrentText(val);
	} else {
		combox[0]->setCurrentItem(0);
	}

	val = xmlOper.getParamVal(codelist[5]);
	if (val == "DTU") {
		combox[1]->setCurrentItem(0);
	} else if (val == "ETH") {
		combox[1]->setCurrentItem(1);
	} else {
		combox[1]->setCurrentItem(2);
	}

	val = xmlOper.getParamVal(codelist[6]);
	//lnedit[5]->setText(val);
	if (val == "30" || val == "60" || val == "120" || val == "180" || val == "300") {
		combox[2]->setCurrentText(val);
	} else {
		combox[2]->setCurrentItem(0);
	}

	for(int index = 8; index < codelist.count(); index++) {
		val = xmlOper.getParamVal(codelist[index]);
		bool st = val == "EN" ? true : false;
		checkBox[index - 8]->setChecked(st);
	}

	xmlOper.save();
}

void rkSetSys::popupKeyboard()
{
	focusedLineEdit = (rkLnEdit *)this->sender();
#if 0
	if (focusedLineEdit == lnedit[4] || focusedLineEdit == lnedit[5]) {
		showKeyboard(TopRight);
	} else {
		showKeyboard(BottomLeft);
	}
#endif
	showKeyboard(BottomLeft);
}

void rkSetSys::procBtnPressed()
{
	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();

	hideKeyboard();

	if (index == 0) {
		seldlg->show();
	} else if (index == 1) {
		setLocalTime();
	} else if (index == 2) {
		int ret = QMessageBox::information(this, "提示", "确定重新启动系统吗?", "确定", "取消");
		if (ret == 0) {
			system("reboot");
		}
	} else if (index == 3) {
		close();
	} else if (index == 4) {
		QString sim_num = lnedit[0]->text();
		if (sim_num.length() != 0 && sim_num.length() != 11) {
			QMessageBox::information(this, "提示", "SIM卡号必须为11位数字", "确定");
			return;
		}

		QString mn_num = lnedit[1]->text();
		if (mn_num.length() == 0) {
			QMessageBox::information(this, "提示", "请输入14位MN号", "确定");
			return;
		} else if (mn_num.length() != 14) {
			QMessageBox::information(this, "提示", "MN号必须为14位数字", "确定");
			return;
		}

		//puts("TP1");
		saveParamValue();
	}
}

void rkSetSys::recvKeyCode(const QString &keycode)
{
	if(!focusedLineEdit) {
		return;
	}

	if (keycode.length() == 1) {
		focusedLineEdit->insert(keycode);
	} else if (keycode.compare("Hide") == 0){
		hideKeyboard();
	} else if (keycode.compare("Enter") == 0) {
	} else if (keycode.compare("Back") == 0) {
		focusedLineEdit->backspace();
	} else if (keycode.compare("Del") == 0) {
		focusedLineEdit->del();
	} else if (keycode.compare("Space") == 0) {
		focusedLineEdit->insert(" ");
	}
}

void rkSetSys::recvItems(QString items)
{
	lnedit[2]->setText(items.remove(";"));
}

void rkSetSys::setLocalTime()
{
	QString temp;
	QDateTime datetime = timeEdit->dateTime();
	int year = datetime.date().year();
	int month = datetime.date().month();
	int day = datetime.date().day();
	int hour = datetime.time().hour();
	int min = datetime.time().minute();
	int sec = datetime.time().second();

	temp.sprintf("date -s \"%d-%d-%d %d:%d:%d\" && hwclock -w", year, month, day, hour, min, sec);
#if 0
	temp = QString::number(year) + "-";
	temp += QString::number(month) + "-";
	temp += QString::number(day) + " ";
	temp += QString::number(hour) + ":";
	temp += QString::number(min);
	temp += QString::number(sec) + "\"";
	QString setString = "date -s \"" + temp;
#endif

	//printf("%s\n", temp.data());
	if (system(temp.data()) == -1) {
		QMessageBox::warning(this, "提示", "设置失败!", "确定");
	} else {
		QMessageBox::information(this, "提示", "设置成功!", "确定");
	}

	return;
}

void rkSetSys::saveParamValue()
{
	int i;
	rkXmlOper xmlOper(SYS_CFG_FILE, xmlRootName);

	for (i = 0; i < 4; i++) {
		xmlOper.setParamVal(codelist[i], lnedit[i]->text().remove(' '));
	}

	xmlOper.setParamVal(codelist[i++], combox[0]->currentText().remove(' '));
	//xmlOper.setParamVal(codelist[i++], lnedit[5]->text().remove(' '));
	xmlOper.setParamVal(codelist[i++], combox[1]->currentText().remove(' '));
	xmlOper.setParamVal(codelist[i++], combox[2]->currentText().remove(' '));

	for (i = 8; i < labeltxt.count(); i++) {
		QString val = checkBox[i - 8]->isChecked() ? "EN" : "DIS";
		xmlOper.setParamVal(codelist[i], val);
	}
	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n是否使设置生效?", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}
}
