#include "rklnedit.h"

rkLnEdit::rkLnEdit(QWidget *parent, const char *name)
	:QLineEdit(parent, name)
{
	this->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	this->setDragEnabled(false);
}

void rkLnEdit::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->state() != Qt::LeftButton) {
		return;
	}
	emit clicked();
}
