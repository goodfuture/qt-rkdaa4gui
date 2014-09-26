#ifndef _RK_COLOR_H_
#define _RK_COLOR_H_ 

#include <qcolor.h>

class rkClr : public QColor
{
	public:
		rkClr();
		rkClr(int r, int g, int b);
		rkClr(const QColor &);
		~rkClr(){};
};

#endif /* _RK_COLOR_H_ */
