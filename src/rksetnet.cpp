/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rksetnet.cpp
 * Created At       : 2013-11-12 09:55
 * Last Modified    : 2013-11-14 12:48
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rktype.h"
#include "rksetnet.h"
#include "rklnedit.h"
#include "rkxmloper.h"
#include "rkfltip.h"
#include "rkifaceoper.h"
#include "rkckbox.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <qmessagebox.h>
#include <qregexp.h>

rkSetNet::rkSetNet(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("网络参数设置");

	/* Set Global Variables */
	xmlRootName = "network";
	ifaceArg = QStringList::split(",", "address,netmask,gateway,dnsserver", false);

	QString labelTextPool = "本机地址,子网掩码,网关,DNS,通讯方式,本地端口,远程地址,远程端口";
	labelText = QStringList::split(",", labelTextPool, false);

	paramCode = QStringList::split(",", "LOCALADDR,NETMASK,GATEWAY,DNS,CM,LOCALPORT,REMOTEADDR,REMOTEPORT", false);

	/* Mainframe */
	mainframe = createMainframe(this);
	frameLayout = new QGridLayout(mainframe);
	frameLayout->setSpacing(5);
	frameLayout->setName("frameLayout");

	/* |-----|------|-------|-----|------|
	 * |label|widget|spacing|label|widget|
	 * |-----|------|-------|-----|------|
	 *  4Rows * 5Columns
	 */

	/* Spacing */
	frameLayout->setRowSpacing(0, 30);
	frameLayout->setColSpacing(2, 200);
	frameLayout->setRowStretch(5, 10);
	frameLayout->setColStretch(0, 10);
	frameLayout->setColSpacing(3, 20);
	frameLayout->setColStretch(6, 10);

	/* Create Main Widgets */
	for (unsigned int i = 0; i < labelText.count(); i++) {
		QHBoxLayout *childLayout;
		QLabel *label;

		/* Label */
		label = new QLabel(mainframe, "label" + QString::number(i));
		label->setText(labelText[i]);
		if (i < 4) { /* Column 0 */
			frameLayout->addWidget(label, i + 1, 1);
		} else { /* Column 3 */
			frameLayout->addWidget(label, i - 4 + 1, 4);
		}

		int row = i < 4 ? i + 1 : i - 4 + 1;
		int col = i < 4 ? 2 : 5;
		if (i == 4) {
			combox = new QComboBox(mainframe);
			combox->insertItem("UDP客户端", 0);
			combox->insertItem("UDP服务端", 1);
			combox->insertItem("TCP客户端", 2);
			combox->insertItem("TCP服务端", 3);
			frameLayout->addWidget(combox, row, col);
		} else if (i == 5) {
			childLayout = new QHBoxLayout;
			frameLayout->addLayout(childLayout, row, col);

			lineEdit[4] = new rkLnEdit(mainframe);
			//lineEdit[4]->setMaxLength(4);
			lineEdit[4]->setValidator(new QIntValidator(0, 65535, lineEdit[4]));
			connect(lineEdit[4], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
			childLayout->addWidget(lineEdit[4]);

			checkBox = new rkCkBox(mainframe, "checkBox" + QString::number(i));
			childLayout->addWidget(checkBox);
			connect(checkBox, SIGNAL(checked(bool)), this, SLOT(processToggled(bool)));
			lineEdit[4]->clear();
			label = new QLabel(mainframe);
			label->setText("随机");
			childLayout->addWidget(label);
		} else if (i < 4) {
			lineEdit[i] = new rkLnEdit(mainframe);
			connect(lineEdit[i], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
			lineEdit[i]->clear();
			frameLayout->addWidget(lineEdit[i], row, col);

			QString reg_exp("([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}");
			QValidator *validator = new QRegExpValidator(QRegExp(reg_exp), this);
#if 0
			lineEdit[i]->setInputMask("000.000.000.000");
#else
			lineEdit[i]->setValidator(validator);
#endif
		} else {
			lineEdit[i - 1] = new rkLnEdit(mainframe);
			connect(lineEdit[i - 1], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
			lineEdit[i - 1]->clear();
			frameLayout->addWidget(lineEdit[i - 1], row, col);
			if (i == 7) {
				//lineEdit[i - 1]->setMaxLength(4);
				lineEdit[i - 1]->setValidator(new QIntValidator(0, 65535, lineEdit[i - 1]));
			} else {
				QString reg_exp("([1-9]|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}");
				QValidator *validator = new QRegExpValidator(QRegExp(reg_exp), this);
#if 0
				lineEdit[i - 1]->setInputMask("000.000.000.000");
#else
				lineEdit[i - 1]->setValidator(validator);
#endif
			}
		}
	}

	/* Icon Button */
	int pos_x[2] = { 470, 560 };
	int pos_y = 260;
	QString imagePool = "btn_ret,btn_sv";
	QStringList imageList = QStringList::split(",", imagePool, false);
	for(int i = 0; i < 2; i++) {
		QPushButton *iconBtn = new QPushButton(mainframe, QString::number(i));
		QPixmap pixmap = QPixmap::fromMimeSource(imageList[i] + "_arm.png");
		iconBtn->setPixmap(pixmap);
		iconBtn->setFixedSize(80, 80);
		iconBtn->move(pos_x[i], pos_y);
		connect(iconBtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	}
	hide();
}

void rkSetNet::showEvent(QShowEvent *)
{
	/* Read XML File */
	rkXmlOper xmlOper(NET_CFG_FILE, xmlRootName);
	QString value;

	/* Get XML Value */
	int index = 0;
	for (; index < 4; index++) {
		value = RkIfaceGetParam("eth0", ifaceArg[index].data());
		lineEdit[index]->setText(value);
	}

	value = xmlOper.getParamVal(paramCode[index++]);
	if (value == "UDPCLIENT") {
		combox->setCurrentItem(0);
	} else if (value == "UDPSERVER") {
		combox->setCurrentItem(1);
	} else if (value == "TCPCLIENT") {
		combox->setCurrentItem(2);
	} else if (value == "TCPSERVER") {
		combox->setCurrentItem(3);
	}

	value = xmlOper.getParamVal(paramCode[index++]);
	if (value == "0" || value.isEmpty()) {
		processToggled(true);
	} else {
		processToggled(false);
		lineEdit[4]->setText(value);
	}

	value = xmlOper.getParamVal(paramCode[index++]);
	lineEdit[5]->setText(value);

	value = xmlOper.getParamVal(paramCode[index++]);
	lineEdit[6]->setText(value);

	xmlOper.save();
}

void rkSetNet::popupKeyboard()
{
	focusedLineEdit = (rkLnEdit *)this->sender();
	showKeyboard(BottomLeft);
}

void rkSetNet::procBtnPressed()
{
	QPushButton *btn = (QPushButton *)this->sender();
	QString name = btn->name();
	int index = name.toInt(); /* "iconBtn" + number */
	hideKeyboard();
	if (index == 0) {
		close();
	} else if (index == 1) {
		saveParamValue();
	}
}

void rkSetNet::recvKeyCode(const QString &keycode)
{
	if (keycode.length() == 1) {
		focusedLineEdit->insert(keycode);
	} else if (keycode == "Hide"){
		hideKeyboard();
	} else if (keycode == "Enter") {
		/* 
		 * Do Nothing 
		 */
	} else if (keycode == "Back") {
		focusedLineEdit->backspace();
	} else if (keycode == "Del") {
		focusedLineEdit->del();
	} else if (keycode == "Space") {
		focusedLineEdit->insert(" ");
	}
}

void rkSetNet::processToggled(bool toggle)
{
	if (toggle) {
		lineEdit[4]->setReadOnly(true);
		lineEdit[4]->setText("0");
		checkBox->setChecked(true);
		disconnect(lineEdit[4], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	} else {
		lineEdit[4]->setReadOnly(false);
		lineEdit[4]->clear();
		checkBox->setChecked(false);
		connect(lineEdit[4], SIGNAL(clicked()), this, SLOT(popupKeyboard()));
	}
}

void rkSetNet::saveParamValue()
{
	rkXmlOper xmlOper(NET_CFG_FILE, xmlRootName);
	for(unsigned int i = 0; i < labelText.count(); i++) {
		if (i == 0 || i == 1 || i == 2 || i == 3) {
			if (!lineEdit[i]->text().isNull()) {
				RkIfaceSetParam("eth0", ifaceArg[i].data(), lineEdit[i]->text().data());
			}
			xmlOper.setParamVal(paramCode[i], lineEdit[i]->text().remove(' '));
		} else if (i == 4) {
			const char *val;
			if (combox->currentItem() == 0) {
				val = "UDPCLIENT";
			} else if (combox->currentItem() == 1) {
				val = "UDPSERVER";
			} else if (combox->currentItem() == 2) {
				val = "TCPCLIENT";
			} else if (combox->currentItem() == 3) {
				val = "TCPSERVER";
			}
			xmlOper.setParamVal(paramCode[i], val);
		} else {
			xmlOper.setParamVal(paramCode[i], lineEdit[i - 1]->text().remove(' '));
		}
	}

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n需重启生效，是否立即重启?", "确定", "取消");
		if (rc == 0) {
			//emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
			system("reboot");
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}
	QString str = QString("ifconfig eth0 %1 netmask %2\n").arg(lineEdit[0]->text()).arg(lineEdit[1]->text());
	printf(str.data());
	system(str.data());
}
