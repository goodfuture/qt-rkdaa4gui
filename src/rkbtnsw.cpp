#include "rktype.h"
#include "rkbtnsw.h"
#include <qpainter.h>
#include <qevent.h>
#include <qfont.h>

rkBtnSw::rkBtnSw(QWidget *parent, const char *name)
	: QWidget(parent, name)
{
	setWFlags(Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	pixmapBackground.load(QString(IMG_DIR) + "/sw_bk_arm.png");
	pixmapForeground.load(QString(IMG_DIR) + "/sw_fr_arm.png");
	setFixedSize(pixmapBackground.width(), pixmapBackground.height());
	buttonOn = false;
	mouseDown = false;
}

void rkBtnSw::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(QPoint(0,0), pixmapBackground); /* Background */
	QPoint ptFore;

	/* Set Font */
	QFont font = painter.font();
	font.setPointSize(12);
	painter.setFont(font);

	if (buttonOn) {
		ptFore = rect().topLeft();
	} else {
		ptFore = QPoint(this->width() - pixmapForeground.width(),this->rect().top());
	}
	QRect rcOn;
	rcOn.setTopLeft(rect().topLeft());
	rcOn.setBottomRight(QPoint(pixmapForeground.width(), pixmapForeground.height()));
	painter.drawText(rcOn, Qt::AlignCenter, "OFF"); /* Text "OFF" */

	QRect rcOff;
	rcOff.setTopLeft(QPoint(rect().width() - pixmapForeground.width(), rect().top()));
	rcOff.setBottomRight(rect().bottomRight());
	painter.drawText(rcOff, Qt::AlignCenter, "ON"); /* Text "ON" */

	painter.drawPixmap(ptFore, pixmapForeground); /* Foreground */
}

void rkBtnSw::mousePressEvent(QMouseEvent *)
{
	mouseDown = true;
}

void rkBtnSw::mouseReleaseEvent(QMouseEvent *)
{
	if (mouseDown) {
		buttonOn = !buttonOn;
		mouseDown = false;
		emit switched(buttonOn);
	}
	update();
}

void rkBtnSw::initialize(const QString& strImageBk, const QString& strImageFore)
{
	pixmapBackground.load(strImageBk);
	pixmapForeground.load(strImageFore);
	setFixedSize(pixmapBackground.width(), pixmapBackground.height());
}

void rkBtnSw::setSwitch(bool state)
{
	buttonOn = state;
	update();
}

bool rkBtnSw::isOn()
{
	return buttonOn;
}
