/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfmexp.h
 * Created At       : 2013-11-18 10:59
 * Last Modified    : 2013-11-18 10:59
 * Description      : 
 * History	        : 
*******************************************************************************/
#ifndef _RK_FORM_DATA_EXPORT_H_
#define _RK_FORM_DATA_EXPORT_H_ 

#include "rkfrm.h"
#include <qfont.h>
#include <qwidget.h>
#include <sqlite3.h>
#include <qevent.h>

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QComboBox;
class QLabel;
class QDateTimeEdit;
class QPushButton;

/* export data subpage */
class rkFmExp : public rkFrame
{
	Q_OBJECT
	public:
		rkFmExp(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *btmfrm;
		QGridLayout *glayout;
		QHBoxLayout *layout;
		QLabel *label;
		QComboBox *combox[2];
		QDateTimeEdit *stm;
		QDateTimeEdit *etm;
		QPushButton *btn;
	private slots:
		void procBtnPressed();
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
};

#endif /* _RK_FORM_DATA_EXPORT_H_ */
