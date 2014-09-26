#include "rkfmcalib.h"
#include "rkfmscrcalib.h"
#include "rkfmaicalib.h"
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfont.h>

extern rkFmScrCalib *scrcalib;

rkFmCalib :: rkFmCalib(QWidget *parent, const char *name)
	: rkFrame(parent, name)
{
	setTitle("校准");

	QFont font = this->font();
#ifdef __ARM__
	font.setPointSize(24);
#else
	font.setPointSize(14);
#endif
	this->setFont(font);

	mainfrm = createMainframe(this);

	glayout = new QGridLayout(this);
	glayout->setRowStretch(0, 10);
	glayout->setRowStretch(2, 10);
	glayout->setColStretch(0, 10);
	glayout->setColStretch(2, 10);

	vlayout = new QVBoxLayout;
	vlayout->addStretch(10);
	glayout->addLayout(vlayout, 1, 1);

	btn = new QPushButton(this, "0");
	btn->setText("模拟量校准");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	vlayout->addWidget(btn);
	vlayout->addSpacing(10);

	btn = new QPushButton(this, "1");
	btn->setText("触摸屏校准");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	vlayout->addWidget(btn);
	vlayout->addSpacing(10);

	btn = new QPushButton(this, "2");
	btn->setText("返回上一级");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));
	vlayout->addWidget(btn);
	vlayout->addStretch(10);

	aicalib = new rkFmAiCalib(this);
	connect(aicalib, SIGNAL(sendUICmd(int)), this, SLOT(forwardUICommand(int)));
}

void rkFmCalib :: procBtnPressed()
{
	btn = (QPushButton *)this->sender();
	int id = QString(btn->name()).toInt();

	switch(id) {
		case 0: //!模拟量校准
			aicalib->show();
			break;
		case 1: //!触摸屏校准
			scrcalib->show();
			break;
		case 2: //!返回
			close();
			break;
	}
}

void rkFmCalib::forwardUICommand(int cmd)
{
	emit sendUICmd(cmd);
}
