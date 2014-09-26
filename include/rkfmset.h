#ifndef _RK_FORM_PARAM_SET_H_
#define _RK_FORM_PARAM_SET_H_ 

#include "rkfrm.h"

class rkSetSys;  
class rkSetCom; 
class rkSetNet;  
class rkSetAnalog;
class rkSetVar; 
class rkSetDio;
class rkSetDo;
class rkFmCalib;
class QPushButton;

/* Parameter Setting subpage */
class rkFmSet : public rkFrame
{
	Q_OBJECT
	public:
		rkFmSet(QWidget *parent = 0, const char *name = 0);
	private:
		QPushButton *iconBtn[10];
		bool scrLock; /* Screen Lock */
		bool enDialog;
		rkSetSys *settingForm1;
		rkSetCom *settingForm2;
		rkSetNet *settingForm3;
		rkSetAnalog *settingForm4;
		rkSetVar *settingForm5;
		rkSetDio *settingForm6;
		rkSetDo *settingForm7;
		rkFmCalib *calibrateForm;
		private slots:
			void clickProc();
		void childProcessExited();
		protected slots:
			bool eventFilter(QObject *obj, QEvent *event);
		public slots:
			void forwardUICmd(int cmd);
		signals:
			void sendUICmd(int);
};

#endif /* _RK_FORM_PARAM_SET_H_ */
