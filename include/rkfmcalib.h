#ifndef _RK_FM_CALIB_H_
#define _RK_FM_CALIB_H_

#include "rkfrm.h"
#include "rktype.h"

class QPushButton;
class rkFmScrCalib;
class rkFmAiCalib;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QFrame;

class rkFmCalib : public rkFrame
{
	Q_OBJECT
	public :
		rkFmCalib(QWidget *parent = 0, const char *name = 0);

	private:
		QFrame *mainfrm;
		QGridLayout *glayout;
		QHBoxLayout *hlayout;
		QVBoxLayout *vlayout;
		QPushButton *btn;
		//rkFmScrCalib *scrcalib;
		rkFmAiCalib *aicalib;
	public slots:
		void procBtnPressed();
		void forwardUICommand(int);
signals:
		void sendUICmd(int);
};

#endif
