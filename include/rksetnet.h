#ifndef _RK_SET_PARAM_NET_H_
#define _RK_SET_PARAM_NET_H_

#include "rkfrm.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class rkCkBox;
class rkXmlOper;
class QComboBox;
class QValidator;

/* Network Parameter Setting */
class rkSetNet : public rkFrame
{
	Q_OBJECT
	public:
		rkSetNet(QWidget *parent = 0, const char *name = 0);
	private:
		QStringList labelText;
		QStringList paramCode;
		QFrame *mainframe;
		QGridLayout *frameLayout;
		rkLnEdit *focusedLineEdit;
		rkCkBox *checkBox;
		rkLnEdit* lineEdit[7];
		QComboBox *combox;
		QValidator *validator;
	protected:
		QString xmlRootName;
		QStringList ifaceArg;
		void saveParamValue();
	protected slots:
		void showEvent(QShowEvent *);
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void processToggled(bool);
signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_PARAM_NET_H_ */
