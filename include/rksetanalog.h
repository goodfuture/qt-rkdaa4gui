/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rksetanalog.h
 * Created At       : 2013-10-17 16:16
 * Last Modified    : 2013-11-12 11:01
 * Description      : 
 * History	        : 
*******************************************************************************/
#ifndef _RK_SET_PARAM_ANALOG_H_
#define _RK_SET_PARAM_ANALOG_H_

#include "rkfrm.h"
#include "rktype.h"
#include "qmap.h"
#include "qcombobox.h"
#include "rklnedit.h"
#include "rkckbox.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class rkDlgFmlSel;
class rkDlgSel;
class QPushButton;
class QToolButton;

/* Analog Parameter Setting */
class rkSetAnalog : public rkFrame
{
	Q_OBJECT
	public:
		rkSetAnalog(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *mainfrm;
		QLabel *label;
		int row;
		int col;
		QGridLayout *glayout;
		QHBoxLayout *hlayout;
		rkDlgSel *seldlg;
		QPushButton *btn;
		QToolButton *tbtn;
		rkLnEdit *focusedLineEdit;
		rkDlgFmlSel *fmlDlg;
		QComboBox *combox[2];
		rkLnEdit *lnedit[6];
		rkCkBox *ckbox[3];
		QString xmlRootName;
		QStringList pcode;
		QStringList ltext;
		int curChl;
	protected slots:
		void showEvent(QShowEvent *);
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void recvItems(QString);
		void recvFml(const QString &);
		void saveParamValue();
		void changeChl(int);
	signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_PARAM_ANALOG_H_ */
