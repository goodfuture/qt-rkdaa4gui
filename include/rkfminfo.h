#ifndef _RK_FORM_ABOUT_H_
#define _RK_FORM_ABOUT_H_ 

#include "rkfrm.h"
#include <qevent.h>

class QFrame;
class QGridLayout;
class QLabel;
class QHBoxLayout;

/* About Subpage */
class rkFmInfo : public rkFrame 
{
	Q_OBJECT
	public:
		rkFmInfo(QWidget *parent = 0, const char *name = 0);
	private:
		int timer1;
		QFrame *mainfrm;
		QGridLayout *frmlayout;
		QLabel *label;
		QLabel *vlabel[2];
	protected:
		void timerEvent(QTimerEvent *);
	private slots:
		void setCoreStatus(bool run, const char *version);
		void procBtnPressed();
};

#endif /* _RK_FORM_ABOUT_H_ */
