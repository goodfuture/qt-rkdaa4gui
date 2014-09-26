#ifndef _RK_SELECT_DIALOG_H_
#define _RK_SELECT_DIALOG_H_ 

#include <qdialog.h>
#include <qfont.h>
#include <qmap.h>

class QFont;
class QFrame;
class QLabel;
class QStringList;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class rkCkBox;
class QObjectList;

class rkDlgSel : public QDialog 
{
	Q_OBJECT
	public:
		rkDlgSel(QWidget *parent = 0, const char *name = 0, bool modal = false, WFlags f = Qt::WStyle_Customize | Qt::WStyle_NoBorder);
		~rkDlgSel(){};
	private:
		QFont ifont;
		int frmWidth;
		int frmHeight;
		int pos_x;
		int pos_y;
		QLabel *title;
		QLabel *prompt;
		QLabel *label;
		QHBoxLayout *hlayout;
		QGridLayout *glayout;
		QPushButton *btn;
		rkCkBox *ckbox;
		QObjectList *objlist;
		int selcnt;
		int maxselnum;
		QMap<int, QString> codemap;
	public slots:
		void setFitSize();
		void setTitle(const QString&);
		void setMaxSelect(int);
		void addItem(const QString &, const QString &, int);
		QString getSelItems();
		void clearSelect();
	private slots:
		void procBtnClick();
		void procCkboxSel(bool);
	signals:
		void selectedItems(QString items);
};

#endif /* _RK_SELECT_DIALOG_H_ */
