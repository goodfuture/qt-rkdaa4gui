#include "rktype.h"
#include "rksetdio.h"
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
#include <qmessagebox.h>

rkSetDio::rkSetDio(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("数字量使能设置");

	/* Set Global Variable */
	xmlRootName = "dio";
	xmlGroupPrefix = "DI";

	/* Mainframe */
	mainfrm = createMainframe(this);
	glayout = new QGridLayout(mainfrm);
	glayout->setSpacing(5);
	glayout->setName("glayout");

	glayout->setRowSpacing(0, 30);
	glayout->setRowSpacing(2, 20);
	glayout->setRowSpacing(6, 10);
	glayout->setRowStretch(10, 10);
	glayout->setColStretch(0, 5);
	glayout->setColStretch(12, 5);
	glayout->setColSpacing(2, 50);
	glayout->setColSpacing(2, 50);
	glayout->setColSpacing(5, 50);
	glayout->setColSpacing(8, 50);
	glayout->setColSpacing(11, 50);

	hlayout = new QHBoxLayout;
	hlayout->setSpacing(10);
	glayout->addMultiCellLayout(hlayout, 1, 1, 1, 11);

	label = new QLabel(mainfrm);
	label->setText("数字量类型选择");
	hlayout->addWidget(label);

	combox = new QComboBox(mainfrm);
	combox->insertItem("输入通道", 0);
	combox->insertItem("输出通道", 1);
	combox->setCurrentItem(0);

	connect(combox, SIGNAL(activated(int)), this, SLOT(changeChannel(int)));
	hlayout->addWidget(combox);
	hlayout->addStretch(0);

	itemCount = 8;
	for(int i = 0; i < itemCount; i++) {
		QString value;
		int row = (i / 4) * 4 + 3;
		int col = (i % 4) * 3 + 1;

		/* Label */
		label = new QLabel(mainfrm);
		label->setText("通道ID");
		glayout->addWidget(label, row, col);

		/* ID */
		label = new QLabel(mainfrm);
		label->setText(QString::number(i + 1));
		glayout->addWidget(label, row++, col + 1);

		/* Label */
		label = new QLabel(mainfrm);
		label->setText("通道代码");
		glayout->addWidget(label, row, col);

		/* Code */
		codelabel[i] = new QLabel(mainfrm);
		codelabel[i]->setText("P" + QString::number(i) + "I");
		glayout->addWidget(codelabel[i], row++, col + 1);

		/* Label */
		label = new QLabel(mainfrm);
		label->setText("是否启用");
		glayout->addWidget(label, row, col);

		/* CheckBox */
		ckbox = new rkCkBox(mainfrm, QString::number(i + 1));
		glayout->addWidget(ckbox, row++, col + 1);
	}

	/* Right Frame */
	rfrm = createRightFrame(this);

	/* Icon Button */
	btn = new QPushButton(rfrm, "0");
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(0, 180);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	btn = new QPushButton(rfrm, "1");
	btn->setPixmap(QPixmap::fromMimeSource("btn_sv_arm.png"));
	btn->setFixedSize(80, 80);
	btn->move(0, 280);
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	hide();
}

void rkSetDio::showEvent(QShowEvent *)
{
	int channel = combox->currentItem();
	changeChannel(channel);
}

void rkSetDio::popupKeyboard()
{
	focusedLineEdit = (rkLnEdit *)this->sender();
	QString name = focusedLineEdit->name();
	int index = name.remove(0, 8).toInt(); /* "lineEdit" + number */
	switch(index) {
		case 0:
		case 1:
		case 4:
		case 5:
			showKeyboard(TopRight);
			break;
		default:
			showKeyboard(BottomLeft);
			break;
	}
}

void rkSetDio::procBtnPressed()
{
	hideKeyboard();
	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();
	if (index == 0) {
		close();
	} else if (index == 1) {
		saveParamValue();
	}
}

void rkSetDio::recvKeyCode(const QString &keycode)
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

void rkSetDio::changeChannel(int channel)
{
	rkXmlOper xmlOper(DIO_CFG_FILE, xmlRootName);
	xmlGroupPrefix = channel == 0 ? "DI" : "DO";

	QString value;
	for(int i = 0; i < itemCount; i++) {
		/* Get Param Value */
		value = xmlOper.getGroupParamVal(xmlGroupPrefix + QString::number(i), "INUSE");

		ckbox = (rkCkBox *)this->child(QString::number(i + 1), "rkCkBox", true);
		bool rst = value == "EN" ? true : false;
		ckbox->setChecked(rst);

		value = "P" + QString::number(i);
		value += channel == 0 ? "I" : "O";
		codelabel[i]->setText(value);
	}
	xmlOper.save();
}

void rkSetDio::saveParamValue()
{
	rkXmlOper xmlOper(DIO_CFG_FILE, xmlRootName);

	QString value;
	for(int i = 0; i < itemCount; i++) {
		/* CheckBox */
		ckbox = (rkCkBox *)this->child( QString::number(i + 1), "rkCkBox", true);
		value = ckbox->isChecked() ? "EN" : "DIS";
		xmlOper.setGroupParamVal(xmlGroupPrefix + QString::number(i), "INUSE", value);

		/* LineEdit */
		xmlOper.setGroupParamVal(xmlGroupPrefix + QString::number(i), "CODE", codelabel[i]->text().remove(' '));
	}

	if (!xmlOper.save()) {
		int rc = QMessageBox::information(this, "提示", "保存成功!\n是否使设置生效?", "确定", "取消");
		if (rc == 0) {
			emit sendUICmd(UI_CMD_RELOAD_USR_CFG);
		}
	} else {
		QMessageBox::information(this, "提示", "保存失败!", "确定");
	}   
}
