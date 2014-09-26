#include "rkdlg.h"
#include "rkclr.h"
#include <qlabel.h>
#include <qframe.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <qapplication.h>
#include <qpoint.h>
#include <qfont.h>

rkDlg::rkDlg(QWidget *parent, const char *name, int width, int height)
	:QDialog(parent, name, false, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{
	setFrameSize(width, height);
	QFont font = this->font();
#ifdef __ARM__
	font.setPointSize(20);
#else
	font.setPointSize(14);
#endif
	this->setFont(font);
	/* Set Background Color */
#if 0
	this->setPaletteBackgroundColor(rkClr(92, 126, 128));
	this->setPaletteForegroundColor(rkClr(255, 255, 255));
#endif

	/* Frame Layout */
	frameLayout = new QVBoxLayout(this, 0, 0, "frameLayout");
	
	/* Head Frame */
	headFrame = new QFrame(this, "headFrame");
	headFrame->setFixedSize(frameWidth, 40);
	frameLayout->addWidget(headFrame);
	QHBoxLayout *layout = new QHBoxLayout(headFrame, 0, 0, "headLayout");
	titleLabel = new QLabel(headFrame, "titleLabel");
	font = titleLabel->font();
#ifdef __ARM__
	font.setPointSize(28);
#else
	font.setPointSize(18);
#endif
	titleLabel->setFont(font);
	titleLabel->setText("Base Dialog");
	titleLabel->setAlignment(Qt::AlignCenter);
	layout->addWidget(titleLabel);
}

void rkDlg::setTitle(const QString &title)
{
	titleLabel->setText(title);
}

void rkDlg::setFrameSize(int width, int height)
{
	frameWidth = width;
	frameHeight = height;
	setFixedSize(frameWidth, frameHeight);
	int pos_x = (QApplication::desktop()->width() - frameWidth) / 2;
	int pos_y = (QApplication::desktop()->height() - frameHeight) / 2;
	move(pos_x, pos_y);
	update();
}

QFrame *rkDlg::createMiddleFrame(QWidget *parent)
{
	middleFrame = new QFrame(parent, "middleFrame");
	middleFrame->setFixedSize(frameWidth, frameHeight - 80);
	frameLayout->addWidget(middleFrame);

	return middleFrame;
}

QFrame *rkDlg::createButtonFrame(QWidget *parent)
{
	buttonFrame = new QFrame(parent, "buttonFrame");
	buttonFrame->setFixedSize(frameWidth, 40);
	frameLayout->addWidget(buttonFrame);

	return buttonFrame;
}

void rkDlg::moveOffset(int x, int y)
{
	QPoint point = this->pos();
	point.rx() += x;
	point.ry() += y;
	this->move(point);
}
