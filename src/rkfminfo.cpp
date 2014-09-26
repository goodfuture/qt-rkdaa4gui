#include "rktype.h"
#include "rkshm.h"
#include "rkfminfo.h"
#include "rklnedit.h"
#include "rkdlgsel.h"
#include <qlayout.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qtoolbutton.h>
#include <qapplication.h>

extern struct context ctx;

rkFmInfo::rkFmInfo(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("关于");

	/* Main Frame */
	mainfrm = createMainframe(this);

	int row = 1, col = 1;
	frmlayout = new QGridLayout(mainfrm);
	frmlayout->setSpacing(10);
	frmlayout->addRowSpacing(0, 30);
	frmlayout->setRowStretch(10, 30);
	frmlayout->setColStretch(0, 30);
	frmlayout->setColStretch(10, 30);

	label = new QLabel(mainfrm);
	label->setText("界面程序版本");
	frmlayout->addWidget(label, row++, col);

	label = new QLabel(mainfrm);
	label->setText("内核程序版本");
	frmlayout->addWidget(label, row++, col);

	row = 1, col = 2;

	vlabel[0] = new QLabel(mainfrm);
	vlabel[0]->setText((GUI_VERSION));
	frmlayout->addWidget(vlabel[0], row++, col);

	vlabel[1] = new QLabel(mainfrm);
	vlabel[1]->clear();
	frmlayout->addWidget(vlabel[1], row++, col);

	/* Icon Button */
	QPushButton *iconBtn = new QPushButton(mainfrm, QString::number(0));
	QPixmap pixmap = QPixmap::fromMimeSource("btn_ret_arm.png");
	iconBtn->setPixmap(pixmap);
	iconBtn->setFixedSize(80, 80);
	iconBtn->move(560, 260);
	connect(iconBtn, SIGNAL(clicked()), this, SLOT(procBtnPressed()));

	timer1 = startTimer(2000);
	hide();
}

void rkFmInfo::timerEvent(QTimerEvent *timer)
{
	if (timer->timerId() == timer1) {
		updateTime();
	}
}

void rkFmInfo::setCoreStatus(bool run, const char *version)
{
	if (!run) {
		vlabel[1]->setText(QString("未运行"));
	} else {
		vlabel[1]->setText(QString(version));
	}
}

void rkFmInfo::procBtnPressed()
{
	hideKeyboard();
	close();
}
