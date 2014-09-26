#ifndef _RK_FORMULA_EDIT_DIALOG_H_
#define _RK_FORMULA_EDIT_DIALOG_H_

#include "rkdlg.h"

class QFrame;
class rkLnEdit;

class rkDlgFml : public rkDlg
{
	Q_OBJECT
	public:
		rkDlgFml(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *editFrame;
		QFrame *buttonFrame;
		rkLnEdit *lineEdit0;
		rkLnEdit *lineEdit1;
		rkLnEdit *focusedLineEdit;
	public slots:
		void recvKeyCode(const QString &);
	private slots:
		void processBtnClicked();
		void processEditClicked();
	signals:
		void showKeyboard();
		void closed();
		void newFormula(const QString &, const QString &);
};

#endif /* _RK_FORMULA_EDIT_DIALOG_H_ */
