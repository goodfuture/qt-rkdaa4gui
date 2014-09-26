#ifndef _RK_FM_AI_CALIB_H_
#define _RK_FM_AI_CALIB_H_

#include "rkcom.h"
#include "rkfrm.h"
#include <qevent.h>
#include <qwidget.h>

class QFrame;
class QGridLayout;
class QLabel;
class QHBoxLayout;
class QPushButton;
class QWidgetStack;
class rkCkBox;

/* About Subpage */
class rkFmAiCalib : public rkFrame
{
	Q_OBJECT
	public:
		rkFmAiCalib(QWidget *parent = 0, const char *name = 0);
	private:
		int id;
		int timer1;
		int timer2;
		int timer3;
		int cnt1;
		int cnt2;
		int calibChl;
		QString prompt;
		QFrame *mainfrm;
		QGridLayout *glayout;
		QHBoxLayout *hlayout;
		rkCkBox *ckbox[AI_NUM];
		rkCkBox *checkBox;
		QWidget *wgt;
		QWidgetStack *wgtstack;
		QLabel *label;
		QLabel *pmtlabel[8];
		QPushButton *btn;
	protected:
		void timerEvent(QTimerEvent *);
		void showEvent(QShowEvent *);
		void hideEvent(QHideEvent *);
	private slots:
		void procBtnPressed();
		void procCkboxSel(bool);
signals:
		void sendUICmd(int);
};

#endif /* _RK_FM_CALIB_H_ */
