#ifndef _RK_SET_PARAM_UART_H_
#define _RK_SET_PARAM_UART_H_

#include "rkfrm.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class QComboBox;

/* Uart Parameter Setting */
class rkSetCom : public rkFrame
{
	Q_OBJECT

	public:
		rkSetCom(QWidget *parent = 0, const char *name = 0);

	private:
		QComboBox *combox[6];
		QFrame *mainfrm;
		QGridLayout *glayout;
		rkLnEdit *focusedLineEdit;
		QStringList plib;

	protected:
		QString xmlRootName;
		QStringList labelText;
		QStringList pcode;
		void savePvalue();

	protected slots:
		void showEvent(QShowEvent *);
		void popupKeyboard();
		void procBtnPressed();
		void changeChl(int);
		int scanPlib(const QString &path);

	signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_PARAM_UART_H_ */
