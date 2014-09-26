#ifndef _RK_BASE_DIALOG_H_
#define _RK_BASE_DIALOG_H_

#include <qdialog.h>

class QLabel;
class QFrame;
class QVBoxLayout;

class rkDlg : public QDialog
{
	Q_OBJECT

	public:
		rkDlg(QWidget *parent = 0, const char *name = 0, int frameWidth = 400, int frameHeight = 250);

	private:
		int frameWidth;
		int frameHeight;
		QFrame *headFrame;
		QFrame *middleFrame;
		QFrame *buttonFrame;
		QVBoxLayout *frameLayout;
		QLabel *titleLabel;

	public slots:
		void setTitle(const QString &);

	protected slots:
		void setFrameSize(int width, int height);
		QFrame *createMiddleFrame(QWidget *parent);
		QFrame *createButtonFrame(QWidget *parent);
		void moveOffset(int x, int y);
};

#endif /* _RK_BASE_DIALOG_H_ */
