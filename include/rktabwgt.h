#ifndef _RK_TAB_WIDGET_H_
#define _RK_TAB_WIDGET_H_

#include <qtabwidget.h>

class rkTabWgt: public QTabWidget 
{
	Q_OBJECT
	public:
		rkTabWgt(QWidget *parent = 0, const char *name = 0);
};

#endif /* _RK_TAB_WIDGET_H_ */
