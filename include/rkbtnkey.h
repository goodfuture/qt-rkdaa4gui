#ifndef _RK_KEY_BUTTON_H_
#define _RK_KEY_BUTTON_H_

#include <qpushbutton.h>

class rkBtnKey : public QPushButton 
{
	Q_OBJECT
	public:
		rkBtnKey(QWidget *parent, const char *name);
		~rkBtnKey(){};
};

#endif /* _RK_KEY_BUTTON_H_ */
