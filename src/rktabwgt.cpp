#include "rktabwgt.h"
#include "rkclr.h"
#include <qtabbar.h>

rkTabWgt::rkTabWgt(QWidget *parent, const char *name)
	:QTabWidget(parent, name, Qt::WStyle_Customize | Qt::WStyle_NoBorder)
{
}
