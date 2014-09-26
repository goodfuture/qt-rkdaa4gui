#include "rktype.h"
#include "rksetvar.h"
#include "rkdlgsel.h"
#include "rkdlgfmlsel.h"
#include "rklnedit.h"
#include "rkxmloper.h"
#include "rkckbox.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <qvalidator.h>

rkSetVar::rkSetVar(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("串口量设置");

	/* Set Global Variables */
	xmlRootName = "uart";

	QString ltextPool = "通道号,通道代码,串口号,设备地址,数据地址,数据类型,";
	ltextPool += "是否启用,是否折算值,是否启用公式,公式编辑器";
	ltext = QStringList::split(",", ltextPool, false);

	/* Mainframe */
	mainfrm = createMainframe(this);
	glayout = new QGridLayout(mainfrm);
	glayout->setSpacing(5);
	glayout->setName("glayout");

	glayout->setRowSpacing(0, 30);
	glayout->setRowStretch(9, 10);
	glayout->setColStretch(0, 10);
	glayout->setColSpacing(2, 200);
	glayout->setColSpacing(3, 30);
	glayout->setColStretch(6, 10);

	for(int i = 0; i < ltext.count(); i++) {
		row = i < 6 ? i + 1 : i - 5;
		col = i < 6 ? 1 : 4;
		label = new QLabel(mainfrm);
		label->setText(ltext[i]);
		glayout->addWidget(label, row, col);
	}

	/* Main Widgets */
	row = 1; 
	col = 2;

	spinbox = new QSpinBox(mainfrm);
	spinbox->setPrefix("CH");
	spinbox->setMinValue(1);
	spinbox->setMaxValue(EI_NUM);
	spinbox->setValue(0);
	connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(changeChl(int)));
	glayout->addWidget(spinbox, row++ , col);

	hlayout = new QHBoxLayout;
	glayout->addLayout(hlayout, row++, col);

	lnedit[0] = new rkLnEdit(mainfrm, "0");
	connect(lnedit[0], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	hlayout->addWidget(lnedit[0]);

	tbtn = new QToolButton(mainfrm, "0");
	tbtn->setText(" ... ");
	connect(tbtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(tbtn);

	combox[0] = new QComboBox(mainfrm);
	combox[0]->insertItem("COM1", 0);
	combox[0]->insertItem("COM2", 1);
	combox[0]->insertItem("COM3", 2);
	combox[0]->insertItem("COM4", 3);
	glayout->addWidget(combox[0], row++, col);

	lnedit[1] = new rkLnEdit(mainfrm, "1");
	lnedit[1]->setValidator(new QIntValidator(1, 255, lnedit[1]));
	connect(lnedit[1], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[1], row++, col);

	lnedit[2] = new rkLnEdit(mainfrm, "2");
	lnedit[2]->setValidator(new QIntValidator(1, 65535, lnedit[2]));
	connect(lnedit[2], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	glayout->addWidget(lnedit[2], row++, col);

	combox[1] = new QComboBox(mainfrm);
	combox[1]->insertItem("整型", 0);
	combox[1]->insertItem("浮点型", 1);
	glayout->addWidget(combox[1], row++, col);

	row = 1;
	col = 5;

	ckbox[0] = new rkCkBox(mainfrm);
	glayout->addWidget(ckbox[0], row++, col);

	ckbox[1] = new rkCkBox(mainfrm);
	glayout->addWidget(ckbox[1], row++, col);

	ckbox[2] = new rkCkBox(mainfrm);
	glayout->addWidget(ckbox[2], row++, col);

	hlayout = new QHBoxLayout;
	glayout->addLayout(hlayout, row, col);

	lnedit[3] = new rkLnEdit(mainfrm, "3");
	connect(lnedit[3], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	hlayout->addWidget(lnedit[3]);

	tbtn = new QToolButton(mainfrm, "1");
	tbtn->setText(" ... ");
	connect(tbtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(tbtn);

	/* Icon Button */
	btn = new QPushButton(mainfrm, "2");
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(470, 260);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	btn = new QPushButton(mainfrm, "3");
	btn->setPixmap(QPixmap::fromMimeSource("btn_sv_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(560, 260);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	/* Fml Dialog */
	fmlDlg = new rkDlgFmlSel(this, "fmlDlg");
	connect(fmlDlg, SIGNAL(showKeyboard()), this, SLOT(popupKeyboard()));
	connect(fmlDlg, SIGNAL(hideKeyboard()), this, SLOT(hideKeyboard()));
	connect(fmlDlg, SIGNAL(formula(const QString &)), this, SLOT(recvFml(const QString &)));
	connect(keyboard, SIGNAL(sendKeyCode(const QString &)), fmlDlg, SLOT(recvKeyCode(const QString &)));
	hide();


	/* Sel Dialog */
	int index = 0;
	seldlg = new rkDlgSel(this);
	seldlg->setTitle("通道代码选择");
	seldlg->setMaxSelect(1);
	seldlg->addItem("烟(粉)尘", "01", index++);
	seldlg->addItem("二氧化硫", "02", index++);
	seldlg->addItem("氮氧化物", "03", index++);
	seldlg->addItem("一氧化碳", "04", index++);
	seldlg->addItem("硫化氢", "05", index++);
	seldlg->addItem("二氧化碳", "12", index++);
	seldlg->addItem("氧气含量", "S01", index++);
	seldlg->addItem("烟气流速", "S02", index++);
	seldlg->addItem("烟气温度", "S03", index++);
	seldlg->addItem("含湿量", "S05", index++);
	seldlg->addItem("烟气动压", "S04", index++);
	seldlg->addItem("烟气压力", "S08", index++);
	seldlg->addItem("废气流量", "B02", index++);
	index = 18; 
	seldlg->addItem("污水流量", "B01", index++);
	seldlg->addItem("PH值", "001", index++);
	seldlg->addItem("COD", "011", index++);
	seldlg->addItem("氨氮", "060", index++);
	seldlg->addItem("总磷", "101", index++);
	seldlg->addItem("总排放量", "Pfk", index++);
	seldlg->setFitSize();
	connect(seldlg, SIGNAL(selectedItems(QString)), this, SLOT(recvItems(QString)));
}

void rkSetVar::showEvent(QShowEvent *)
{
	int channel = spinbox->value();
	changeChl(channel);
}

void rkSetVar::recvItems(QString items) 
{
	lnedit[0]->setText(items.remove(";"));
}

void rkSetVar::popupKeyboard()
{
	const QObject *obj = this->sender();
	focusedLnEdit = (rkLnEdit *)obj;
	int index = QString(obj->name()).toInt();
	if (index == 5 || index == 6) {
		showKeyboard(TopRight);
	} else {
		showKeyboard(BottomLeft);
	}
}

void rkSetVar::procBtnPressed()
{
	hideKeyboard();
	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();
	if (index == 0) {
		seldlg->show();
	} else if (index == 1) {
		fmlDlg->show();
	} else if (index == 2) {
		close();
	} else if (index == 3) {
		saveParamValue();
	}
}

void rkSetVar::recvKeyCode(const QString &keycode)
{
	if(!focusedLnEdit) {
		return;
	}

	if (keycode.length() == 1) {
		focusedLnEdit->insert(keycode);
	} else if (keycode.compare("Hide") == 0){
		hideKeyboard();
	} else if (keycode.compare("Enter") == 0) {
	} else if (keycode.compare("Back") == 0) {
		focusedLnEdit->backspace();
	} else if (keycode.compare("Space") == 0) {
		focusedLnEdit->insert(" ");
	}
}

void rkSetVar::recvFml(const QString &formula)
{
	lnedit[3]->setText(formula);
}

void rkSetVar::changeChl(int channel)
{
	rkXmlOper xmlOper(SER_CFG_FILE, xmlRootName);

	QString val;
	QString groupId = "CH" + QString::number(channel - 1);
	val = xmlOper.getGroupParamVal(groupId, "CODE");
	lnedit[0]->setText(val);

	val = xmlOper.getGroupParamVal(groupId, "COM");
	val = val.isEmpty() ? "COM0" : val;
	int index = val.remove("COM").toInt();
	if (index < 0) {
		index = 0;
	}
	combox[0]->setCurrentItem(index);

	val = xmlOper.getGroupParamVal(groupId, "DA");
	lnedit[1]->setText(val);

	val = xmlOper.getGroupParamVal(groupId, "RA");
	lnedit[2]->setText(val);

	val = xmlOper.getGroupParamVal(groupId, "DT");
	val = val == "FLT" ? "浮点型" :"整型";
	combox[1]->setCurrentText(val);

	val = xmlOper.getGroupParamVal(groupId, "INUSE");
	bool rst = val == "EN" ? true : false;
	ckbox[0]->setChecked(rst);

	val = xmlOper.getGroupParamVal(groupId, "ISCONV");
	rst = val == "EN" ? true : false;
	ckbox[1]->setChecked(rst);

	val = xmlOper.getGroupParamVal(groupId, "USEFML");
	rst = val == "EN" ? true : false;
	ckbox[2]->setChecked(rst);

	val = xmlOper.getGroupParamVal(groupId, "FMLEXPR");
	lnedit[3]->setText(val);

	xmlOper.save();
}

void rkSetVar::saveParamValue()
{
	QString val;
	rkXmlOper xmlOper(SER_CFG_FILE, xmlRootName);

	QString gid = "CH" + QString::number(spinbox->value() - 1);
	xmlOper.setGroupParamVal(gid, "CODE", lnedit[0]->text().remove(' '));
	xmlOper.setGroupParamVal(gid, "COM", QString("COM") + QString::number(combox[0]->currentItem()));
	xmlOper.setGroupParamVal(gid, "DA", lnedit[1]->text().remove(' '));
	xmlOper.setGroupParamVal(gid, "RA", lnedit[2]->text().remove(' '));
	if (combox[1]->currentItem() == 0) {
		val = "INT";
	} else {
		val = "FLT";
	}
	xmlOper.setGroupParamVal(gid, "DT", val);
	val = ckbox[0]->isChecked() ? "EN" : "DIS";
	xmlOper.setGroupParamVal(gid, "INUSE", val);
	val = ckbox[1]->isChecked() ? "EN" : "DIS";
	xmlOper.setGroupParamVal(gid, "ISCONV", val);
	val = ckbox[2]->isChecked() ? "EN" : "DIS";
	xmlOper.setGroupParamVal(gid, "USEFML", val);
	xmlOper.setGroupParamVal(gid, "FMLEXPR", lnedit[3]->text().remove(' '));

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n是否使设置生效?", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}
}
