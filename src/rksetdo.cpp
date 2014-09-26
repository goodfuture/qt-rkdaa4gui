#include "rktype.h"
#include "rksetdo.h"
#include "rklnedit.h"
#include "rkbtnsw.h"
#include "rkxmloper.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

rkSetDo::rkSetDo(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("数字量输出状态设置");

	/* Set Global Variables */
	xmlRootName = "dio";
	xmlGroupPrefix = "DO";

	/* Mainframe */
	mainframe = createMainframe(this);
	frameLayout = new QGridLayout(mainframe);
	frameLayout->setSpacing(10);
	frameLayout->setName("frameLayout");

	/* Spacing */
	frameLayout->setRowSpacing(0, 30);
	frameLayout->setRowSpacing(3, 20);
	frameLayout->setRowSpacing(6, 20);
	frameLayout->setRowStretch(9, 5);
	frameLayout->setColStretch(0, 10);
	frameLayout->setColSpacing(3, 30);
	frameLayout->setColSpacing(6, 30);
	frameLayout->setColStretch(9, 10);

	/* Item */
	itemCount = 8;
	for(int i = 0; i < itemCount; i++) {
		QLabel *label;
		QString value;

		int row = (i / 3) * 3 + 1;
		int col = (i % 3) * 3 + 1;

		/* Label */
		label = new QLabel(mainframe);
		label->setText("通道ID");
		frameLayout->addWidget(label, row, col);

		/* ID */
		label = new QLabel(mainframe);
		label->setText(QString::number(i + 1));
		frameLayout->addWidget(label, row, col + 1);

		/* Label */
		label = new QLabel(mainframe);
		label->setText("输出状态");
		frameLayout->addWidget(label, ++row, col);

		/* SwitchButton */
		switchBtn[i] = new rkBtnSw(mainframe, "switchBtn" + QString::number(i));
		connect(switchBtn[i], SIGNAL(switched(bool)), this, SLOT(switchChanged(bool)));
		frameLayout->addWidget(switchBtn[i], row, col + 1);
	}

	/* Icon Button */
	QPushButton *iconBtn = new QPushButton(mainframe, "0");
	iconBtn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	iconBtn->setFixedSize(80, 80);
	iconBtn->move(560, 260);
	connect(iconBtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hide();
}

void rkSetDo::showEvent(QShowEvent *)
{
	rkXmlOper xmlOper(DIO_CFG_FILE, xmlRootName);
	QString value;
	for (int i = 0; i < 8; i++) {
		value = xmlOper.getGroupParamVal(xmlGroupPrefix + QString::number(i), "STATE");
		if (value == "ON") {
			switchBtn[i]->setSwitch(true);
		} else {
			switchBtn[i]->setSwitch(false);
		}
	}
	xmlOper.save();
}

void rkSetDo::popupKeyboard()
{
}

void rkSetDo::procBtnPressed()
{
	hideKeyboard();
	saveParamValue();
	close();
}

void rkSetDo::recvKeyCode(const QString &keycode)
{
	if (!focusedLineEdit) {
		return;
	}

	if (keycode.length() == 1) {
		focusedLineEdit->insert(keycode);
	} else if (keycode.compare("Hide") == 0){
		hideKeyboard();
	} else if (keycode.compare("Enter") == 0) {
		/* 
		 * Do Nothing 
		 */
	} else if (keycode.compare("Back") == 0) {
		focusedLineEdit->backspace();
	} else if (keycode.compare("Space") == 0) {
		focusedLineEdit->insert(" ");
	}
}

void rkSetDo::switchChanged(bool value)
{
#if 0
	rkBtnSw *switchBtn = (rkBtnSw *)this->sender();
	QString name = switchBtn->name();
#endif
}

void rkSetDo::saveParamValue()
{
	rkXmlOper xmlOper(DIO_CFG_FILE, xmlRootName);

	for(int i = 0; i < itemCount; i++) {
		rkBtnSw *switchBtn = (rkBtnSw *)this->child("switchBtn" + QString::number(i), "QWidget", true);
		if (switchBtn->isOn()) {
			xmlOper.setGroupParamVal(xmlGroupPrefix + QString::number(i), "STATE", "ON");
		} else {
			xmlOper.setGroupParamVal(xmlGroupPrefix + QString::number(i), "STATE", "OFF");
		}
	}

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "是否使设置生效?", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_RELOAD_USR_CFG);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}
}
