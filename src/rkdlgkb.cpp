#include "rkclr.h"
#include "rkdlgkb.h"
#include "rkbtnkey.h"
#include <qfont.h>
#include <qlayout.h>
#include <qstringlist.h>
#include <qbuttongroup.h>

rkDlgKb::rkDlgKb(QWidget *parent, const char *name, bool modal, WFlags fl)
:QDialog(parent, name, modal, fl)
{
	/* Set Background Color */
	this->setFixedSize(346, 150);
	this->setPaletteBackgroundColor(rkClr(218, 218, 218));
	/* Font Setting */
	QFont font = this->font();
#ifdef __ARM__
	font.setPointSize(18);
#else
	font.setPointSize(12);
#endif
	this->setFont(font);

	/* Key Map */
	keyValueLower = "1 2 3 4 5 6 7 8 9 0 Back ";
	keyValueLower += "q w e r t y u i o p ; ";
	keyValueLower += "$ a s d f g h j k l Enter ";
	keyValueLower += "Shift z x c v b n m , . # ";
	keyValueLower += "( ) + - Space * / Hide ";
	keyMapLower = QStringList::split(" ", keyValueLower, true);

	keyValueUpper = "1 2 3 4 5 6 7 8 9 0 Del ";
	keyValueUpper += "Q W E R T Y U I O P ; ";
	keyValueUpper += "$ A S D F G H J K L Enter ";
	keyValueUpper += "Shift Z X C V B N M , . # ";
	keyValueUpper += "( ) + - Space * / Hide ";
	keyMapUpper = QStringList::split(" ", keyValueUpper, true);

	keyboardLayout = new QVBoxLayout(this, 0, 0, "keyboardLayout");

	row = new QHBoxLayout(keyboardLayout, 0);

	/* Number key */
	rkBtnKey *keyButton;
	int i = 0;
	for(; i < 10; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	/* Backspace key */
	keyButton = new rkBtnKey(this, "key" + QString::number(i++, 10));
	keyButton->setFixedSize(46, 30);
	row->addWidget(keyButton);

	row = new QHBoxLayout(keyboardLayout, 0);
	row->addSpacing(16);

	/* Q W E R T Y U I O P ; */
	for(; i < 22; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	row = new QHBoxLayout(keyboardLayout, 0);
	/* $ A S D F G H J K L */
	for(; i <32; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	/* Enter */
	keyButton = new rkBtnKey(this, "key" + QString::number(i++, 10));
	keyButton->setFixedSize(46, 30);
	keyButton->setDefault(true);
	row->addWidget(keyButton);

	row = new QHBoxLayout(keyboardLayout, 0);
	/* Shift */
	keyButton = new rkBtnKey(this, "key" + QString::number(i++, 10));
	keyButton->setFixedSize(46, 30);
	row->addWidget(keyButton);

	/* Z X C V B N M , . # */
	for (; i < 44; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	/* ( ) + - */
	row = new QHBoxLayout(keyboardLayout, 0);
	for(; i < 48; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	/* Space */
	keyButton = new rkBtnKey(this, "key" + QString::number(i++, 10));
	keyButton->setFixedSize(120, 30);
	row->addWidget(keyButton);

	/* * / */
	for(; i < 51; i++) {
		keyButton = new rkBtnKey(this, "key" + QString::number(i, 10));
		keyButton->setFixedSize(30, 30);
		row->addWidget(keyButton);
	}

	/* Hide */
	keyButton = new rkBtnKey(this, "key" + QString::number(i++, 10));
	keyButton->setFixedSize(46, 30);
	row->addWidget(keyButton);

	/* set keyboard's key text to lower */
	for(int j = 0; j < 52; j++) {
		QString btnName = "key" + QString::number(j, 10);
		keyButton = (rkBtnKey *)this->child(btnName, "rkBtnKey", true);
		if (keyButton) {
			keyButton->setText(keyMapLower[j]);
			connect(keyButton, SIGNAL(pressed()), this, SLOT(processKeyPressed()));
		}
	}
	isUpper = false;
}

void rkDlgKb::convertKeyMap()
{
	QString btnName;
	rkBtnKey *keyBtn;
	if (isUpper) {
		for(int i = 0; i < 52; i++) {
			btnName = "key" + QString::number(i, 10);
			keyBtn = (rkBtnKey *)this->child(btnName, "rkBtnKey", true);
			if (keyBtn) {
				keyBtn->setText(keyMapLower[i]);
			}
		}
		isUpper = false;
	} else {
		for(int i = 0; i < 52; i++) {
			btnName = "key" + QString::number(i, 10);
			keyBtn = (rkBtnKey *)this->child(btnName, "rkBtnKey", true);
			if (keyBtn) {
				keyBtn->setText(keyMapUpper[i]);
			}
		}
		isUpper = true;
	}
}

void rkDlgKb:: processKeyPressed()
{
	rkBtnKey *keyBtn;
	QString btnName;
	int index;
	keyBtn = (rkBtnKey *)sender();
	if (keyBtn) {
		btnName = QString(keyBtn->name());
		index = btnName.remove(0, 3).toInt();
		//printf("index = %d\n", index);
		switch (index) {
			case 33: /* Shift */
				convertKeyMap();
				return;
#if 0
			case 51: /* Hide */
				hide();
				return;
#endif
			default:
				break;
		}

		if (isUpper) {
			sendKeyCode(keyMapUpper[index]);
		} else {
			sendKeyCode(keyMapLower[index]);
		}
	}
}
