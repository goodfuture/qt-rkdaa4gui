#include "rkdlgsel.h"
#include "rkclr.h"
#include "rkckbox.h"
#include <qlabel.h>
#include <qobjectlist.h>
#include <qapplication.h>
#include <qfont.h>
#include <qlayout.h>
#include <qpushbutton.h>

rkDlgSel::rkDlgSel(QWidget *parent, const char *name, bool modal, WFlags f)
	:QDialog(parent, name, modal, f)
{
	selcnt = 0;
	maxselnum = 5;

	ifont = this->font();
#ifdef __ARM__
	ifont.setPointSize(20);
#else
	ifont.setPointSize(14);
#endif
	this->setFont(ifont);

	/* Set Foreground & Background Color */
	this->setPaletteBackgroundColor(rkClr(215, 215, 215));
	this->setPaletteForegroundColor(rkClr(30, 30, 30));

	/* Set Frame Geometory */
	frmWidth = 520;
	frmHeight = 200;
	pos_x = (QApplication::desktop()->width() - frmWidth) / 2;
	pos_y = (QApplication::desktop()->height() - frmHeight) / 2;
	this->resize(frmWidth, frmHeight);
	this->move(pos_x, pos_y);

	glayout = new QGridLayout(this);
	glayout->setSpacing(8);
	glayout->setRowSpacing(10, 20);
	glayout->setColSpacing(0, 20);
	glayout->setColSpacing(10, 20);

	hlayout = new QHBoxLayout;
	hlayout->setAlignment(Qt::AlignCenter);
	glayout->addMultiCellLayout(hlayout, 0, 0, 1, 9);

	title  = new QLabel(this);
	title->setAlignment(Qt::AlignCenter);
	title->setText("选择对话框");
	hlayout->addWidget(title);

	ifont = title->font();
#ifdef __ARM__
	ifont.setPointSize(28);
#else
	ifont.setPointSize(18);
#endif
	title->setFont(ifont);

	prompt = new QLabel(this);
	prompt->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	glayout->addMultiCellWidget(prompt, 7, 7, 1, 9);

	hlayout = new QHBoxLayout;
	hlayout->addStretch(20);
	glayout->addMultiCellLayout(hlayout, 8, 8, 1, 9);

	btn = new QPushButton(this, "0");
	btn->setText("清除");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnClick()));
	hlayout->addWidget(btn);

	btn = new QPushButton(this, "1");
	btn->setText("取消");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnClick()));
	hlayout->addWidget(btn);

	btn = new QPushButton(this, "2");
	btn->setText("确定");
	connect(btn, SIGNAL(clicked()), this, SLOT(procBtnClick()));
	hlayout->addWidget(btn);

	codemap.clear();
}

void rkDlgSel::setFitSize()
{
	adjustSize();
	frmWidth = width();
	frmHeight = height();
	pos_x = (QApplication::desktop()->width() - frmWidth) / 2;
	pos_y = (QApplication::desktop()->height() - frmHeight) / 2;
	this->move(pos_x, pos_y);
}

void rkDlgSel::setTitle(const QString& txt)
{
	title->setText(txt);
}

QString rkDlgSel::getSelItems()
{
	QString items;

	objlist = queryList("rkCkBox");
	QObjectListIt objlistIt(*objlist);
	for (objlistIt.toFirst(); objlistIt; ++objlistIt) {
		ckbox = ((rkCkBox *)objlistIt.current());
		if (ckbox->isChecked()== false) continue;
		items += codemap[QString(ckbox->name()).toInt()];
		items += ";";

	}
	delete objlist;

	return items;
}

void rkDlgSel::procBtnClick()
{
	QPushButton *btn = (QPushButton *)this->sender();
	switch(QString(btn->name()).toInt()) {
		case 0:
			clearSelect();
			break;
		case 1:
			hide();
			break;
		case 2:
			emit selectedItems(getSelItems());
			close();
			break;
	}
}

void rkDlgSel::procCkboxSel(bool state)
{
	ckbox= (rkCkBox *)this->sender();

	selcnt = state ? selcnt + 1 : selcnt - 1;

	if (selcnt > maxselnum) {
		if (maxselnum == 1) {
			clearSelect();
			ckbox->setChecked(true);
			selcnt++;
			return;
		} else {
			ckbox->setChecked(false);
			selcnt--;
			prompt->setText("超过最大选择数量:" + QString::number(maxselnum));
			return;
		}
	}

	prompt->setText("");
	ckbox->setChecked(state);
}

void rkDlgSel::clearSelect()
{
	rkCkBox *lckbox;
	objlist = queryList("rkCkBox");
	QObjectListIt objlistIt(*objlist);
	for (objlistIt.toFirst(); objlistIt; ++objlistIt) {
		lckbox = ((rkCkBox *)objlistIt.current());
		lckbox->setEnabled(true);
		lckbox->setChecked(false);
	}
	delete objlist;
	selcnt = 0;
	prompt->setText("");
}

void rkDlgSel::addItem(const QString &labeltxt, const QString &code, int index)
{
	int row = index % 6 + 1;
	int col = index / 6 * 2 + 1;

	label = new QLabel(this);
	label->setText(labeltxt);
	label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	glayout->addWidget(label, row, col++);

	ckbox = new rkCkBox(this, QString::number(index));
	ckbox->setChecked(false);
	connect(ckbox, SIGNAL(checked(bool)), this, SLOT(procCkboxSel(bool)));
	glayout->addWidget(ckbox, row, col);

	codemap.insert(index, code);
}

void rkDlgSel::setMaxSelect(int num)
{
	if (num > 0) {
		maxselnum = num;
	}
}
