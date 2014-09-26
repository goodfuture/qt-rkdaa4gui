/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmqrymsg.h
 * Created At       : 2013-11-18 10:43
 * Last Modified    : 2013-11-20 15:17
 * Description      : 
 * History	        : 
*******************************************************************************/
#ifndef _RK_FORM_MSG_QUERY_H_
#define _RK_FORM_MSG_QUERY_H_ 

#include "rkfrm.h"
#include <qwidget.h>
#include <qfont.h>
#include <sqlite3.h>
#include <qevent.h>

class labelLayout;
class QFrame;
class QLabel;
class QComboBox;
class QGridLayout;
class QWidgetStack;
class QDateTimeEdit;
class rkTbl;
class QPushButton;
class rkMsgView;

/* message query subpage */
class rkFmQryMsg : public rkFrame
{
	Q_OBJECT
	public:
		rkFmQryMsg(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *mainfrm;
		QFrame *leftfrm;
		QFrame *rightfrm;
		QGridLayout *glayout;
		QLabel *label;
		QComboBox *combox;
		QDateTimeEdit *tmedit[2];
		QPushButton *btn;
		sqlite3 *db;
		char **qryRst;
		rkMsgView *view;
	private slots:
		void clickProc();
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
	signals:
		void sendQryCond(int tbl_index, unsigned long start_time, unsigned long end_time);
};

#endif /* _RK_FORM_MSG_QUERY_H_ */
