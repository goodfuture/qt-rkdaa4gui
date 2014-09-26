#ifndef _RK_SWITCH_BUTTON_H_
#define _RK_SWITCH_BUTTON_H_

#include <qwidget.h>
#include <qpixmap.h>

class rkBtnSw : public QWidget
{
	Q_OBJECT
	public:
		explicit rkBtnSw(QWidget *parent = 0, const char *name = 0);
		void initialize(const QString&, const QString&);
	private:
		QPixmap     pixmapBackground;
		QPixmap     pixmapForeground;
		bool        buttonOn;
		bool        mouseDown;
	protected:
		virtual void paintEvent(QPaintEvent *);
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
	signals:
		void switched(bool);
	public slots:
		void setSwitch(bool);
		bool isOn();
};

#endif /* _RK_SWITCH_BUTTON_H_ */
