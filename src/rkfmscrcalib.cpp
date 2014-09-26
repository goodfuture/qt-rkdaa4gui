#include "rkfmscrcalib.h"
#include "rkclr.h"
#include <qapplication.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpen.h>
#include <qbrush.h>
#include <qmessagebox.h>
#include <fcntl.h>
#include <stdlib.h>

int sort_by_x(const void* a, const void *b)
{
	return (((struct ts_sample *)a)->x - ((struct ts_sample *)b)->x);
}

int sort_by_y(const void* a, const void *b)
{
	return (((struct ts_sample *)a)->y - ((struct ts_sample *)b)->y);
}

rkFmScrCalib::rkFmScrCalib(QWidget *parent, const char *name)
	: QWidget(parent, name, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{
	splitTimerFlag = false;
	rebootTimerFlag = false;

	setFixedSize(800, 480);
	setPaletteBackgroundColor(rkClr(25, 31, 46));
	setPaletteForegroundColor(rkClr(255, 255, 255));

	pt.resize(5);
	posmap[0] = QString("左上角");
	pt[0] = QPoint(50, 50);
	posmap[1] = QString("右上角");
	pt[1] = QPoint(750, 50);
	posmap[2] = QString("右下角");
	pt[2] = QPoint(750, 430);
	posmap[3] = QString("左下角");
	pt[3] = QPoint(50, 430);
	posmap[4] = QString("中心");
	pt[4] = QPoint(400, 240);
	//setHidden(true);

	painter = new QPainter(this);
	QFont font = painter->font();
	font.setPointSize(18);
	painter->setFont(font);
	QPen pen = painter->pen();
	pen.setWidth(3);
	painter->setPen(pen);
}

void rkFmScrCalib::drawCrosshair(int index)
{
	int x = pt[index].x(), y = pt[index].y();
	painter->eraseRect(rect());
	QRect rcText(310, 160, 180, 60);
	painter->drawText(rcText, Qt::AlignCenter, "触摸屏校准工具\n点击十字光标进行校准");

	rcText.moveTop(280);
	painter->drawText(rcText, Qt::AlignCenter, posmap[index] + "校准中");

	painter->drawPoint(pt[index]);
	painter->drawLine(x - 13, y - 3, x - 3, y - 3);
	painter->drawLine(x + 3, y - 3, x + 13, y - 3);
	painter->drawLine(x - 13, y + 3, x - 3, y + 3);
	painter->drawLine(x + 3, y + 3, x + 13, y + 3);
	painter->drawLine(x - 3, y - 13, x - 3, y - 3);
	painter->drawLine(x - 3, y + 3, x - 3, y + 13);
	painter->drawLine(x + 3, y - 13, x + 3, y - 3);
	painter->drawLine(x + 3, y + 3, x + 3, y + 13);
}

bool rkFmScrCalib::calibrateInit()
{
#ifdef __ARM__
	char *tsdevice = NULL;

	if((tsdevice = getenv("TSLIB_TSDEVICE")) != NULL ) { 
		ts = ts_open(tsdevice,0);
	} else {
		return false;
	}   

	if (!ts || ts_config(ts)) {
		ts_close(ts);
		return false;
	}   
#endif /* __ARM__ */

	return true;
}

bool rkFmScrCalib::calibratePerform()
{
	float n, x, y, x2, y2, xy, z, zx, zy; 
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

	/* Get sums for matrix */
	n = x = y = x2 = y2 = xy = 0;
	for(int j = 0;j < 5; j++) {
		n += 1.0;
		x += (float)cal.x[j];
		y += (float)cal.y[j];
		x2 += (float)(cal.x[j] * cal.x[j]);
		y2 += (float)(cal.y[j] * cal.y[j]);
		xy += (float)(cal.x[j] * cal.y[j]);
	}   

	/* Get determinant of matrix -- check if determinant is too small */
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
		printf("ts_calibrate: determinant is too small -- %f\n",det);
		return false;
	}   

	/* Get elements of inverse matrix */
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

	/* Get sums for x calibration */
	z = zx = zy = 0;
	for(int j = 0;j < 5; j++) {
		z += (float)cal.xfb[j];
		zx += (float)(cal.xfb[j]*cal.x[j]);
		zy += (float)(cal.xfb[j]*cal.y[j]);
	}   

	// Now multiply out to get the calibration for framebuffer x coord
	cal.a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal.a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal.a[2] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n",(a*z + b*zx + c*zy),
			(b*z + e*zx + f*zy),
			(c*z + f*zx + i*zy));

	// Get sums for y calibration
	z = zx = zy = 0;
	for(int j=0; j<5; j++) {
		z += (float)cal.yfb[j];
		zx += (float)(cal.yfb[j]*cal.x[j]);
		zy += (float)(cal.yfb[j]*cal.y[j]);
	}

	// Now multiply out to get the calibration for framebuffer y coord
	cal.a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal.a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal.a[5] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n",(a*z + b*zx + c*zy),
			(b*z + e*zx + f*zy),
			(c*z + f*zx + i*zy));

	// If we got here, we're OK, so assign scaling to a[6] and return
	cal.a[6] = (int)scaling;
	return true;
}

bool rkFmScrCalib::getPosition(int *x, int *y)
{
#ifdef __ARM__
	struct ts_sample samp[MAX_SAMPLES], action;
	int index = 0, middle;
	QRect rcText(310, 320, 180, 60);
	painter->eraseRect(rcText);

	while(splitTimerFlag == true);

	do {
		if (ts_read_raw(ts, &samp[0], 1) < 0) {
			return false;
		}
		usleep(100);
	} while (samp[0].pressure == 0);

	painter->drawText(rcText, Qt::AlignCenter, QString("采样中,请保持..."));

	do {
		//index = index < MAX_SAMPLES - 1 ? index + 1 : index;
		if (index < MAX_SAMPLES - 1) {
			index++;
		} else {
			break;
		}
		if (ts_read_raw(ts, &samp[index], 1) < 0) {
			return false;
		}
		usleep(100);
	} while (samp[index].pressure > 0);

	printf("Took %d samples...\n", index);

	painter->eraseRect(rcText);
	if (index >=  MAX_SAMPLES - 1) {
		painter->drawText(rcText, Qt::AlignCenter, QString("采样完成!\n请松开并移置下一点."));
		do {
			if (ts_read_raw(ts, &action, 1) < 0) {
				return false;
			}
			usleep(100);
		} while (action.pressure > 0);
	} else {
		painter->drawText(rcText, Qt::AlignCenter, QString("采样失败!\n请根据提示重新校准."));
		return false;
	}

	//splitTimer = startTimer(2000);

	middle = index/2;
	if (x) {
		qsort(samp, index, sizeof(struct ts_sample), sort_by_x);
		if (index & 1) /* ODD */
			*x = samp[middle].x; 
		else /* EVEN */
			*x = (samp[middle-1].x + samp[middle].x) / 2;
	}
	if (y) {
		qsort(samp, index, sizeof(struct ts_sample), sort_by_y);
		if (index & 1)
			*y = samp[middle].y;
		else
			*y = (samp[middle-1].y + samp[middle].y) / 2;
	}
#endif /* __ARM__ */

	return true;
}

bool rkFmScrCalib::getSample(int index)
{
#ifdef __ARM__
	int x = pt[index].x();
	int y = pt[index].y();

	if (getPosition(&cal.x[index], &cal.y[index]) == false) {
		return false;
	}

	cal.xfb[index] = x;
	cal.yfb[index] = y;

	return true;
	//printf("X = %4d Y = %4d\n", cal.x[index], cal.y[index]);
#endif /* __ARM__ */
}

void rkFmScrCalib::paintEvent(QPaintEvent *)
{
	printf("paintCounter = %d\n", paintCounter);
	if (++paintCounter > 1) {
		return;
	}

	if (!calibrateInit()) {
		QMessageBox::critical(0, "错误", "初始化失败!", "确定");
		close();
		//setHidden(true);
		return;
	}

	for (int i = 0; i < pt.size(); ++i) {
		printf("index = %d\n", i);
		drawCrosshair(i);
		if (getSample(i) == false) {
			i--;
		}
	}
#ifdef __ARM__
	ts_close(ts);
#endif

	if (calibratePerform()) {
#ifdef __ARM__
		char *calfile;
		FILE *cal_fp;

		printf ("Calibration constants: ");
		for (int i = 0; i < 7; i++) 
			printf("%d ", cal.a [i]);
		printf("\n");                                                                                                                 
		if ((calfile = getenv("TSLIB_CALIBFILE")) != NULL) {
			cal_fp = fopen (calfile, "w+");
		} else {
			cal_fp = fopen ("/etc/pointercal", "w+");
		}

		fprintf(cal_fp, "%d %d %d %d %d %d %d",
				cal.a[1], cal.a[2], cal.a[0],
				cal.a[4], cal.a[5], cal.a[3], cal.a[6]);
		fclose(cal_fp);

		QRect rcText(310, 320, 180, 60);
		painter->eraseRect(rcText);
		painter->drawText(rcText, Qt::AlignCenter, QString("校准完成，程序将重启!"));
		qApp->quit();
#endif /* __ARM__ */
	} else {
		QRect rcText(310, 320, 180, 60);
		painter->eraseRect(rcText);
		painter->drawText(rcText, Qt::AlignCenter, QString("校准失败！！！"));
		close();
	}
	//setHidden(true);
}

void rkFmScrCalib::showEvent(QShowEvent *e)
{
	paintCounter = 0;
	QWidget::showEvent(e);
}

void rkFmScrCalib::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == splitTimer) {
		splitTimerFlag = false;
		killTimer(splitTimer);
	} else if (e->timerId() == rebootTimer) {
		rebootTimerFlag = true;
		killTimer(rebootTimer);
	}
}
