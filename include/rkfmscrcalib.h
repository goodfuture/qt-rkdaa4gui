#ifndef _RK_FM_SCR_CALIBRATE_H_
#define _RK_FM_SCR_CALIBRATE_H_ 

#include <qwidget.h>
#include <qpointarray.h>
#include <qmap.h>
#include "tslib.h"

#define MAX_SAMPLES 128
#define NR_STEPS 10

class QPoint;
class QPainter;

typedef struct {
	int x[5], xfb[5];
	int y[5], yfb[5];
	int a[7];
} calibration;

class rkFmScrCalib : public QWidget {
	Q_OBJECT
	public:
		rkFmScrCalib(QWidget *parent = 0, const char * name = 0);
	
	private:
		QPainter *painter;
		struct tsdev *ts;
		QMap<int, QString> posmap;
		calibration cal;
		QPointArray pt;
		void drawCrosshair(int);
		bool getSample(int);
		bool getPosition(int *, int *);
		int splitTimer;
		int rebootTimer;
		bool splitTimerFlag;
		bool rebootTimerFlag;
		int paintCounter;
	public slots:
		bool calibrateInit();
		bool calibratePerform();
	protected slots:
		virtual void paintEvent(QPaintEvent *);
		virtual void showEvent(QShowEvent *);
		void timerEvent(QTimerEvent *);
};

#endif /* _RK_SCR_CALIBRATE_H_ */
