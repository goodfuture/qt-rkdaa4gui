#ifndef _RK_FORM_DATA_QUERY_H_
#define _RK_FORM_DATA_QUERY_H_ 

#include <qwidget.h>
#include <qfont.h>
#include <sqlite3.h>
#include <qmap.h>
#include <qevent.h>

class QGridLayout;
class QHBoxLayout;
class QStringList;
class rkDlgSel;
class rkLnEdit;
class QLabel;
class QWidgetStack;
class QPushButton;
class QDateTimeEdit;
class rkCkBox;
class rkTbl;

typedef QMap<rkCkBox *, QString> ckMap;

/* data query subpage */
class rkFmQryRtd : public QWidget
{
	Q_OBJECT

	public:
		rkFmQryRtd(QWidget *parent = 0, const char *name = 0);
	private:
		QFont gfont;
		QWidgetStack *wstack;
		QWidget *qrywgt;
		QWidget *dspwgt;
		QWidget *selwgt;
		QGridLayout *glayout;
		QHBoxLayout *layout;
		QLabel *label;
		rkLnEdit *lnedit;
		rkCkBox *ckbox;
		QPushButton *btn;
		QDateTimeEdit *tmedit[2];
		sqlite3 *db;
		char *sql;
		ckMap map;
		QMap<QString, QString>cmap;
		rkTbl *tbl;
		char **qryRst;
		int row;
		int col;
		int pagenum;
		int pagecnt;
	private slots:
		void clickProc();
		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent(QKeyEvent *e);
};

#endif /* _RK_SUBFORM1_H_ */
