/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmview.h
 * Created At       : 2013-11-18 10:43
 * Last Modified    : 2013-11-22 09:26
 * Description      : 
 * History	        : 
*******************************************************************************/
#ifndef _RK_FORM_DATA_VIEW_H_
#define _RK_FORM_DATA_VIEW_H_ 

#include "rktype.h"
#include "rkfrm.h"
#include "rkshm.h"
#include <qstringlist.h>
#include <qvaluevector.h>
#include <qmap.h>
#include <qevent.h>
#include <qvaluevector.h>
#include <qvaluelist.h>

class rkTbl;
class QFrame;
class QTable;
class QGridLayout;
class QPushButton;
class QWidgetStack;
class rkTabWgt;
class rkPlotter;
class QPoint;

/* Realtime-data subpage */
class rkFmView : public rkFrame
{
	Q_OBJECT
	public:
		rkFmView(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *mainfrm;
		QFrame *leftfrm;
		QFrame *rightfrm;
		rkTabWgt *tabWidget;
		QStringList lbtnPicPrefix;
		QStringList rbtnPicPrefix;
		QStringList tlabel[3];
		QWidgetStack *wstack[3];
		rkTbl *tbl[3];
		rkPlotter *plotter[3];
		QVBoxLayout *vlayout;
		QPushButton *lbtn[2];
		QPushButton *rbtn[3];
		QValueVector<QPoint> pointvector;
		int timer1;
		int timer2;
		bool isTableShown;
	protected :
		void timerEvent(QTimerEvent *);
		void updateAiTbl();
		void updateAiTbl(QString &);
		void updateEiTbl();
		void updateDioTbl();
	public slots:
	private slots:
		void procLBtnClick();
		void procRBtnClick();
		void btnChangePic(int);
};

#endif /* _RK_FORM_DATA_VIEW_H_ */
