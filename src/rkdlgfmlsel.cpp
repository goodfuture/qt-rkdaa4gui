#include "rkdlgfmlsel.h"
#include "rkdlgfml.h"
#include <qtable.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qstring.h>

rkDlgFmlSel::rkDlgFmlSel(QWidget *parent, const char *name)
	:rkDlg(parent, name)
{
	setTitle("公式选择器");

	/* Global Variable Initialize */
	currentRow = -1;
	oldRow = -1;

	/* Middle Frame */
	formulaFrame = createMiddleFrame(this);
	QHBoxLayout *layout = new QHBoxLayout(formulaFrame, 0, 0, "layout0");
	layout->addSpacing(20);
	table = new QTable(formulaFrame, "formulaTable");
	table->setSelectionMode(QTable::SingleRow);
	table->setReadOnly(true);
	table->setDragEnabled(false);
	setColumnLabels(QStringList::split(",", "公式,说明"));
	connect(table, SIGNAL(currentChanged(int, int)), this, SLOT(selectChanged(int, int)));
	layout->addWidget(table);
	layout->addSpacing(20);

	/* Bottom Frame */
	buttonFrame = createButtonFrame(this);
	layout = new QHBoxLayout(buttonFrame, 0, 10, "layout1");
	layout->addStretch(0);
	QStringList btnText = QStringList::split(",", "增加,移除,取消,确定");
	for(unsigned int i = 0; i < btnText.count(); i++) {
		QPushButton *btn = new QPushButton(buttonFrame, QString::number(i));
		btn->setText(btnText[i]);
		connect(btn, SIGNAL(clicked()), this, SLOT(processButtonClicked()));
		layout->addWidget(btn);
	}
	layout->addSpacing(20);

	/* Create Edit Dialog */
	editDialog = new rkDlgFml(this, "editDialog");
	connect(editDialog, SIGNAL(closed()), this, SIGNAL(hideKeyboard()));
	connect(editDialog, SIGNAL(showKeyboard()), this, SIGNAL(showKeyboard()));
	connect(editDialog, SIGNAL(newFormula(const QString &, const QString &)), this, SLOT(addNewFormula(const QString &, const QString &)));
	connect(this, SIGNAL(sendKeyCode(const QString &)), editDialog, SLOT(recvKeyCode(const QString &)));
}

void rkDlgFmlSel::processButtonClicked()
{
	QPushButton *btn = (QPushButton *)this->sender();
	QString name = btn->name(); /* "btn" + Number */
	int index = name.toInt();
	switch(index) {
		case 0: /* Add */
			editDialog->show();
			break;
		case 1: /* Remove */
			remove();
			break;
		case 2: /* Cancel */
			close();
			break;
		case 3: /* Ok */
			if(currentRow >= 0) {
				emit formula(table->text(currentRow, 0));
			}
			close();
			break;
		default:
			break;
	}
}

void rkDlgFmlSel::addNewFormula(const QString &formula, const QString &info)
{
	int row = table->numRows();
	table->setNumRows(row + 1);
	table->setText(row, 0, formula); /* formula */
	table->setText(row, 1, info); /* specify */
}

void rkDlgFmlSel::remove()
{
	if(currentRow >= 0) {
		table->removeRow(currentRow);
	}
}

void rkDlgFmlSel::recvKeyCode(const QString &keycode)
{
	emit sendKeyCode(keycode);
}

void rkDlgFmlSel::selectChanged(int row, int)
{
	oldRow = currentRow;
	currentRow = row;
}

void rkDlgFmlSel::setColumnLabels(const QStringList &labels)
{
	table->setNumCols(labels.count());
	table->setColumnLabels(labels);
	for(int i = 0; i < table->numCols(); i++) {
		table->setColumnStretchable(i, true);
	}
}
