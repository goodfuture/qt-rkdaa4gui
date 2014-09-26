/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmview.cpp
 * Created At       : 2013-09-26 13:17
 * Last Modified    : 2014-01-14 11:24
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rktype.h"
#include "rkfmview.h"
#include "rkhisto.h"
#include "rkplotter.h"
#include "rkclr.h"
#include "rktbl.h"
#include "rktabwgt.h"
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qwidgetstack.h>
#include <qvaluevector.h>
#include <qpoint.h>
#include <unistd.h>
#include <math.h>
#include <qmessagebox.h>

extern struct context ctx;

rkFmView::rkFmView(QWidget *parent, const char *name)
	:rkFrame(parent, name)
{
	/* Set Title */
	setTitle("实时数据");
	isTableShown = true;

	/* Main Frame */
	mainfrm = createMainframe(this);

	/* Add tabWidget to the main frame */
	tabWidget = new rkTabWgt(mainfrm);
	tabWidget->setFixedSize(640, 370);
	tabWidget->move(0, 0);

	tlabel[0] = QStringList::split(",", "通道ID,状态,名称,实时值,时均值");
	tlabel[1] = QStringList::split(",", "通道ID,状态,名称,实时值,时均值");
	tlabel[2] = QStringList::split(",", "通道ID,名称,当前状态,通道ID,名称,当前状态");
	QStringList tablab = QStringList::split(",", "模拟量,串口量,开关量");

	int rownum[3];
	rownum[0] = AI_NUM;
	rownum[1] = EI_NUM;
	rownum[2] = DI_NUM > DO_NUM ? DI_NUM : DO_NUM;

	for (int i = 0; i < 3; i++) {
		wstack[i] = new QWidgetStack(tabWidget);
		tbl[i] = new rkTbl(wstack[i]); 
		tbl[i]->move(0, 0);
		tbl[i]->setNumRows(rownum[i]);
		tbl[i]->setNumCols(tlabel[i].count());
		tbl[i]->setLeftMargin(0);
		tbl[i]->setColumnLabels(tlabel[i]);
		tbl[i]->setReadOnly(true);
		tbl[i]->setSelectionMode(QTable::SingleRow);
		for (int j = 0; j < tbl[i]->numCols(); j++) {
			tbl[i]->setColumnStretchable(j, true);
		}

		for (int j = 0; j < tbl[i]->numRows(); j++) {
			tbl[i]->setRowHeight(j, 30);
		}
		wstack[i]->addWidget(tbl[i], 0);
		plotter[i] = new rkPlotter(wstack[i]);
		//pointvector.clear();
		//plotter[i]->setCurveData(i, "curve0", pointvector);
		wstack[i]->addWidget(plotter[i], 1);
		wstack[i]->raiseWidget(0);
		tabWidget->addTab(wstack[i], tablab[i]);
	}

	/* Left Frame */
	leftfrm = createLeftFrame(this);
	vlayout = new QVBoxLayout(leftfrm, 3, 7);
	lbtnPicPrefix = QStringList::split(",", "btn_tbl,btn_crv", false);
	for (unsigned int i = 0; i < 2; i++) {
		lbtn[i] = new QPushButton(leftfrm, QString::number(i));
		QPixmap pixmap = !i ? QPixmap::fromMimeSource(lbtnPicPrefix[i] + "_on_arm.png") : QPixmap::fromMimeSource(lbtnPicPrefix[i] + "_off_arm.png");
		lbtn[i]->setPixmap(pixmap);
		lbtn[i]->setFixedSize(74, 116);
		connect(lbtn[i], SIGNAL(clicked()), this, SLOT(procLBtnClick()));
		vlayout->addWidget(lbtn[i]);
	}

	/* Right Frame */
	rightfrm = createRightFrame(this);
	vlayout = new QVBoxLayout(rightfrm, 3, 7);
	rbtnPicPrefix = QStringList::split(",", "btn_frtpg,btn_nxtpg,btn_bk", false);
	for (unsigned int i = 0; i < 3; i++) {
		rbtn[i] = new QPushButton(rightfrm, QString::number(i));
		QPixmap pixmap = QPixmap::fromMimeSource(rbtnPicPrefix[i] + "_arm.png");
		rbtn[i]->setPixmap(pixmap);
		rbtn[i]->setFixedSize(74, 116);
		connect(rbtn[i], SIGNAL(clicked()), this, SLOT(procRBtnClick()));
		vlayout->addWidget(rbtn[i]);
	}
	hide();

	timer1 = startTimer(3000);
	timer2 = startTimer(1000);
}

void rkFmView::timerEvent(QTimerEvent *timer)
{
	if (isShown()) {
		if (timer->timerId() == timer1) {
			if (isTableShown) {
				switch(tabWidget->currentPageIndex()) {
					case 0:
						updateAiTbl();
						break;
					case 1:
						updateEiTbl();
						break;
					case 2:
						updateDioTbl();
						break;
					default:
						break;
				}
			}
		} else if (timer->timerId() == timer2) {
			updateTime();
		}
	}
}

void rkFmView::btnChangePic(int index)
{
	for (int i = 0; i < lbtnPicPrefix.count(); i++) {
		QString picPath = lbtnPicPrefix[i];
		picPath += index == i ? "_on_arm.png" : "_off_arm.png";
		lbtn[i]->setPixmap(QPixmap::fromMimeSource(picPath));
	}
}

void rkFmView::procLBtnClick()
{
	const QObject *obj = this->sender();
	int index = QString(obj->name()).toInt();
	btnChangePic(index);

	isTableShown = index == 0 ? true : false;
	if (isTableShown == false) {
		QMessageBox::information(this, "提示", "抱歉！此功能尚未实现!", "确定");
		btnChangePic(0);
		return;
	}

	for (int i = 0; i < tabWidget->count(); i++) {
		wstack[i]->raiseWidget(index);
	}
}

void rkFmView::procRBtnClick()
{
	const QObject *obj = this->sender();
	QString name = obj->name();
	int index = name.toInt();

	if (index == 0) { /* 前一页 */
		index = tabWidget->currentPageIndex();
		if (index - 1 >= 0) {
			tabWidget->setCurrentPage(index - 1);
		} else {
			tabWidget->setCurrentPage(tabWidget->count() - 1);
		}
	} else if (index == 1) {  /* 后一页 */
		index = tabWidget->currentPageIndex();
		if ((index + 1) < tabWidget->count()) {
			tabWidget->setCurrentPage(index + 1);
		} else {
			tabWidget->setCurrentPage(0);
		}
	} else if (index == 2) {
		close();
	}
} 
void rkFmView::updateAiTbl(QString &str)
{
}

#define VIEW_SCIENCE_MODE

void rkFmView::updateAiTbl()
{
	QString disp;

	for(int index = 0; index < AI_NUM; index++) {
		if (ctx.ai[index].inuse == 0) {
			tbl[0]->setText(index, 0, QString::number(index));
			tbl[0]->setText(index, 1, "禁用");
			tbl[0]->setText(index, 2, "-");
			tbl[0]->setText(index, 3, "-");
			tbl[0]->setText(index, 4, "-");
			continue;
		} 
		tbl[0]->setText(index, 0, QString::number(index + 1));
		if (ctx.ai[index].alm == ALARM_N) {
			tbl[0]->setText(index, 1, "正常");
		} else if (ctx.ai[index].alm == ALARM_H) {
			tbl[0]->setText(index, 1, "高");
		} else if (ctx.ai[index].alm == ALARM_L) {
			tbl[0]->setText(index, 1, "低");
		} else {
			tbl[0]->setText(index, 1, "错误");
			tbl[0]->setText(index, 2, "*");
			tbl[0]->setText(index, 3, "*");
			tbl[0]->setText(index, 4, "*");
			continue;
		}

		tbl[0]->setText(index, 2, QString(ctx.ai[index].code));
#ifdef VIEW_SCIENCE_MODE
		if (ctx.ai[index].vals[RTV_OFFSET] >= 1000) {
			disp.sprintf("%.3g", ctx.ai[index].vals[RTV_OFFSET]);
		} else if (ctx.ai[index].vals[RTV_OFFSET] >= 100) {
			disp.sprintf("%.1f", ctx.ai[index].vals[RTV_OFFSET]);
		} else if (ctx.ai[index].vals[RTV_OFFSET] >= 10) {
			disp.sprintf("%.2f", ctx.ai[index].vals[RTV_OFFSET]);
		} else {
			disp.sprintf("%.3f", ctx.ai[index].vals[RTV_OFFSET]);
		}
#else
		disp.sprintf("%.3f", ctx.ai[index].vals[RTV_OFFSET]);
#endif
		tbl[0]->setText(index, 3, disp);

#ifdef VIEW_SCIENCE_MODE
		if (ctx.ai[index].vals[HAV_OFFSET] >= 1000) {
			disp.sprintf("%.3g", ctx.ai[index].vals[HAV_OFFSET]);
		} else if (ctx.ai[index].vals[HAV_OFFSET] >= 100) {
			disp.sprintf("%.1f", ctx.ai[index].vals[HAV_OFFSET]);
		} else if (ctx.ai[index].vals[HAV_OFFSET] >= 10) {
			disp.sprintf("%.2f", ctx.ai[index].vals[HAV_OFFSET]);
		} else {
			disp.sprintf("%.3f", ctx.ai[index].vals[HAV_OFFSET]);
		}
#else
		disp.sprintf(".3%f", ctx.ai[index].vals[HAV_OFFSET]);
#endif
		tbl[0]->setText(index, 4, disp);
	}
}

void rkFmView::updateEiTbl()
{
	QString disp;

	for(int index = 0; index < EI_NUM; index++) {
		tbl[1]->setText(index, 0, QString::number(index + 1));
		if (ctx.ei[index].inuse == 0) {
			tbl[1]->setText(index, 1, "禁用");
			tbl[1]->setText(index, 2, "-");
			tbl[1]->setText(index, 3, "-");
			tbl[1]->setText(index, 4, "-");
			//tbl[1]->setText(index, 5, "-");
			//tbl[1]->setText(index, 6, "-");
			continue;
		}

		if (ctx.ei[index].alm != ALARM_N) {
			tbl[1]->setText(index, 1, "错误");
			tbl[1]->setText(index, 2, QString(ctx.ei[index].code));
			tbl[1]->setText(index, 3, "##");
			tbl[1]->setText(index, 4, "##");
			//tbl[1]->setText(index, 5, "##");
			//tbl[1]->setText(index, 6, "##");
			continue;
		}

		tbl[1]->setText(index, 1, "正常");
		tbl[1]->setText(index, 2, QString(ctx.ei[index].code));
#ifdef VIEW_SCIENCE_MODE
		if (ctx.ei[index].vals[RTV_OFFSET] >= 99999) {
			disp.sprintf("%.3g", ctx.ei[index].vals[RTV_OFFSET]);
		} else if (ctx.ei[index].vals[RTV_OFFSET] >= 9999) {
			disp.sprintf("%.1f", ctx.ei[index].vals[RTV_OFFSET]);
		} else if (ctx.ei[index].vals[RTV_OFFSET] >= 999) {
			disp.sprintf("%.2f", ctx.ei[index].vals[RTV_OFFSET]);
		} else if (ctx.ei[index].vals[RTV_OFFSET] >= 99) {
			disp.sprintf("%.3f", ctx.ei[index].vals[RTV_OFFSET]);
		} else {
			disp.sprintf("%.4f", ctx.ei[index].vals[RTV_OFFSET]);
		}
#else
		disp.sprintf("%.3f", ctx.ei[index].vals[RTV_OFFSET]);
#endif
		tbl[1]->setText(index, 3, disp);

#ifdef VIEW_SCIENCE_MODE
		if (ctx.ei[index].vals[HAV_OFFSET] >= 99999) {
			disp.sprintf("%.3g", ctx.ei[index].vals[HAV_OFFSET]);
		} else if (ctx.ei[index].vals[HAV_OFFSET] >= 9999) {
			disp.sprintf("%.1f", ctx.ei[index].vals[HAV_OFFSET]);
		} else if (ctx.ei[index].vals[HAV_OFFSET] >= 999) {
			disp.sprintf("%.2f", ctx.ei[index].vals[HAV_OFFSET]);
		} else if (ctx.ei[index].vals[HAV_OFFSET] >= 99) {
			disp.sprintf("%.3f", ctx.ei[index].vals[HAV_OFFSET]);
		} else {
			disp.sprintf("%.4f", ctx.ei[index].vals[HAV_OFFSET]);
		}
#else
		disp.sprintf("%.3f", ctx.ei[index].vals[HAV_OFFSET]);
#endif
		tbl[1]->setText(index, 4, disp);
	}
}

void rkFmView::updateDioTbl()
{
	QString str;
	int row = 0;

	for (int index = 0; index < DI_NUM; index++) {
		//printf("DI = %d\n", index);
		tbl[2]->setText(row, 0, "输入通道" + QString::number(index + 1));
		tbl[2]->setText(row, 1, ctx.dip[index].code);
		if (ctx.dip[index].inuse) {
			str = !ctx.dip[index].val ? "关闭" : "开启";
		} else {
			str = "禁用";
		}
		tbl[2]->setText(row, 2, str);
		row ++;
	}

	row = 0;
	for (int index = 0; index < DO_NUM; index++) {
		//printf("DO = %d\n", index);
		tbl[2]->setText(row, 3, "输出通道" + QString::number(index + 1));
		tbl[2]->setText(row, 4, ctx.dop[index].code);
		if (ctx.dop[index].inuse) {
			str = !ctx.dop[index].val ? "关闭" : "开启";
		} else {
			str = "禁用";
		}
		tbl[2]->setText(row, 5, str);
		row++;
	}
}
