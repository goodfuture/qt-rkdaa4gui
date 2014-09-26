#ifndef _RK_PASSWD_VERIFY_H_
#define _RK_PASSWD_VERIFY_H_ 

#include <qdialog.h>

class rkLnEdit;
class QPushButton;
class QGridLayout;
class rkFrame;
class QString;
class QLabel;

class rkFmPw : public QDialog
{
	Q_OBJECT
	public:
		rkFmPw(QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags f = Qt::WStyle_Customize | WStyle_NoBorder);
	private:
		QGridLayout *frameLayout;
		rkLnEdit *lineEdit;
		QLabel * promptLable;
		QPushButton *button0; /* Ok */
		QPushButton *button1; /* Cancel */
		int lastKey;
		int combKeyCounter;
	signals:
		void hideKeyboard();
		void dispKeyboard(int);
		void passwordOk();
	protected slots:
		void processBtnClicked(int index);
		void processBtnClicked();
		void processLineEditClicked();
		void recvKeyCode(const QString &);
		virtual void showEvent(QShowEvent *);
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
		void hideEvent(QHideEvent *);
		void closeEvent(QCloseEvent *);
};

#endif /* _RK_PASSWD_VERIFY_H_ */
