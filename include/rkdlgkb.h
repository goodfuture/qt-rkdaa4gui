#ifndef _RK_KEYBOARD_H_
#define _RK_KEYBOARD_H_

#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QStringList;
class QButtonGroup;

class rkDlgKb : public QDialog
{
	Q_OBJECT

	public:
		rkDlgKb(QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags fl = Qt::WStyle_Customize | Qt::WStyle_NoBorder | WStyle_Tool);
	private:
		QButtonGroup *btngroup;
		QVBoxLayout *keyboardLayout;
		QHBoxLayout *row;
		QString keyValueLower;
		QStringList keyMapLower;
		QString keyValueUpper;
		QStringList keyMapUpper;
	protected:
		bool isUpper;
	public slots:
	private slots:
		void convertKeyMap(void);
		void processKeyPressed(void);
	signals:
		void sendKeyCode(const QString &keyCode);
};

#endif /* _RK_KEYBOARD_H_ */
