#ifndef _RK_SET_PARAM_SWITCH_H_
#define _RK_SET_PARAM_SWITCH_H_

#include "rkfrm.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class QLabel;
class QComboBox;
class rkCkBox;

/* Switch Parameter Setting */
class rkSetDio : public rkFrame
{
	Q_OBJECT
	public:
		rkSetDio(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *mainfrm;
		QFrame *rfrm;
		QGridLayout *glayout;
		QLabel *label;
		QLabel *codelabel[8];
		QPushButton *btn;
		rkCkBox *ckbox;
		QComboBox *combox;
		QHBoxLayout *hlayout;
		rkLnEdit *focusedLineEdit;
	protected:
		QString xmlRootName;
		QString xmlGroupPrefix; /* CHI || CHO */
		int itemCount;

	protected slots:
		void showEvent(QShowEvent *);
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void changeChannel(int);
		void saveParamValue();
signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_PARAM_SWITCH_H_ */
