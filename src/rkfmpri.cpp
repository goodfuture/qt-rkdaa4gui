/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmpri.cpp
 * Created At       : 2013-11-22 09:09
 * Last Modified    : 2014-01-23 09:40
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "ui/rkmsgview.h"
#include "rkfmpri.h"
#include "rkfmset.h"
#include "rkfmview.h"
#include "rkfmexp.h"
#include "rkfmqrymsg.h"
#include "rkfmqryrtd.h"
#include "rkfminfo.h"
#include "rkfmpw.h"
#include "rktrdmsg.h"
#include "rkfmscrcalib.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qstringlist.h>

extern rkFmScrCalib *scrcalib;

rkFmPri::rkFmPri(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	this->setFocusPolicy(QWidget::NoFocus);
	focusId = 0;
	curpos = 0;
	keyRepeat = false;
	combkeycnt = 0;

	/* Inititialize */
	QPixmap pixmap;
	int pos_x[3] = { 80, 330, 580 };
	int pos_y[2] = { 120, 285 };
	QString imageNamePool = "btn_prm,btn_rtd,btn_exp_data,btn_qry_msg,btn_qry_data,btn_about";
	QStringList imageNameList = QStringList::split(",", imageNamePool, false);
	setTitle("环保数采仪主页");

	/* Create Icon Button */
	for(unsigned int i = 0; i < imageNameList.count(); i++) {
		btn[i] = new QPushButton(this, QString::number(i));
		btn[i]->setFixedSize(140, 125);
		if(i < 3) {
			btn[i]->move(pos_x[i], pos_y[0]);
		} else {
			btn[i]->move(pos_x[i - 3], pos_y[1]);
		}
		pixmap = QPixmap::fromMimeSource(imageNameList[i] + "_arm.png");
		btn[i]->setPixmap(pixmap);
		connect(btn[i], SIGNAL(clicked()), this, SLOT(showSubform()));
	}
	btn[focusId]->setFocus();

	/* Create Sub Page */
	paramSetForm = new rkFmSet(this);
	dataViewForm = new rkFmView(this); 
	dataExportForm = new rkFmExp(this);
	msgQueryForm = new rkMsgView(this);
	dataQueryForm = new rkFmQryRtd(this);
	aboutForm = new rkFmInfo(this);
	loginform = new rkFmPw(this); /* Login form */
	//scrcalib = new rkFmScrCalib(this);
	connect(loginform, SIGNAL(dispKeyboard(int)), this, SLOT(showKeyboard(int)));
	connect(loginform, SIGNAL(hideKeyboard()), this, SLOT(hideKeyboard()));
	connect(loginform, SIGNAL(passwordOk()), this, SLOT(showSettingForm()));
	connect(this, SIGNAL(sendKeyCode(const QString &)), loginform, SLOT(recvKeyCode(const QString &)));

#if 1
	/* Create Messager Thread And Run It */
	rkTrdMsg *thread = new rkTrdMsg();
	thread->start();
#endif
	connect(thread, SIGNAL(sendCoreStatus(bool, const char *)), aboutForm, SLOT(setCoreStatus(bool, const char *)));
	connect(paramSetForm, SIGNAL(sendUICmd(int)), thread, SLOT(procUICmd(int)));
	connect(msgQueryForm, SIGNAL(sendUICmd(int)), thread, SLOT(procUICmd(int)));
}

void rkFmPri::recvKeyCode(const QString &keyCode)
{
	if (keyCode.compare("Hide") == 0) {
		hideKeyboard();
	} else {
		emit sendKeyCode(keyCode);
	}
}

void rkFmPri::showSubform(int id)
{
	loginform->hide();
	switch(id) {
		case 0: /* Parameter Set */
			loginform->show();
			loginform->setFocus();
			break;
		case 1: /* Real Time Data View */
			dataViewForm->show();
			dataViewForm->setFocus();
			break;
		case 2: /* Data Export */
			dataExportForm->show();
			dataExportForm->setFocus();
			break;
		case 3: /* Message Query */
			msgQueryForm->show();
			msgQueryForm->setFocus();
			break;
		case 4: /* Data Query */
			dataQueryForm->show();
			dataQueryForm->setFocus();
			break;
		case 5: /* About */
			aboutForm->show();
			aboutForm->setFocus();
			break;
		default:
			break;
	}
}

void rkFmPri::showSubform()
{
	QPushButton *btn = (QPushButton *)this->sender();
	QString btnName = btn->name();

	if (loginform->isShown()) {
		loginform->hide();
	}
	hideKeyboard();

	showSubform(btnName.toInt());
}

void rkFmPri::showSettingForm()
{
	paramSetForm->show();
}

void rkFmPri::keyPressEvent(QKeyEvent *e)
{
}

void rkFmPri::keyReleaseEvent(QKeyEvent *e)
{
	//puts("Release");
	if (keyRepeat == true) {
		return;
	} else if (e->key() == Key_Escape) {
		puts("ESC");
		btn[focusId]->clearFocus();
		if (lastkey == Key_Return) {
			combkeycnt++;
		} else {
			combkeycnt = 0;
		}
		lastkey = e->key();
		return;
		//puts("Esc");
	} else if (e->key() == Key_Return) {
		puts("Return");
		if (lastkey == Key_Escape) {
			if (combkeycnt >= 10) {
				//puts("CALIBRATE");
				scrcalib->show();
				//scrcalib->setFocus();
				lastkey = Key_Left;
				return;
			}
		} else {
			combkeycnt = 0;
		}
		lastkey = e->key();
		return;
		//puts("Return");
	}
	//puts("PASS");

	if (e->key() == Key_Up || e->key() == Key_Left) {
		focusId--;
		if (focusId < 0) {
			focusId = 5;
		}
		btn[focusId]->setFocus();
		combkeycnt = 0;
		puts("Front");
	} else if (e->key() == Key_Down || e->key() == Key_Right) {
		focusId++;
		if (focusId > 5) {
			focusId = 0;
		}
		btn[focusId]->setFocus();
		combkeycnt = 0;
		puts("Behind");
	} else if (e->key() == Key_Return) {
	} else if (e->key() == Key_Escape) {
	}

	lastkey = e->key();
	keyRepeat = true;
	keyTimer = startTimer(200);

	return;
}

void rkFmPri::timerEvent(QTimerEvent *e)
{
	if (keyTimer == e->timerId()) {
		keyRepeat = false;
		killTimer(keyTimer);
	}

	return rkFrame::timerEvent(e);
}

bool rkFmPri::focusNextPrevChild(bool next)
{
}
