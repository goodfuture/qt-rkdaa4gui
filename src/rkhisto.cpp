#include "rkhisto.h"
#include <qpainter.h>
#include <qpoint.h>
#include <qsize.h>
#include <qfont.h>

rkHisto::rkHisto(QWidget *parent, const char *name)
	:QWidget(parent, name)
{
	items.clear();
	//this->setFixedSize(640, 310);
	QFont font = this->font();
	font.setPointSize(14);
	this->setFont(font);
}

void rkHisto::addItem(const QString &name, double value, const QColor &color)
{
	rkHistoItem item;
	item.name = name;
	item.value = value;
	item.color = color;
	item.rect = QRect();
	items.push_back(item);
	if (items.size() > 5) {
		blankWidth = 10;
	} else {
		blankWidth = 20;
	}
}

void rkHisto::setMaxValue(int maxValue)
{
	this->maxValue = (double)maxValue;
}

void rkHisto::setItemValue(unsigned int index, double value)
{
	if (items.size() <= index) {
		return;
	}

	items[index].value = value;
}

void rkHisto::setItemName(unsigned int index, const QString &name)
{
	if (items.size() <= index) {
		return;
	}
	
	items[index].name = name;
}

void rkHisto::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	/* Draw Axises */
	painter.drawLine(0, height() - yAxisOffset, width(), height() - yAxisOffset); /* X Axis */
	painter.drawLine(xAxisOffset, 0, xAxisOffset, height()); /* Y Axis */

	/* Draw Pillars */
	int pillarWidth = ( width() - yAxisOffset - 2 * pillarIndent - blankWidth * ( items.size() - 1)) / items.size();
	double heightFactor = (double)(height() - xAxisOffset - textRectHeight) / maxValue;

	for (unsigned int i = 0; i < items.size(); i++) {
		int pillarHeight =  (int)(items[i].value * heightFactor);

		int topLeftX = pillarIndent + yAxisOffset + i * (pillarWidth + blankWidth);
		int topLeftY = height() - xAxisOffset - pillarHeight;

		QRect& rect = items[i].rect;
		rect.setTopLeft(QPoint(topLeftX, topLeftY));
		rect.setSize(QSize(pillarWidth, pillarHeight));
		painter.setPen(QColor(0, 0, 0));
		painter.setBrush(items[i].color);
		painter.drawRect(rect);
	}

	/* Draw Value Text */
	painter.setPen(QColor(255, 255, 255));
	for (unsigned int i = 0; i < items.size(); i++) {
		QRect rect = QRect(items[i].rect.left() - blankWidth / 2, items[i].rect.top() - textRectHeight, items[i].rect.width() + blankWidth, textRectHeight);
		const QString & text = QString::number(items[i].value);
		painter.drawText(rect, Qt::AlignCenter, text);
	}

	/* Draw Label Text */
	for (unsigned int i = 0; i < items.size(); i++) {
		QRect rect = QRect(items[i].rect.left() - blankWidth / 2, height() - xAxisOffset, items[i].rect.width() + blankWidth, xAxisOffset);
		const QString & text = items[i].name;
		painter.drawText(rect, Qt::AlignCenter, text);
	}

	/* Draw Scale */
	int interval = (height() - xAxisOffset - textRectHeight) / 5;
	for (int i = 0; i <= 5; i++) {
		int topleft_x = 0;
		int topleft_y = height() - xAxisOffset - (interval * i);
		painter.drawLine(yAxisOffset - 5, topleft_y, yAxisOffset, topleft_y);
		QRect rect = QRect(topleft_x, topleft_y - textRectHeight, yAxisOffset, textRectHeight);
		QString text = QString::number(maxValue / 5 * i);
		painter.drawText(rect, Qt::AlignCenter, text);
	}
}
