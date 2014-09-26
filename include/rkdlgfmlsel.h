#ifndef _RK_FORMULA_SELECT_DIALOG_H_
#define _RK_FORMULA_SELECT_DIALOG_H_ 

#include "rkdlg.h"

class QTable;
class QStringList;
class rkDlgFml;

class rkDlgFmlSel : public rkDlg
{
	Q_OBJECT
	public:
		rkDlgFmlSel(QWidget *parent = 0, const char *name = 0);
	private:
		QFrame *formulaFrame;
		QFrame *buttonFrame;
		QTable *table;
		int currentRow;
		int oldRow;
		rkDlgFml *editDialog;
	public slots:
		void recvKeyCode(const QString &);
		void setColumnLabels(const QStringList &);
	protected slots:
		void addNewFormula(const QString &, const QString &);
		void remove();
		void processButtonClicked();
		void selectChanged(int, int);
	signals:
		void showKeyboard();
		void hideKeyboard();
		void sendKeyCode(const QString &);
		void formula(const QString &);
};

#endif /* _RK_FORMULA_SELECT_DIALOG_H_ */
