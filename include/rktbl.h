#ifndef _RK_TABLE_VIEW_H_
#define _RK_TABLE_VIEW_H_

#include <qtable.h>

class rkTblItem : public QTableItem {
	public:
		rkTblItem(QTable *table, EditType et, const QString &text) : QTableItem(table, et, text) {};
		virtual int alignment() const {
			return Qt::AlignHCenter | Qt::AlignVCenter;
		};
};

class rkTbl : public QTable {
	public:
		rkTbl(QWidget *parent = 0, const char *name = 0) : QTable(parent, name) {};
		~rkTbl(){};
		virtual void setText(int row, int col, const QString &text) {
			QTableItem* itm = item(row, col);
			if (!itm) {
				QTableItem *i = new rkTblItem(this, QTableItem::OnTyping, text);
				setItem(row, col, i);
			} else {
				QTable::setText(row, col, text);
			}
		};
#if 0
		virtual void paintCell(QPainter *p, int row, int col, const QRect &cr, bool selected, const QColorGroup &cg) {
		};
#endif
};
#endif /* _RK_TABLE_VIEW_H_ */
