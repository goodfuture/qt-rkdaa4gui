/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rkfrm.cpp
 * Created At       : 2013-09-25 11:17
 * Last Modified    : 2013-11-11 12:38
 * Description      : 
 * History	        : 
*******************************************************************************/
#include "rkfrm.h"
#include "rktype.h"
#include "rkclr.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qframe.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qfont.h>
#include <unistd.h>
#include <stdlib.h>

extern struct context ctx;

rkFrame::rkFrame(QWidget *parent, const char *name)
	//:QWidget(parent, name, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
	:QDialog(parent, name, false, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{
	//keyRepeat = false;
	frameWidth = 800;
	frameHeight = 480;
	int pos_x = (QApplication::desktop()->width() - frameWidth) / 2;
	int pos_y = (QApplication::desktop()->height() - frameHeight) / 2;
	this->setFixedSize(frameWidth, frameHeight);
	this->setPaletteForegroundColor(rkClr(255, 255, 255));
	this->setPaletteBackgroundColor(rkClr(47, 63, 95));
	this->move(pos_x, pos_y);

	/* title frame */
	titleFrame = new QFrame(this, "titleFrame");
	titleFrame->setLineWidth(0);
	titleFrame->setMargin(0);
	titleFrame->setFrameShape(QFrame::NoFrame);
	titleFrame->setFrameShadow(QFrame::Plain);
	titleFrame->setFixedSize(frameWidth, 80);
	titleFrame->move(0, 0);
	
	titleLayout = new QVBoxLayout(titleFrame, 0, 0, "titleLayout");

	titleLabel = new QLabel(titleFrame, "titleLabel", 0);
	titleLabel->setText("Title");
	QFont titleFont(titleLabel->font());
#ifdef __ARM__
	titleFont.setPointSize(38);
#else
	titleFont.setPointSize(28);
#endif
	titleLabel->setFont(titleFont);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLayout->addWidget(titleLabel);

	topLine = new QFrame(titleFrame, "topLine");
	topLine->setFrameShape(QFrame::HLine);
	topLine->setFrameShadow(QFrame::Sunken);
	topLine->setFixedSize(frameWidth, 1);
	titleLayout->addWidget(topLine);

	/* middle left frame */

	/* middle frame */

	/* middle right frame */

	/* bottom frame */
	bottomFrame = new QFrame(this, "bottomFrame");
	QFont bottomFont = bottomFrame->font();
#ifdef __ARM__
	bottomFont.setPointSize(20);
#else
	bottomFont.setPointSize(18);
#endif
	bottomFrame->setFont(bottomFont);
	bottomFrame->setLineWidth(0);
	bottomFrame->setMargin(0);
	bottomFrame->setFrameShape(QFrame::NoFrame);
	bottomFrame->setFrameShadow(QFrame::Plain);
	bottomFrame->setFixedSize(frameWidth, 30);
	bottomFrame->move(0, 450);

	bottomLayout = new QVBoxLayout(bottomFrame, 0, 0, "bottomLayout");

	bottomLine = new QFrame(bottomFrame, "bottomLine");
	bottomLine->setFrameShape(QFrame::HLine);
	bottomLine->setFrameShadow(QFrame::Sunken);
	bottomLine->setFixedSize(frameWidth, 1);
	bottomLayout->addWidget(bottomLine);

	bottomChildLayout = new QHBoxLayout(bottomLayout, 0, "childLayout");

	bottomChildLayout->addSpacing(5);

	dateLabel = new QLabel(bottomFrame, "dateLabel");
	QString dateTime;
	dateTime.sprintf("%04d-%02d-%02d", QDate::currentDate().year(), 
			QDate::currentDate().month(), QDate::currentDate().day());
	dateLabel->setText(dateTime);
	dateLabel->setAlignment(Qt::AlignCenter);
	dateLabel->adjustSize();
	bottomChildLayout->addWidget(dateLabel);

	int size = 400 - 5 - dateLabel->width();

	venderLabel = new QLabel(bottomFrame, "venderLabel");
	venderLabel->setText("太原罗克佳华工业有限公司");
	venderLabel->setAlignment(Qt::AlignCenter);
	venderLabel->adjustSize();
	bottomChildLayout->addWidget(venderLabel);
	
	size = size - venderLabel->width() / 2;
	bottomChildLayout->insertSpacing(2, size);

	bottomChildLayout->addStretch(5);

	for(int i = 0; i < 4; i++) {
		QString labelName = "iconLabel" + QString::number(i + 1);
		QLabel *iconLabel = new QLabel(bottomFrame, labelName);
#if 0
		iconLabel->setPixmap(QPixmap::fromMimeSource("icon_none_arm.png"));
#endif
		bottomChildLayout->addWidget(iconLabel);
		bottomChildLayout->addSpacing(2);
		iconStat[i] = IconNone;
	}
	bottomChildLayout->addSpacing(3);

	timeLabel = new QLabel(bottomFrame, "timeLabel");
	dateTime.sprintf("%02d:%02d:%02d", QTime::currentTime().hour(), 
			QTime::currentTime().minute(), QTime::currentTime().second());
	timeLabel->setText(dateTime);
	timeLabel->setAlignment(Qt::AlignCenter);
	bottomChildLayout->addWidget(timeLabel);
	bottomChildLayout->addSpacing(5);

	/* Keyboard */
	keyboard = new rkDlgKb(this, "keyboard");
	point1.setX(pos_x);
	point1.setY(pos_y + 80);
	point2.setX(pos_x + frameWidth - keyboard->width());
	point2.setY(pos_y + 80);
	point3.setX(pos_x);
	point3.setY(pos_y + frameHeight - keyboard->height() - 30);
	point4.setX(pos_x + frameWidth - keyboard->width());
	point4.setY(pos_y + frameHeight - keyboard->height() - 30);
	keyboard->move(point4);
	connect(keyboard, SIGNAL(sendKeyCode(const QString &)), this, SLOT(recvKeyCode(const QString &)));

	/* start the timer to update date and time display */
	timer1 = startTimer(500);
	timer2 = 0;
}

void rkFrame::setTitle(const QString &title)
{
	titleLabel->setText(title);
}

void rkFrame::timerEvent(QTimerEvent *timer)
{
	int id = timer->timerId();

	if (id == timer1 && isShown()) {
		updateTime();
	} else if (id == timer2) {
		screenOn();
	}

#if 0
	if (id == keyTimer) {
		killTimer(id);
		keyRepeat = false;
	}
#endif
}

void rkFrame::screenOn(void)
{
	ctx.usbflag = 1;
}

void rkFrame::screenOff(void)
{
	ctx.usbflag = 0;
}

void rkFrame::updateTime(void)
{
	QDate date = QDate::currentDate();
	QString str;
	str.sprintf("%04d-%02d-%02d", date.year(), date.month(), date.day());
	dateLabel->setText(str);

	QTime time = QTime::currentTime();
	str.sprintf("%02d:%02d:%02d", time.hour(), time.minute(), time.second());
	timeLabel->setText(str);

	if (ctx.usbflag) {
		showIcon(IconUsb);
	} else {
		hideIcon(IconUsb);
	}
}

void rkFrame::showKeyboard(int pos)
{
	QPoint point;
	switch(pos) {
		case TopLeft:
			point = point1;
			break;
		case TopRight:
			point = point2;
			break;
		case BottomLeft:
			point = point3;
			break;
		case BottomRight:
			point = point4;
			break;
	}
	keyboard->move(point);
	if (keyboard->isHidden()) {
		keyboard->show();
		showIcon(IconKeyboard);
		this->setFocus();
	}
}

void rkFrame::hideKeyboard()
{
	if (keyboard->isShown()) {
		keyboard->hide();
		hideIcon(IconKeyboard);
	}
}

void rkFrame::showIcon(IconType type)
{
	QStringList iconTypeList = QStringList::split(",", "none,alm,kb,usb,drop,", true);
	QString iconTypeName;
	QString labelName;
	QLabel *label;
	int index = -1;

	for(int i = 0; i < 4; i++) {
		if (iconStat[i] == type) {
			return;
		} else if (iconStat[i] == IconNone) {
			index = i;
		}
	}

	if (index != -1) {
		labelName = "iconLabel" + QString::number(index + 1, 10);
		label = (QLabel *)this->child(labelName, "QLabel", true);
		if (!label) {
			return;
		} 
		iconTypeName = "icon_" + iconTypeList[type] + "_arm.png";
		QPixmap pixmap = QPixmap::fromMimeSource(iconTypeName);
		label->setPixmap(pixmap);
		label->show();
		iconStat[index] = type;
	}
}

void rkFrame::hideIcon(IconType type)
{
	QString labelName;
	QLabel *label;
	for(int i = 0; i < 4; i++) {
		if (iconStat[i] == type) {
			labelName = "iconLabel" + QString::number(i + 1, 10);
			label = (QLabel *)this->child(labelName, "QLabel", true);
			if (!label) {
				return;
			} 
			QPixmap pixmap = QPixmap::fromMimeSource("icon_none_arm.png");
			label->setPixmap(pixmap);
			label->hide();
			iconStat[i] = IconNone;
		}
	}
}

void rkFrame::recvKeyCode(const QString &keyCode)
{
	if (keyCode == "Hide") {
		hideKeyboard();
	}
}

QFrame *rkFrame::createLeftFrame(QWidget *parent)
{
	leftFrame = new QFrame(parent);
	QFont frameFont = leftFrame->font();
#ifdef __ARM__
	frameFont.setPointSize(22);
#else
	frameFont.setPointSize(18);
#endif
	leftFrame->setFont(frameFont);
	leftFrame->setLineWidth(0);
	leftFrame->setMargin(0);
	leftFrame->setFrameShape(QFrame::NoFrame);
	leftFrame->setFrameShadow(QFrame::Plain);
	leftFrame->setFixedSize(80, 370);
	leftFrame->move(0, 80);

	return leftFrame;
}

QFrame *rkFrame::createMainframe(QWidget *parent)
{
	mainFrame = new QFrame(parent);
	QFont frameFont = mainFrame->font();
#ifdef __ARM__
	frameFont.setPointSize(22);
#else
	frameFont.setPointSize(14);
#endif

	mainFrame->setFont(frameFont);
	mainFrame->setLineWidth(0);
	mainFrame->setMargin(0);
	mainFrame->setFrameShape(QFrame::NoFrame);
	mainFrame->setFrameShadow(QFrame::Plain);
	mainFrame->setFixedSize(frameWidth - 160, 370);
	mainFrame->move(80, 80);

	return mainFrame;
}

QFrame *rkFrame::createRightFrame(QWidget *parent)
{
	rightFrame = new QFrame(parent, "rightframe");
	QFont frameFont = rightFrame->font();
#ifdef __ARM__
	frameFont.setPointSize(22);
#else
	frameFont.setPointSize(18);
#endif
	rightFrame->setFont(frameFont);
	rightFrame->setLineWidth(0);
	rightFrame->setMargin(0);
	rightFrame->setFrameShape(QFrame::NoFrame);
	rightFrame->setFrameShadow(QFrame::Plain);
	rightFrame->setFixedSize(80, 370);
	rightFrame->move(720, 80);

	return rightFrame;
}

void rkFrame::mouseMoveEvent(QMouseEvent *e)
{
	//printf("x:%d y:%d\n", e->x(), e->y());
}

#if 0
void rkFrame::keyPressEvent(QKeyEvent *e)
{
}

void rkFrame::keyReleaseEvent(QKeyEvent *e)
{
	if (keyRepeat == true) {
		return;
	}

	if (e->key() == Key_Up) {
		puts("up");
	} else if (e->key() == Key_Down) {
		puts("down");
	} else if (e->key() == Key_Left) {
		puts("left");
	} else if (e->key() == Key_Right) {
		puts("right");
	} else if (e->key() == Key_Return) {
		puts("return");
	} else if (e->key() == Key_Escape) {
		puts("Esc");
	} else {
		puts("unknow");
	}

	keyRepeat = true;
	keyTimer = startTimer(1000);

	return;
}
#endif
