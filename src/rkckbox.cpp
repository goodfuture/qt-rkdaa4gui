#include "rktype.h"
#include "rkckbox.h"
#include <qpainter.h>
#include <qevent.h>

rkCkBox::rkCkBox(QWidget *parent, const char *name)
	:QWidget(parent, name)
{
	setWFlags(Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	pixmapOn.load(QString(IMG_DIR) + "/ckbox_on_arm.png");
	pixmapOff.load(QString(IMG_DIR) + "/ckbox_off_arm.png");
	setFixedSize(pixmapOn.width(), pixmapOn.height());
	checkState = false;
	mouseDown = false;
}

void rkCkBox::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	if (checkState) {
		painter.drawPixmap(QPoint(0, 0), pixmapOn);
	} else {
		painter.drawPixmap(QPoint(0, 0), pixmapOff);
	}
}

void rkCkBox::mousePressEvent(QMouseEvent *)
{
	mouseDown = true;
}

void rkCkBox::mouseReleaseEvent(QMouseEvent *)
{
	if (mouseDown) {
		checkState = !checkState;
		mouseDown = false;
		emit checked(checkState);
	}

	update();
}

void rkCkBox::setChecked(bool val)
{
	checkState = val;
	update();
}

bool rkCkBox::isChecked()
{
	return checkState;
}
