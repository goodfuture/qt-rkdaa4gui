#ifndef _RK_HISTOGRAM_H_
#define _RK_HISTOGRAM_H_

#include <qwidget.h>
#include <qvaluevector.h>

class rkHistoItem 
{
	public:
		QString name;
		double value;
		QColor color;
		QRect rect;
};

class rkHisto : public QWidget
{
	Q_OBJECT
	public:
		rkHisto(QWidget *parent = 0, const char *name = 0);
		void addItem(const QString &name, double value, const QColor &color);
		void setMaxValue(int maxValue);
		void setItemValue(unsigned int index, double value);
		void setItemName(unsigned int index, const QString &name);

	private:
		enum rkHistoOpt
		{
			pillarIndent = 10,
			xAxisOffset = 30,
			yAxisOffset = 30,
			textRectHeight = 28,
		};
		int blankWidth;
		double maxValue;
		QValueVector<rkHistoItem> items;

	protected:
		virtual void paintEvent( QPaintEvent *);
		void drawPillars();
		void drawText();
		void drawScale();

	public slots:

	signals:
};

#endif /* _RK_HISTOGRAM_H_ */
