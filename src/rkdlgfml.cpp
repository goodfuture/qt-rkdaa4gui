#include "rkdlgfml.h"
#include "rklnedit.h"
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstringlist.h>

rkDlgFml::rkDlgFml(QWidget *parent, const char *name)
	:rkDlg(parent, name, 300, 150)
{
	setTitle("公式编辑器");
	moveOffset(0, -50);

	/* Global Vairable Initialize */
	focusedLineEdit = NULL;

	/* Edit Frame */
	editFrame = createMiddleFrame(this);
	QHBoxLayout *layout = new QHBoxLayout(editFrame, 0, 0, "layout0");
	layout->addSpacing(20);

	QGridLayout *childLayout = new QGridLayout;
	childLayout->setSpacing(10);
	layout->addLayout(childLayout);

	QStringList labelName = QStringList::split(",", "公式,说明");
	for(unsigned int i = 0; i < labelName.count(); i++)
	{
		QLabel *label = new QLabel(editFrame, "label" + QString::number(i));
		label->setText(labelName[i]);
		childLayout->addWidget(label, i, 0);

		rkLnEdit *lineEdit = new rkLnEdit(editFrame, "lineEdit" + QString::number(i));
		connect(lineEdit, SIGNAL(clicked()), this, SLOT(processEditClicked()));
		childLayout->addWidget(lineEdit, i, 1);
		switch(i) {
			case 0:
				lineEdit0 = lineEdit;
				break;
			case 1:
				lineEdit1 = lineEdit;
				break;
		}
	}
	layout->addSpacing(20);

	/* Button Frame */
	buttonFrame = createButtonFrame(this);
	layout = new QHBoxLayout(buttonFrame, 0, 10, "layout1");
	layout->addStretch(0);
	QStringList buttonLabel = QStringList::split(",", "取消,确定");
	for(unsigned int i = 0; i < buttonLabel.count(); i++)
	{
		QPushButton *btn = new QPushButton(buttonFrame, QString::number(i));
		btn->setText(buttonLabel[i]);
		connect(btn, SIGNAL(clicked()), this, SLOT(processBtnClicked()));
		layout->addWidget(btn);
	}
	layout->addSpacing(20);
}

void rkDlgFml::processBtnClicked()
{
	QPushButton *btn = (QPushButton *)this->sender();
	QString name = btn->name();
	int index = name.toInt();
	switch(index) {
		case 0: /* Cancel */
			emit closed();
			focusedLineEdit = NULL;
			close();
			break;
		case 1: /* Ok */
			emit closed();
			if(lineEdit0->text().length() != 0) {
				emit newFormula(lineEdit0->text(), lineEdit1->text());
			}
			focusedLineEdit = NULL;
			lineEdit0->clear();
			lineEdit1->clear();
			close();
			break;
		default:
			break;
	}
}

void rkDlgFml::processEditClicked()
{
	focusedLineEdit = (rkLnEdit *)this->sender();
	emit showKeyboard();
}

void rkDlgFml::recvKeyCode(const QString & keycode)
{
	if(!focusedLineEdit) {
		return;
	}

	if (keycode.length() == 1) {
		focusedLineEdit->insert(keycode);
	} else if (keycode.compare("Hide") == 0) {
		emit closed();
	} else if (keycode.compare("Enter") == 0) {
		/* 
		 * Do Nothing 
		 */
	} else if (keycode.compare("Back") == 0) {
		focusedLineEdit->backspace();
	} else if (keycode.compare("Space") == 0) {
		focusedLineEdit->insert(" ");
	}
}
