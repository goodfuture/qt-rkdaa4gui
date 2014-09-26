/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmqrymsg.cpp
 * Created At       : 2013-11-18 10:43
 * Last Modified    : 2013-11-20 10:34
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "ui/rkmsgview.h"
#include "rktbl.h"
#include "rkclr.h"
#include "rktype.h"
#include "rkfmqrymsg.h"
#include <time.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qstringlist.h>
#include <qdatetimeedit.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qmessagebox.h>
#include <qsqldatabase.h>

#define TBL_ROW_NUM 20

extern struct context ctx;

rkFmQryMsg::rkFmQryMsg(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	setTitle("报文查询");

	mainfrm = createMainframe(this);

	QFont font = this->font();
	font.setPointSize(24);
	this->setFont(font);

	glayout = new QGridLayout(this);
	glayout->setSpacing(8);
	glayout->setRowStretch(0, 10);
	glayout->setRowSpacing(4, 10);
	glayout->setRowStretch(6, 10);
	glayout->setColStretch(0, 1); 
	glayout->setColSpacing(2, 360);
	glayout->setColStretch(3, 1); 

	label = new QLabel(this);
	label->setText("报文类型");
	glayout->addWidget(label, 1, 1); 
	label = new QLabel(this);
	label->setText("起始时间");
	glayout->addWidget(label, 2, 1); 
	label = new QLabel(this);
	label->setText("终止时间");
	glayout->addWidget(label, 3, 1); 

	combox = new QComboBox(this);
	combox->insertItem("实时报文", 0); 
	combox->insertItem("分钟报文", 1); 
	combox->insertItem("小时报文", 2); 
	combox->insertItem("日报文", 3); 
	glayout->addWidget(combox, 1, 2); 

	tmedit[0] = new QDateTimeEdit(QDateTime::currentDateTime(), this);
	tmedit[0]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[0], 2, 2);

	tmedit[1] = new QDateTimeEdit(QDateTime::currentDateTime(), this);
	tmedit[1]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[1], 3, 2);

	QHBoxLayout *layout = new QHBoxLayout;
	glayout->addMultiCellLayout(layout, 5, 5, 1, 2);

	layout->setSpacing(10);
	layout->addStretch(10);
	btn = new QPushButton(this, "0");
	btn->setPixmap(QPixmap::fromMimeSource("btn_ret_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	layout->addWidget(btn);

	btn = new QPushButton(this, "1");
	btn->setPixmap(QPixmap::fromMimeSource("btn_qry_arm.png"));
	btn->setFixedSize(80, 80);
	connect(btn, SIGNAL(clicked()), this, SLOT(clickProc()));
	layout->addWidget(btn);

	/* Create Message View Page */
	view = new rkMsgView(this);
	connect(this, SIGNAL(sendQryCond(int, unsigned long, unsigned long)), view, SLOT(recvQryCond(int, unsigned long, unsigned long)));
}

void rkFmQryMsg::clickProc()
{
	static int rc, row, col, pagenum = 0, pagecnt= 0;
	char *sql, *err;
	time_t stm, etm;

	QPushButton *btn = (QPushButton *)this->sender();
	int index = QString(btn->name()).toInt();

	if (index == 0) {
		close();
	} if (index == 1) { /* 查询 */
		//view->show();
		stm = tmedit[0]->dateTime().toTime_t();
		etm = tmedit[1]->dateTime().toTime_t();
		if (stm > etm) {
			QMessageBox::warning(this, "查询失败", "检测到起始时间大于结束时间!", "确定");
			return;
		}

		if (combox->currentItem() == 0 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 1) {
			QMessageBox::warning(this, "查询失败", "实时报文查询时间跨度不能超过1天!", "确定");
			return;
		} else if (combox->currentItem() == 1 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 2) {
			QMessageBox::warning(this, "查询失败", "分钟报文查询时间跨度不能超过1天!", "确定");
			return;
		} else if (combox->currentItem() == 2 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 10) {
			QMessageBox::warning(this, "查询失败", "小时报文查询时间跨度不能超过10天!", "确定");
			return;
		} else if (combox->currentItem() == 3 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 365) {
			QMessageBox::warning(this, "查询失败", "日报文查询时间跨度不能超过1年!", "确定");
			return;
		}
		emit sendQryCond(combox->currentItem(), stm, etm);
	}
}

void rkFmQryMsg::keyPressEvent(QKeyEvent *e)
{
}

void rkFmQryMsg::keyReleaseEvent(QKeyEvent *e)
{
}
