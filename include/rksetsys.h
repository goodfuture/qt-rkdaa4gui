/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rksetsys.h
 * Created At       : 2013-11-12 09:02
 * Last Modified    : 2013-11-12 15:30
 * Description      : 
 * History	        : 
*******************************************************************************/
#ifndef _RK_SET_PARAM_SYS_H_
#define _RK_SET_PARAM_SYS_H_

#include "rkfrm.h"
#include "qmap.h"

class QLabel;
class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class QPushButton;
class QDateTimeEdit;
class QComboBox;
class rkCkBox;
class rkDlgSel;
class QValidator;

/* System Parameter Setting */
class rkSetSys : public rkFrame
{
	Q_OBJECT
	public:
		rkSetSys(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *mainfrm;
		QFrame *rightfrm;
		QStringList labeltxt;
		QStringList codelist;
		QGridLayout *glayout;
		rkLnEdit *focusedLineEdit;
		QLabel *label;
		rkDlgSel *seldlg;
		rkLnEdit *lnedit[6];
		QComboBox *combox[3];
		rkCkBox *checkBox[6];
		QHBoxLayout *hlayout;
		QPushButton *btn;
		QDateTimeEdit *timeEdit;
		QValidator *validator;
	protected:
		QString xmlRootName;
		void saveParamValue();

	protected slots:
		void showEvent(QShowEvent *);
		void setLocalTime();
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void recvItems(QString);
signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_PARAM_SYS_H_ */
