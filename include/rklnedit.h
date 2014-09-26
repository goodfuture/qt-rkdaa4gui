#ifndef _RK_LINE_EDIT_H_
#define _RK_LINE_EDIT_H_ 

#include <qlineedit.h>

class rkLnEdit : public QLineEdit 
{
	Q_OBJECT
	public:
		rkLnEdit(QWidget *parent = 0, const char *name = 0);
		~rkLnEdit(){};
	protected:
		virtual void mouseReleaseEvent(QMouseEvent *e);
	signals:
		void clicked();
};

#endif /* _RK_LINE_EDIT_H_ */
