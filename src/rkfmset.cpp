/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmset.cpp
 * Created At       : 2013-11-11 15:44
 * Last Modified    : 2014-01-23 09:42
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rkfmset.h"
#include "rksetanalog.h"
#include "rksetsys.h"
#include "rksetnet.h"
#include "rksetdio.h"
#include "rksetcom.h"
#include "rkfmcalib.h"
#include "rksetdo.h"
#include "rksetvar.h"
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <stdlib.h>

rkFmSet::rkFmSet(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("参数设置");
	scrLock = false;
	QString imageNamePool = "btn_sys,btn_com,btn_net,btn_ai,btn_ser,btn_dio,btn_do,btn_dtu,btn_calib,btn_home";
	QStringList imageNameList = QStringList::split(",", imageNamePool, false);

	/* Icon Button */
	int pos_x[5] = { 80, 215, 350, 485, 620};
	int pos_y[2] = { 140, 295};
	for(int i = 0; i < 10; i++) {
		iconBtn[i] = new QPushButton(this, QString::number(i));
		iconBtn[i]->setFixedSize(100, 95);
		QPixmap pixmap = QPixmap::fromMimeSource(imageNameList[i] + "_arm.png");
		iconBtn[i]->adjustSize();
		iconBtn[i]->setPixmap(pixmap);
		if (i < 5) {
			iconBtn[i]->move(pos_x[i], pos_y[0]);
		} else {
			iconBtn[i]->move(pos_x[i - 5], pos_y[1]);
		}
		connect(iconBtn[i], SIGNAL(clicked()), this, SLOT(clickProc()));
	}

	/* Create Child Page */
	settingForm1 = new rkSetSys(this); /* System setting */
	connect(settingForm1, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm2 = new rkSetCom(this); /* Uart setting */
	connect(settingForm2, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm3 = new rkSetNet(this); /* Network setting */
	connect(settingForm3, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm4 = new rkSetAnalog(this); /* Analog setting */
	connect(settingForm4, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm5 = new rkSetVar(this); /* Uart variable */
	connect(settingForm5, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm6 = new rkSetDio(this); /* Switch variable */
	connect(settingForm6, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	settingForm7 = new rkSetDo(this); /* Switch control */
	connect(settingForm7, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	calibrateForm = new rkFmCalib(this); /* Touch Screen Calibrate */
	connect(calibrateForm, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICmd(int)));
	hide();
}

void rkFmSet::forwardUICmd(int cmd)
{
	emit sendUICmd(cmd);
}

void rkFmSet::clickProc()
{
	if (scrLock) return;

	QPushButton *iconBtn = (QPushButton *)this->sender();
	QString btnName = iconBtn->name();
	QProcess *proc;
	switch(btnName.toInt()) {
		case 0: /* System setting */
			settingForm1->show();
			break;
		case 1: /* Uart setting */
			settingForm2->show();
			break;
		case 2: /* Network setting */
			settingForm3->show();
			break;
		case 3: /* Analog variables setting */
			settingForm4->show();
			break;
		case 4: /* Uart variables setting */
			settingForm5->show();
			break;
		case 5: /* Switch variables setting */
			settingForm6->show();
			break;
		case 6: /* Switch control */
			settingForm7->show();
			break;
		case 7: /* DTU setting */
			scrLock = true;
			emit sendUICmd(UI_CMD_DISABLE_DTU);
			proc = new QProcess(QString(DTU_CFG_PROG_PATH), this);
			connect(proc, SIGNAL(processExited()), this, SLOT(childProcessExited()));
			if (!proc->start()) {
				QMessageBox::critical(this, "错误", "加载DTU配置程序失败！", "确定");
			}
			//scrLock = false;
			break;
		case 8: /* Touch screen calibrate */
#if 0
			scrLock = true;
			enDialog = true;
			proc = new QProcess(QString("ts_calibrate"), this);
			if (!proc->start()) {
				QMessageBox::critical(this, "错误", "加载校准程序失败！", "确定");
			} 
			connect(proc, SIGNAL(processExited()), this, SLOT(childProcessExited()));
			scrLock = false;
#endif
			calibrateForm->show();
			//sleep(1);
			//calibrateForm->startCalibrate();
			break;
		case 9: /* Return to main page */
			this->close();
			break;
		default:
			break;
	}
}

void rkFmSet::childProcessExited()
{
	show();
	update();
	scrLock = false;
	emit sendUICmd(UI_CMD_ENABLE_DTU);
#if 0
	if (enDialog) {
		switch(QMessageBox::question(this, "", "校准成功，是否重启以使校准生效?", "确定", "取消")) {
			case 0: /* Ok */
				system("reboot");
				break;
			case 1: /* Cancel */
				return;
		}
		enDialog = false;
	}
#endif
}

bool rkFmSet::eventFilter(QObject *obj, QEvent *event)
{
	if (scrLock == true){
		return false;
	} else {
		return QWidget::eventFilter(obj, event);
	}
}
