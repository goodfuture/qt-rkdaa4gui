/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmaicalib.cpp
 * Created At       : 2013-09-24 09:15
 * Last Modified    : 2013-12-04 08:55
 * Description      : 
 * History	        : 
*******************************************************************************/

#include "rkfmaicalib.h"
#include "rkclr.h"
#include "rkcalib.h"
#include "rkckbox.h"
#include <qfont.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qapplication.h>

rkFmAiCalib :: rkFmAiCalib(QWidget *parent, const char *name)
	: rkFrame(parent, name)	
{
	setTitle("模拟量校准");
	int btnid = 0;
	calibChl = -1;

	mainfrm = createMainframe(this);

	wgtstack = new QWidgetStack(mainfrm);
	wgtstack->setFixedSize(640, 370);
	wgtstack->move(0, 0);

	wgt = new QWidget(wgtstack);
	wgtstack->addWidget(wgt, 0);
	wgtstack->raiseWidget(0);

	glayout = new QGridLayout(wgt);
	glayout->setRowSpacing(0, 30);
	glayout->setRowSpacing(2, 15);
	glayout->setRowStretch(8, 30);
	glayout->setColStretch(0, 0);
	glayout->setColStretch(3, 0);

	prompt = QString("说明：\n");
	prompt += "    1、模拟量校准支持单通道校准及连续校准两种校准方式。\n";
	prompt += "       按下屏幕下方对应按钮进入不同的校准模式。\n";
	prompt += "    2、校准过程分为偏置量校准及增益量校准。\n";
	prompt += "    3、校准过程中不得出现信号波动，否则会影响校准结果。\n";
	prompt += "    4、严格按照校准提示进行操作。\n";

	label = new QLabel(wgt);
	label->setText(prompt);
	glayout->addWidget(label, 1, 1);

	hlayout = new QHBoxLayout;
	hlayout->addStretch(10);
	glayout->addLayout(hlayout, 3, 1);

	btn = new QPushButton(wgt, QString::number(btnid++));
	btn->setText("单发校准");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);
	hlayout->addSpacing(10);

	btn = new QPushButton(wgt, QString::number(btnid++));
	btn->setText("连续校准");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);
	hlayout->addSpacing(10);

	btn = new QPushButton(wgt, QString::number(btnid++));
	btn->setText("返回");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);
	hlayout->addSpacing(10);

	wgt = new QWidget(wgtstack);
	wgtstack->addWidget(wgt, 1);

	glayout = new QGridLayout(wgt);
	glayout->setSpacing(10);
	glayout->setRowSpacing(0, 30);
	glayout->setRowSpacing(2, 10);
	glayout->setRowStretch(10, 30);
	glayout->setColStretch(0, 0);
	glayout->setColStretch(10, 0);

	label = new QLabel(wgt);
	label->setText("请选择待校准的模拟量通道:");
	glayout->addMultiCellWidget(label, 1, 1, 1, 9);

	for (id = 0; id < AI_NUM; id++) {
		int row = id / 4 + 3;
		int col = id % 4 * 2+ 1;

		label = new QLabel(wgt);
		label->setText("通道" + QString::number(id + 1));
		glayout->addWidget(label, row, col++);

		ckbox[id] = new rkCkBox(wgt, QString::number(id));
		ckbox[id]->setChecked(false);
		connect(ckbox[id], SIGNAL(checked(bool)), this, SLOT(procCkboxSel(bool)));
		glayout->addWidget(ckbox[id], row, col);
	}

	hlayout = new QHBoxLayout;
	hlayout->addStretch(10);
	glayout->addRowSpacing(8, 25);
	glayout->addMultiCellLayout(hlayout, 9, 9, 1, 9);

	btn = new QPushButton(wgt, QString::number(btnid++));
	btn->setText("开始校准");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);

	btn = new QPushButton(wgt, QString::number(btnid++));
	btn->setText("返回");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	hlayout->addWidget(btn);
	hlayout->addSpacing(10);

	wgt = new QWidget(wgtstack);
	wgt->setFixedSize(640, 370);
	wgtstack->addWidget(wgt, 2);

	glayout = new QGridLayout(wgt);
	glayout->setRowSpacing(0, 20);
	glayout->setRowStretch(8, 20);
	glayout->setColStretch(0, 0);
	glayout->setColStretch(10, 0);

	label = new QLabel(wgt);
	QFont font = label->font();
#ifdef __ARM__
	font.setPointSize(28);
#else
	font.setPointSize(18);
#endif
	label->setFont(font);
	label->setPaletteForegroundColor(rkClr(255, 0, 0));
	label->setAlignment(Qt::AlignCenter);
	label->setText("");
	glayout->addMultiCellWidget(label, 1, 1, 1, 9);
	glayout->setRowSpacing(2, 30);
	glayout->setColSpacing(5, 30);

	for (id = 0; id < 8; id++) {
		int row = id % 4 + 3;
		int col = id / 4 * 5 + 1;

		pmtlabel[id] = new QLabel(wgt);
		pmtlabel[id]->setText("");
		glayout->addMultiCellWidget(pmtlabel[id], row, row, col, col + 3);
	}

	timer3 = startTimer(1000);
}

void rkFmAiCalib :: showEvent(QShowEvent *)
{
}

void rkFmAiCalib :: hideEvent(QHideEvent *)
{
}

void rkFmAiCalib :: timerEvent(QTimerEvent *timer)
{
	int id = timer->timerId();

	if (id == timer3) {
		updateTime();
	}
}

void rkFmAiCalib :: procCkboxSel(bool checked)
{
	if (checkBox != NULL) {
		checkBox->setChecked(false);
	}
	checkBox = (rkCkBox *)this->sender();
	if (checked) {
		calibChl = QString(checkBox->name()).toInt();
	} else {
		calibChl = -1;
	}
}

void rkFmAiCalib :: procBtnPressed()
{
	btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();
	int ret;

	switch(index) {
		case 0: //!单发模式
			setTitle("模拟量单通道校准");
			qApp->processEvents();
			wgtstack->raiseWidget(1);
			break;
		case 2: //!返回
			close();
			break;
		case 1: //!连续模式
			setTitle("模拟量连续校准");
			wgtstack->raiseWidget(2);
			qApp->processEvents();
			QMessageBox::information(this, "提示", "开始偏置信号校准!\n请将信号源调整至0V或0mA", "确定");
			pmtlabel[7]->clear();
			for (int index = 0; index < AI_NUM; index++) {
				pmtlabel[3]->setText("通道" + QString::number(index + 1) + "偏置校准中...");
				qApp->processEvents();
				if (rkCalibExec(index, 0) == -1) {
					ret = QMessageBox::information(this, "错误", "通道" + QString::number(index + 1) + "偏置信号校准失败!\n请检查AI模块驱动及信号源!", "退出", "继续");
					if (ret == 0) {
						wgtstack->raiseWidget(0);
						setTitle("模拟量校准");
						return;
					}
				}
			}
			QMessageBox::information(this, "提示", "偏置信号校准完成,开始增益信号校准!\n请将信号源调整至5V或20mA", "确定");
			pmtlabel[3]->setText("偏置信号校准完成！");
			for (int index = 0; index < AI_NUM; index++) {
				pmtlabel[7]->setText("通道" + QString::number(index + 1) + "增益校准中...");
				qApp->processEvents();
				if (rkCalibExec(index, 1) == -1) {
					ret = QMessageBox::information(this, "错误", "通道" + QString::number(index + 1) + "增益信号校准失败!\n请检查AI模块驱动及信号源!", "退出", "继续");
					if (ret == 0) {
						wgtstack->raiseWidget(0);
						setTitle("模拟量校准");
						return;
					}
				}
			}
			rkGenCalibFile(QString(DEF_CFG_FILE_DIR) + "/" + AI_CALIB_FILE);
			wgtstack->raiseWidget(0);
			if (QMessageBox::information(this, "提示", "校准完成!\n是否使校准立即生效？", "确定", "取消") == 0) {
				emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
			}
			break;
		case 3: //!开始校准
			if (calibChl >= 0) {
				wgtstack->raiseWidget(2);
				QMessageBox::information(this, "提示", "开始偏置信号校准!\n请将信号源调整至0V或0mA!", "确定");
				rkCalibInit();

				pmtlabel[3]->setText("通道" + QString::number(calibChl + 1) + "偏置校准中...");
				pmtlabel[7]->clear();
				qApp->processEvents();
				if (rkCalibExec(calibChl, 0) == -1) {
					QMessageBox::information(this, "错误", "偏置信号校准失败!\n请检查是否正确加载AI模块驱动!", "确定");
					wgtstack->raiseWidget(0);
					setTitle("模拟量校准");
					return;

				}
				QMessageBox::information(this, "提示", "偏置信号校准完成,开始增益信号校准!\n请将信号源调整至5V或20mA", "确定");
				pmtlabel[3]->setText("偏置信号校准完成！");
				pmtlabel[7]->setText("通道" + QString::number(calibChl + 1) + "增益校准中...");
				qApp->processEvents();
				if (rkCalibExec(calibChl, 1) == -1) {
					QMessageBox::information(this, "错误", "增益信号校准失败!\n!", "确定");
					wgtstack->raiseWidget(0);
					setTitle("模拟量校准");
					return;
				}
				rkGenCalibFile(QString(DEF_CFG_FILE_DIR) + "/" + AI_CALIB_FILE);
				if (QMessageBox::information(this, "提示", "校准完成!\n是否使校准立即生效？", "确定", "取消") == 0) {
					emit sendUICmd(UI_CMD_CORE_PROG_EXIT);
				}
				wgtstack->raiseWidget(0);
			} else {
				QMessageBox::information(this, "提示", "请先选择待校准的通道！", "确定");
			}
			break;
		case 4: //!返回
			wgtstack->raiseWidget(0);
			setTitle("模拟量校准");
			break;
	}
}
