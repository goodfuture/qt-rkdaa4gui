#ifndef _RK_CHECK_BOX_H_
#define _RK_CHECK_BOX_H_ 

#include <qwidget.h>
#include <qpixmap.h>

class rkCkBox : public QWidget
{
	Q_OBJECT
	public:
		explicit rkCkBox(QWidget *parent = 0, const char *name = 0);
		void initialize();
	private:
		QPixmap pixmapOn;
		QPixmap pixmapOff;
		bool mouseDown;
		bool checkState; /* True -- Check, False -- Uncheck */
	protected:
		virtual void paintEvent(QPaintEvent *);
		virtual void mousePressEvent(QMouseEvent *);
		virtual void mouseReleaseEvent(QMouseEvent *);
	public slots:
		void setChecked(bool);
		bool isChecked();
	signals:
		void checked(bool);
};

#endif /* _RK_CHECK_BOX_H_ */
