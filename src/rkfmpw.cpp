/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmpw.cpp
 * Created At       : 2013-11-22 09:11
 * Last Modified    : 2013-11-22 09:41
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rkfmpw.h"
#include "rkfrm.h"
#include "rklnedit.h"
#include "rkxmloper.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qfont.h>

rkFmPw::rkFmPw(QWidget *parent, const char *name, bool modal, WFlags f)
	:QDialog(parent, name, modal, f)
{
	this->setFixedSize(260, 100);
	this->setFocusPolicy(QWidget::NoFocus);
	QFont font = this->font();
#ifdef __ARM__
	font.setPointSize(18);
#else
	font.setPointSize(14);
#endif
	this->setFont(font);

	frameLayout = new QGridLayout(this);
	frameLayout->setName("frameLayout");
	frameLayout->setMargin(10);
	frameLayout->setSpacing(5);

	QLabel *label = new QLabel(this, "label0"); /* Name Label */
	label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	label->setText("口令:");
	frameLayout->addWidget(label, 0, 0);

	lineEdit = new rkLnEdit(this, "lineEdit"); /* Password LineEdit */
	lineEdit->setEchoMode(QLineEdit::Password);
	connect(lineEdit, SIGNAL(clicked()), this, SLOT(processLineEditClicked()));
	frameLayout->addWidget(lineEdit, 0, 1);

	promptLable = new QLabel(this, "label1"); /* Prompt Label */
	promptLable->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	frameLayout->addMultiCellWidget(promptLable, 1, 1, 0, 1);
	
	QHBoxLayout *childLayout0 = new QHBoxLayout; /* horizontal layout */
	childLayout0->setSpacing(20);
	frameLayout->addMultiCellLayout(childLayout0, 2, 2, 0, 1);
	button0 = new QPushButton(this, "button0"); /* Button "Yes" */
	button0->setText("确定");
	connect(button0, SIGNAL(clicked()), this, SLOT(processBtnClicked()));
	childLayout0->addWidget(button0);

	button1 = new QPushButton(this, "button1"); /* Button "Calcel" */
	button1->setText("取消");
	button1->setDefault(true);
	connect(button1, SIGNAL(clicked()), this, SLOT(processBtnClicked()));
	childLayout0->addWidget(button1);

	button0->setFocus();
}

void rkFmPw::processBtnClicked(int index)
{
	switch(index) {
		case 0: { /* Ok */
				rkXmlOper *xml = new rkXmlOper("system.xml", "system");
				//printf("lineEdit->text() == \"%s\"\n", lineEdit->text().data());
				//printf("xml->getParamVal(\"PW\") == %s\n", xml->getParamVal("PW").data());
#if 0
				QString src_passwd = QString(xml->getParamVal("PW")).upper();
				QString dst_passwd = QString(lineEdit->text()).upper();
#else
				QString src_passwd = QString(xml->getParamVal("PW"));
				QString dst_passwd = QString(lineEdit->text());
#endif
				if (src_passwd.length() == 0 && dst_passwd.length() == 0 || src_passwd.compare(dst_passwd) == 0) {
					promptLable->setText("密码校验成功!");
					emit passwordOk();
					emit hideKeyboard();
					this->close();
				} else {
					promptLable->setText("密码错误，请重试!");
				}
				xml->free();
				delete xml;
			} break;
		case 1: { /* Cancel */
				emit hideKeyboard();
				promptLable->clear();
				lineEdit->clear();
				this->close();
			} break;
		default:
			break;
	}
}

void rkFmPw::processBtnClicked()
{
	QPushButton *btn = (QPushButton *)this->sender();
	QString name = btn->name();
	int index = name.remove(0, 6).toInt();
	processBtnClicked(index);
}

void rkFmPw::processLineEditClicked()
{
	/* display keyboard on bottom right */
	emit dispKeyboard(rkFrame::BottomRight); 
}

void rkFmPw::recvKeyCode(const QString &keycode)
{
	if (keycode.length() == 1) {
		lineEdit->insert(keycode);
	} else if (keycode.compare("Enter") == 0) {
		/* Do Nothing */
	} else if (keycode.compare("Back") == 0) {
		lineEdit->backspace();
	} else if (keycode.compare("Space") == 0) {
		lineEdit->insert(" ");
	}
}

void rkFmPw::showEvent(QShowEvent *e)
{
	lineEdit->clear();
	promptLable->clear();
	QWidget::showEvent(e);
}

void rkFmPw::keyPressEvent(QKeyEvent *e)
{
}

void rkFmPw::keyReleaseEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Left) {
		puts("LEFT");
	} else if (e->key() == Qt::Key_Right) {
		puts("RIGHT");
	} else if (e->key() == Qt::Key_Up) {
		puts("UP");
		if (lastKey == Qt::Key_Down) {
			combKeyCounter++;
		} else {
			combKeyCounter = 0;
		}
	} else if (e->key() == Qt::Key_Down) {
		puts("DOWN");
		if (lastKey == Qt::Key_Up) {
			if (combKeyCounter >= 10) {
				emit passwordOk();
				emit hideKeyboard();
				this->close();
			}
		} else {
			combKeyCounter = 0;
		}
	} else if (e->key() == Qt::Key_Return) {
		processBtnClicked(0);
	} else if (e->key() == Qt::Key_Escape) {
		processBtnClicked(1);
	}

	lastKey = e->key();
}

void rkFmPw::hideEvent(QHideEvent *e)
{
	combKeyCounter = 0;
	return QDialog::hideEvent(e);
}

void rkFmPw::closeEvent(QCloseEvent *e)
{
	combKeyCounter = 0;
	return QDialog::closeEvent(e);
}
