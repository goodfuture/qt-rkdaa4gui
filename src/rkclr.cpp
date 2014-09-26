#include "rkclr.h"

rkClr::rkClr()
	:QColor()
{
}

rkClr::rkClr(int r, int g, int b)
	:QColor(r, g, b)
{
#ifdef __ARM__
	this->setRgb(b, g, r);
#endif /* __ARM__ */
}

rkClr::rkClr(const QColor& c)
	:QColor(c)
{
#ifdef __ARM__
	int tmpR, tmpG, tmpB;
	tmpR = qBlue(c.rgb());
	tmpG = qGreen(c.rgb());
	tmpB = qRed(c.rgb());

	this->setRgb(tmpR, tmpG, tmpB);
#endif /* __ARM__ */
}
