#ifndef _RK_MAINFORM_H_
#define _RK_MAINFORM_H_ 

#include "rkfrm.h"

class rkMsgView;
class rkFmQryRtd;
class rkFmSet;
class rkFmView;
class rkFmExp;
class rkFmInfo;
class rkFmPw;
class QPushButton;
class rkFmScrCalib;

class rkFmPri : public rkFrame 
{
	Q_OBJECT
	public:
		rkFmPri(QWidget *parent = 0, const char *name = 0);
		~rkFmPri(){};
	private:
		QPushButton *btn[6];
		rkFmSet *paramSetForm;
		rkFmView *dataViewForm;
		rkFmExp *dataExportForm;
		rkMsgView *msgQueryForm;
		rkFmQryRtd *dataQueryForm;
		rkFmInfo *aboutForm;
		rkFmPw *loginform;
		//rkFmScrCalib *scrcalib;
		int curpos;
		bool keyRepeat;
		int keyTimer;
		int combkeycnt;
		int lastkey;
		int focusId;
	private slots:
		void recvKeyCode(const QString &);
		void showSubform();
		void showSubform(int);
		void showSettingForm(); /* Display Parameter Setting Form */
	protected slots:
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
		void timerEvent(QTimerEvent *);
		bool focusNextPrevChild(bool);
	signals:
		void sendKeyCode(const QString &);
};

#endif /* _RK_MAIN_FORM_H_ */
