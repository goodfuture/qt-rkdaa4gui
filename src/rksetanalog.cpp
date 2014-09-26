/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rksetanalog.cpp
 * Created At       : 2013-10-17 16:16
 * Last Modified    : 2013-11-15 09:36
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rksetanalog.h"
#include "rkdlgsel.h"
#include "rklnedit.h"
#include "rkdlgfmlsel.h"
#include "rkxmloper.h"
#include "rkckbox.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qregexp.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qmessagebox.h>
#include <qvalidator.h>

rkSetAnalog::rkSetAnalog(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("模拟量设置");

	/* Set Global Variable */
	curChl = 0;
	xmlRootName = "analog";

	QString ltextPool = "通道选择,通道类型,通道代码,量程上限,量程下限,报警阀值(上),报警阀值(下),";
	ltextPool += "是否启用,是否折算值,是否启用公式,公式编辑器";
	ltext = QStringList::split(",", ltextPool, false);

	/* Mainframe */
	mainfrm = createMainframe(this);
	glayout = new QGridLayout(mainfrm);
	glayout->setSpacing(5);

	glayout->setRowSpacing(0, 30);
	glayout->setRowStretch(8, 10);
	glayout->setColStretch(0, 10);
	glayout->setColSpacing(3, 30);
	glayout->setColStretch(6, 10);

	for(int i = 0; i < ltext.count(); i++) {
		row = i < 7 ? i + 1 : i - 6;
		col = i < 7 ? 1 : 4;
		label = new QLabel(mainfrm);
		label->setText(ltext[i]);
		glayout->addWidget(label, row, col);
	}

	row = 1;
	col = 2;
	combox[0] = new QComboBox(mainfrm);
	combox[0]->setSizeLimit(5);
	for (int i = 0; i < AI_NUM; i++) {
		combox[0]->insertItem("CH" + QString::number(i + 1), i);
	}
	connect(combox[0], SIGNAL(activated(int)), this, SLOT(changeChl(int)));
	combox[0]->setCurrentItem(0);
	glayout->addWidget(combox[0], row++, col);

	combox[1] = new QComboBox(mainfrm);
	combox[1]->insertItem("4-20mA", 0);
	combox[1]->insertItem("0-5V", 1);
	glayout->addWidget(combox[1], row++, col);

	hlayout = new QHBoxLayout;
	hlayout->setSpacing(5);
	glayout->addLayout(hlayout, row++, col);

	lnedit[0] = new rkLnEdit(mainfrm, "0");
	connect(lnedit[0], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	hlayout->addWidget(lnedit[0]);

	tbtn = new QToolButton(mainfrm, "0");
	tbtn->setText(" ... ");
	connect(tbtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(tbtn);

	for (int i = 1; i < 5; i++) {
		lnedit[i] = new rkLnEdit(mainfrm, QString::number(i));
		//lnedit[i]->setMaxLength(8);
		//QRegExp rx("[0-9.]*");
		QDoubleValidator *validator = new QDoubleValidator(lnedit[i]);
		lnedit[i]->setValidator(validator);
		connect(lnedit[i], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
		glayout->addWidget(lnedit[i], row++, col);
	}

	row = 1;
	col = 5;
	for (int i = 0; i < 3; i++) {
		ckbox[i] = new rkCkBox(mainfrm);
		glayout->addWidget(ckbox[i], row++, col);
	}

	hlayout = new QHBoxLayout;
	glayout->addLayout(hlayout, row++, col);

	lnedit[5] = new rkLnEdit(mainfrm, "5");
	connect(lnedit[5], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	hlayout->addWidget(lnedit[5]);

	tbtn = new QToolButton(mainfrm, "1");
	tbtn->setText(" ... ");
	connect(tbtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(tbtn);

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
	fmlDlg = new rkDlgFmlSel(this, "fmldlg");
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

void rkSetAnalog::showEvent(QShowEvent *)
{
	int index = combox[0]->currentItem();
	changeChl(index);
}

void rkSetAnalog::popupKeyboard()
{
	const QObject *object = this->sender();
	QString name = object->name();

	if(name.contains("fmldlg") > 0) {
		focusedLineEdit = NULL;
		showKeyboard(BottomLeft);
	} else {
		focusedLineEdit = (rkLnEdit *)object;
		int index = name.toInt();
		if (index == 5) {
			showKeyboard(BottomLeft);
		} else {
			showKeyboard(TopRight);
		}
	}
}

void rkSetAnalog::recvItems(QString items)
{
	lnedit[0]->setText(items.remove(";"));
}

void rkSetAnalog::procBtnPressed()
{
	hideKeyboard();
	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();
	switch (index) {
		case 0:
			seldlg->show();
			break;
		case 1:
			fmlDlg->show();
			break;
		case 2:
			close();
			break;
		case 3: {
					float ulv = lnedit[1]->text().toFloat();
					float llv = lnedit[2]->text().toFloat();
					float utv = lnedit[3]->text().toFloat();
					float ltv = lnedit[4]->text().toFloat();
					//printf("ulv = %f, llv = %f, utv = %f, ltv = %f\n", ulv, llv, utv, ltv);
					if ( ulv < utv) {
						QMessageBox::information(this, "提示", "量程上限不能小于报警上限！", "确定");
						return;
					}

					if ( llv > ltv) {
						QMessageBox::information(this, "提示", "量程下限不能大于报警下限！", "确定");
						return;
					}

					saveParamValue();
					break;
				}
		default:
				break;
	}
}

void rkSetAnalog::recvKeyCode(const QString &keycode)
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
	} else if (keycode.compare("Space") == 0) {
		focusedLineEdit->insert(" ");
	} else if (keycode.compare("Del") == 0) {
		focusedLineEdit->del();
	}
}

void rkSetAnalog::recvFml(const QString &fml)
{
	lnedit[5]->setText(fml);
}

void rkSetAnalog::saveParamValue()
{
	QString gid = "CH" + QString::number(curChl);
	rkXmlOper xmlOper(AI_CFG_FILE, xmlRootName);

	xmlOper.setGroupParamVal(gid, "TYPE", combox[1]->currentText().upper());

	pcode = QStringList::split(",", "CODE,ULV,LLV,UTV,LTV,FMLEXPR");
	for (int i = 0; i < 6; i++) {
		QString value = lnedit[i]->text().remove(' ');
		xmlOper.setGroupParamVal(gid, pcode[i], value);
	}

	pcode = QStringList::split(",", "INUSE,ISCONV,USEFML");
	for (int i = 0; i < 3; i++) {
		xmlOper.setGroupParamVal(gid, pcode[i], ckbox[i]->isChecked() ? "EN" : "DIS");
	}

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n是否使当前设置生效？", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}
}

void rkSetAnalog::changeChl(int chl)
{
	curChl = chl;
	QString gid, val;
	int index, i;
	bool rst;

	rkXmlOper xmlOper(AI_CFG_FILE, xmlRootName);

	gid = "CH" + QString::number(curChl);

	val = xmlOper.getGroupParamVal(gid, "TYPE");
	index = val == "0-5V" ? 1 : 0;
	combox[1]->setCurrentItem(index);

	val = xmlOper.getGroupParamVal(gid, "CODE");
	lnedit[0]->setText(val);
	val = xmlOper.getGroupParamVal(gid, "ULV");
	lnedit[1]->setText(val);
	val = xmlOper.getGroupParamVal(gid, "LLV");
	lnedit[2]->setText(val);
	val = xmlOper.getGroupParamVal(gid, "UTV");
	lnedit[3]->setText(val);
	val = xmlOper.getGroupParamVal(gid, "LTV");
	lnedit[4]->setText(val);
	val = xmlOper.getGroupParamVal(gid, "FMLEXPR");
	lnedit[5]->setText(val);

	val = xmlOper.getGroupParamVal(gid, "INUSE");
	rst = val == "EN" ? true : false;
	ckbox[0]->setChecked(rst);
	val = xmlOper.getGroupParamVal(gid, "ISCONV");
	rst = val == "EN" ? true : false;
	ckbox[1]->setChecked(rst);
	val = xmlOper.getGroupParamVal(gid, "USEFML");
	rst = val == "EN" ? true : false;
	ckbox[2]->setChecked(rst);

	xmlOper.save();
}
