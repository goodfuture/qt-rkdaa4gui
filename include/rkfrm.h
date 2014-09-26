#ifndef _RK_FRAMEWORK_H_
#define _RK_FRAMEWORK_H_

#include <qwidget.h>
#include <qtimer.h>
#include <qdialog.h>
#include "rkdlgkb.h"

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QFrame;

//class rkFrame : public QWidget
class rkFrame : public QDialog
{
	Q_OBJECT
		Q_ENUMS(IconType)
	public:
		rkFrame(QWidget *parent = 0, const char * name = 0);
		~rkFrame(){};
		enum IconType {
			IconNone		= 0x00,
			IconWarning		= 0x01,
			IconKeyboard	= 0x02,
			IconUsb			= 0x03,
			IconDropped		= 0x04,
		};
		enum Position {
			TopLeft = 0x00,
			TopRight = 0x01,
			BottomLeft = 0x02,
			BottomRight = 0x03,
		};
		void setTitle(const QString &title);
	private:
		int frameWidth;
		int frameHeight;
		QLabel *titleLabel;
		QFrame *titleFrame;
		QVBoxLayout *titleLayout;
		QFrame *topLine;
		QFrame *leftFrame;
		QFrame *mainFrame;
		QFrame *rightFrame;
		QFrame *bottomFrame;
		QFrame *bottomLine;
		QVBoxLayout *bottomLayout;
		QHBoxLayout *bottomChildLayout;
		QLabel *dateLabel;
		QLabel *venderLabel;
		QLabel *timeLabel;
		int timer1;
		int timer2;
		//int keyTimer;
		//bool keyRepeat;
		IconType iconStat[4];
		QPoint point1; /* Used for Keyboard position */
		QPoint point2;
		QPoint point3;
		QPoint point4;
	protected:
		rkDlgKb *keyboard;
		void updateTime(void);
		void screenOn(void);
		void screenOff(void);
		virtual void timerEvent(QTimerEvent *);
		QFrame *createLeftFrame(QWidget *parent);
		QFrame *createMainframe(QWidget *parent);
		QFrame *createRightFrame(QWidget *parent);
		//void keyPressEvent(QKeyEvent *);
		//void keyReleaseEvent(QKeyEvent *);
		void mouseMoveEvent(QMouseEvent *e);
	public slots:
	protected slots:
		virtual void recvKeyCode(const QString &);
		void showKeyboard(int);
		void hideKeyboard();
		void showIcon(IconType type);
		void hideIcon(IconType type);
};

#endif /* _RK_FRAMEWORK_H_ */
