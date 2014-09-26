#ifndef _RK_SET_VALUE_SWITCH_H_
#define _RK_SET_VALUE_SWITCH_H_

#include "rkfrm.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class rkBtnSw;

/* Switch Control Setting */
class rkSetDo : public rkFrame
{
	Q_OBJECT
	public:
		rkSetDo(QWidget *parent = 0, const char *name = 0);
	private:
		QStringList labelText;
		QFrame *mainframe;
		QFrame *rightframe;
		QGridLayout *frameLayout;
		rkLnEdit *focusedLineEdit;
		int itemCount;
		rkBtnSw *switchBtn[8];
	protected:
		QString xmlRootName;
		QString xmlGroupPrefix;
	protected slots:
		void showEvent(QShowEvent *);
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void switchChanged(bool);
		void saveParamValue();
signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_VALUE_SWITCH_H_ */
