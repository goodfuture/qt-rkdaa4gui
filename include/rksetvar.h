#ifndef _RK_SET_VALUE_UART_H_
#define _RK_SET_VALUE_UART_H_

#include "rkfrm.h"

class QFrame;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class rkLnEdit;
class rkDlgFmlSel;
class rkDlgSel;
class QSpinBox;
class rkCkBox;
class QComboBox;
class QToolButton;
class QPushButton;
class QValidator;

/* Uart Value Parameter Setting */
class rkSetVar : public rkFrame
{
	Q_OBJECT
	public:
		rkSetVar(QWidget *parent = 0, const char *name = 0);
	private:
		int row;
		int col;
		QFrame *mainfrm;
		QStringList ltext;
		QGridLayout *glayout;
		QHBoxLayout *hlayout;
		QValidator *validator;
		QLabel *label;
		rkDlgSel *seldlg;
		QSpinBox *spinbox;
		rkCkBox *ckbox[3];
		rkLnEdit *lnedit[4];
		QComboBox *combox[2];
		rkLnEdit *focusedLnEdit;
		rkDlgFmlSel *fmlDlg;
		QPushButton *btn;
		QToolButton *tbtn;
	protected:
		QString xmlRootName;
		QStringList labelText;
		QStringList pcode;

	protected slots:
		void showEvent(QShowEvent *);
		void recvItems(QString);
		void popupKeyboard();
		void procBtnPressed();
		void recvKeyCode(const QString &);
		void recvFml(const QString &);
		void saveParamValue();
		void changeChl(int);
signals:
		void sendUICmd(int cmd);
};

#endif /* _RK_SET_VALUE_UART_H_ */
