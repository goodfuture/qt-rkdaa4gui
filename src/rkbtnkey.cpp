#include "rkbtnkey.h"
#include "rkclr.h"

rkBtnKey::rkBtnKey(QWidget *parent, const char *name)
	:QPushButton(parent, name)
{
	this->setPaletteBackgroundColor(rkClr(255, 255, 255));
}
