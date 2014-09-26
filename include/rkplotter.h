#ifndef _RK_PLOTTER_H_
#define _RK_PLOTTER_H_

#include <qpixmap.h>
#include <qwidget.h>
#include <qvaluevector.h>

class QString;

class PlotSettings
{
	public:
		PlotSettings();

		void scroll(int dx, int dy);
		double spanX() const { return maxX - minX; }
		double spanY() const { return maxY - minY; }

		double minX;
		double maxX;
		int numXTicks;
		double minY;
		double maxY;
		int numYTicks;
		void adjust();

	private:
		static void adjustAxis(double &min, double &max, int &numTicks);
};

class rkPlotter : public QWidget
{
	public:
		rkPlotter(QWidget *parent = 0, const char *name = 0);

		void setPlotSettings(const PlotSettings &settings);
		void setCurveData(int id, const char *name, const QValueVector<QPoint> &data);
		void clearCurve(int id);
		void refreshPixmap();

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);
		QSize sizeHint() const;

	private:
		void drawGrid(QPainter *painter);
		void drawCurves(QPainter *painter);

		enum { AxisXOffset= 10, AxisYOffset = 30, TextZoneHeight = 70};

		QMap<int, QString> nameMap;
		QMap<int, QValueVector<QPoint> > curveMap;
		QValueVector<PlotSettings> zoomStack;
		int curZoom;
		QPixmap pixmap;
};

#endif /* _RK_PLOTTER_H_ */
