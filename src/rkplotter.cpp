#include "rkplotter.h"
#include "rkclr.h"
#include <qpainter.h>
#include <math.h>

PlotSettings::PlotSettings()
{
	minX = 0.0;
	maxX = 60.0;
	numXTicks = 60;

	minY = 0.0;
	maxY = 25.0;
	numYTicks = 25;
}

/* Zoom */
void PlotSettings::scroll(int dx, int dy)
{
	double stepX = spanX() / numXTicks;
	minX += dx * stepX;
	maxX += dx * stepX;

	double stepY = spanY() / numYTicks;
	minY += dy * stepY;
	maxY += dy * stepY;
}

void PlotSettings::adjust()
{
	adjustAxis(minX, maxX, numXTicks);
	adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
	const int MinTicks = 4;
	double grossStep = (max - min) / MinTicks;
	double step = pow(10.0, floor(log10(grossStep)));
	if (5 * step < grossStep) {
		step *= 5;
	} else if (2 * step < grossStep) {
		step *= 2;
	}

	numTicks = int(ceil(max / step) - floor(min / step));
	if (numTicks < MinTicks) {
		numTicks = MinTicks;
	}
	min = floor(min / step) * step;
	max = ceil(max / step) * step;
}

/* Begine Class rkPlotter */
rkPlotter::rkPlotter(QWidget *parent, const char *name)
	: QWidget(parent, name)
{
	setBackgroundColor(rkClr(0, 0, 0));
	setBackgroundMode(Qt::NoBackground);
	setPlotSettings(PlotSettings());
}

void rkPlotter::setPlotSettings(const PlotSettings &settings)
{
	zoomStack.clear();
	zoomStack.append(settings);
	curZoom = 0;
	refreshPixmap();
}

void rkPlotter::setCurveData(int id, const char *name, const QValueVector<QPoint> &data)
{
	curveMap.insert(id, data);
	nameMap.insert(id, name);
	refreshPixmap();
}

void rkPlotter::clearCurve(int id)
{
	curveMap.remove(id);
	nameMap.remove(id);
	refreshPixmap();
}

QSize rkPlotter::sizeHint() const
{
	return QSize(300, 300);
}

void rkPlotter::paintEvent(QPaintEvent * )
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, pixmap);
}

void rkPlotter::resizeEvent(QResizeEvent *  )
{
	refreshPixmap();
}

void rkPlotter::refreshPixmap()
{
	pixmap = QPixmap(size());
	pixmap.fill(this, 0, 0);

	QPainter painter(&pixmap);

	drawGrid(&painter);
	drawCurves(&painter);
	update();
}

void rkPlotter::drawGrid(QPainter *painter)
{
	QRect rect(AxisYOffset, AxisXOffset, width() - AxisYOffset, height() - TextZoneHeight - AxisXOffset);

	if (!rect.isValid()) {
		return;
	}

	PlotSettings settings = zoomStack[curZoom];

	QPen quiteDark(rkClr(60, 60, 60));
	QPen light(rkClr(255, 255, 255));

	/* Vertical Grid */
	for (int i = 0; i <= settings.numXTicks; ++i) {
		int x = rect.left() + i * (rect.width() - 1) / settings.numXTicks;
		painter->setPen(quiteDark);
		painter->drawLine(x, rect.top(), x, rect.bottom());
	}

	/* Horizontal Line */
	for (int j = 0; j <= settings.numYTicks; ++j) {
		int y = rect.bottom() - j * (rect.height() - 1) / settings.numYTicks;
		double label = settings.minY + j * settings.spanY() / settings.numYTicks;

		painter->setPen(quiteDark);
		painter->drawLine(rect.left(), y, rect.right(), y);
		painter->setPen(light);
		painter->drawLine(rect.left() - 5, y, rect.left(), y);

		if( j % 2 == 0)
		{
			painter->drawText(rect.left() - AxisYOffset, y - 10, AxisYOffset - 5, 20, Qt::AlignRight | Qt::AlignVCenter, QString::number(label));
		}
	}

	painter->drawRect(rect);
}

void rkPlotter::drawCurves(QPainter *painter)
{
	static const QColor colorForIds[8] = {
		Qt::red, Qt::green, Qt::blue, Qt::cyan,
		Qt::magenta, Qt::yellow,Qt::darkMagenta,Qt::darkGreen
	};

	PlotSettings settings = zoomStack[curZoom];
	QRect rect(AxisYOffset, AxisXOffset, width() - AxisYOffset, height() - TextZoneHeight - AxisXOffset);
	QRect specRect(AxisYOffset, height() - TextZoneHeight, width() - AxisYOffset, TextZoneHeight);

	if (!rect.isValid())
		return;


	int mapcount = curveMap.count();
	for(int i = 0; i < mapcount; i++)
	{
		painter->setClipRect(rect);
		/* Draw Curve */
		QValueVector<QPoint> data = curveMap[i];
		int datacount = data.count();
		QPointArray polyline(data.count());
		for (int j = 0; j < datacount; ++j) {
			double dx = data[j].x() - settings.minX;
			double dy = data[j].y() - settings.minY;
			double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
			double y = rect.bottom() - (dy * (rect.height()-1) / settings.spanY());
			polyline[j] = QPoint((int)x, (int)y);
		}

		QPen pen;
		pen.setColor(rkClr(colorForIds[i % 8]));
		pen.setWidth(1);
		pen.setJoinStyle(Qt::RoundJoin);
		painter->setPen(pen);
		painter->drawPolyline(polyline);

		/* Draw Color Block */
		painter->setClipRect(specRect);
		int pos_x;
		int pos_y;
		if (i <= 6) {
			pos_x = 30 + i * 80;
			pos_y = height() - TextZoneHeight + 10;
		} else {
			pos_x = 30 + (i - 7) * 80;
			pos_y = height() - TextZoneHeight + 40;
		}

		painter->setPen(QColor(255, 255, 255));
		painter->setBrush(rkClr(colorForIds[i % 8]));
		painter->drawRect(pos_x, pos_y, 25, 20);

		QRect textRect(pos_x + 25, pos_y, 45, 20);
		painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, nameMap[i]);
	}
}
