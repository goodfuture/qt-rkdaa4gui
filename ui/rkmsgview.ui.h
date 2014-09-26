/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/


void rkMsgView::procBtnClicked( int id)
{
	QWidget *visible = widgetStack->visibleWidget();

	switch(id) {
		case 0:
			if (visible == WStackPage0) { /* 上一页 */
				currentPage--;
				currentPage = currentPage > 0 ? currentPage : 1;
				showMsgList();
			} else if (visible == WStackPage1) { /* 查询 */
				startQueryDatabase();
			}
			break;
		case 1:
			if (visible == WStackPage0) { /* 下一页  */
				currentPage++;
				if (currentPage >= totalPage) {
					currentPage = totalPage;
				}
				showMsgList();
			} else if (visible == WStackPage1) { /* 返回主界面 */
				close();
			}
			break;
		case 2: /* 上一条 */
			{
				int item = listBox->currentItem();
				item--;
				if (item < 0) {
					item = 0;
					return;
				} 
				listBox->setCurrentItem(item);
			} break;
		case 3: /* 下一条 */
			{
				int item = listBox->currentItem();
				item++;
				if (item > listBox->count()) {
					item--;
					return;
				}
				listBox->setCurrentItem(item);
			} break;
		case 4: /* 返回查询界面 */
			sqlite3_free_table(ctx.qryRst);
			sqlite3_close(ctx.db);
			widgetStack->raiseWidget(WStackPage1);
			showMode(1);
			break;
		default:
			break;
	}
}


void rkMsgView::init()
{
	this->setPaletteForegroundColor(rkClr(255, 255, 255));
	this->setPaletteBackgroundColor(rkClr(47, 63, 95));
	this->setWindowState(WindowFullScreen);
	this->setFixedSize(800, 480);

	currentPage = 1;
	totalPage = 1;

	listBox->clear();
	for (int index = 0; index < 6; index++) {
		listBox->insertItem(QString(""), index);
	}

	WStackPage1 = new QGroupBox(widgetStack);
	//WStackPage1 = new QWidget(widgetStack);
	widgetStack->addWidget(WStackPage1);
	widgetStack->raiseWidget(WStackPage1);
	showMode(1);

#ifdef __ARM__
	QFont font = caption->font();
	font.setPointSize(34);
	caption->setFont(font);

	font = buttonGroup->font();
	font.setPointSize(20);
	this->setFont(font);

	font = this->font();
	font.setPointSize(20);
	this->setFont(font);

	font = WStackPage1->font();
	font.setPointSize(24);
	WStackPage1->setFont(font);
#endif
	//QGroupBox *gbox = new QGroupBox(WStackPage1);
	WStackPage1->setTitle("查询条件");

	glayout = new QGridLayout(WStackPage1);
	glayout->setSpacing(8);
	glayout->setRowStretch(0, 10);
	glayout->setRowSpacing(4, 10);
	glayout->setRowStretch(6, 10);
	glayout->setColStretch(0, 1); 
	glayout->setColSpacing(2, 360);
	glayout->setColStretch(3, 1); 

	QLabel *label = new QLabel(WStackPage1);
	label->setText("报文类型");
	glayout->addWidget(label, 1, 1); 
	label = new QLabel(WStackPage1);
	label->setText("起始时间");
	glayout->addWidget(label, 2, 1); 
	label = new QLabel(WStackPage1);
	label->setText("终止时间");
	glayout->addWidget(label, 3, 1); 

	combox = new QComboBox(WStackPage1);
	combox->insertItem("实时报文", 0); 
	combox->insertItem("分钟报文", 1); 
	combox->insertItem("小时报文", 2); 
	combox->insertItem("日报文", 3); 
	glayout->addWidget(combox, 1, 2); 

	tmedit[0] = new QDateTimeEdit(QDateTime::currentDateTime(), WStackPage1);
	tmedit[0]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[0], 2, 2);

	tmedit[1] = new QDateTimeEdit(QDateTime::currentDateTime(), WStackPage1);
	tmedit[1]->dateEdit()->setOrder(QDateEdit::YMD);
	glayout->addWidget(tmedit[1], 3, 2);

	QHBoxLayout *layout = new QHBoxLayout;
	glayout->addMultiCellLayout(layout, 5, 5, 1, 2);
	layout->setSpacing(10);

	promptLabel = new QLabel(WStackPage1);
	promptLabel->clear();
	promptLabel->setHidden(true);
	layout->addWidget(promptLabel);
	layout->addStretch(10);

	progressBar = new QProgressBar(WStackPage1);
	progressBar->setFixedWidth(200);
	progressBar->setPercentageVisible(false);
	progressBar->setHidden(true);
	layout->addWidget(progressBar);
}


void rkMsgView::destory()
{

}


void rkMsgView::recvQryCond( int tbl_index, unsigned long start_time, unsigned long end_time )
{
	this->show();
}


void rkMsgView::timerEvent( QTimerEvent * )
{
	static int cnt = 0;

	progressBar->setProgress(cnt++);
	qApp->processEvents();
#if 1
	cnt %= progressBar->totalSteps() + 1;

	if (ctx.query_flag & QUERY_FINISH_BIT_MASK) {
		killTimer(timerA);
		progressBar->setShown(false);
		promptLabel->setShown(false);

		if (ctx.query_flag & QUERY_SUCCESS_BIT_MASK) {
			widgetStack->raiseWidget(WStackPage0);
			printf("ctx.row = %d, ctx.col = %d\n", ctx.row, ctx.col);
			totalPage = (ctx.row + 5) / 6;
			currentPage = 1;
			showMsgList();
		} else {
			QMessageBox::warning(this, "查询失败", QString("查询数据失败!\n") + QString(ctx.err), "确定");
		}
		buttonGroup->setEnabled(true);
	}
#endif
}


void rkMsgView::setWgtShown( int id)
{
	showMode(id);
}


void rkMsgView::showMode( int mode)
{
	switch(mode) {
		case 1: /* 查询界面 */
			pushButton0->setText("查询");
			pushButton1->setText("返回");
			pushButton2->setHidden(true);
			pushButton3->setHidden(true);
			pushButton4->setHidden(true);
			break;
		case 0: /* 显示界面 */
			pushButton0->setText("上一页");
			pushButton1->setText("下一页");
			pushButton2->setText("上一条");
			pushButton2->setShown(true);
			pushButton3->setText("下一条");
			pushButton3->setShown(true);
			pushButton4->setText("返回");
			pushButton4->setShown(true);
			break;
		default:
			break;
	}
}


void rkMsgView::startQueryDatabase()
{
	int rc;
	char *sql;
	time_t	stm, etm;

	stm = tmedit[0]->dateTime().toTime_t();
	etm = tmedit[1]->dateTime().toTime_t();
	if (stm > etm) {
		QMessageBox::warning(this, "查询失败", "检测到起始时间大于结束时间!", "确定");
		return;
	}

	if (combox->currentItem() == 0 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 1) {
		QMessageBox::warning(this, "查询失败", "实时报文查询时间跨度不能超过1天!", "确定");
		return;
	} else if (combox->currentItem() == 1 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 2) {
		QMessageBox::warning(this, "查询失败", "分钟报文查询时间跨度不能超过2天!", "确定");
		return;
	} else if (combox->currentItem() == 2 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 10) {
		QMessageBox::warning(this, "查询失败", "小时报文查询时间跨度不能超过10天!", "确定");
		return;
	} else if (combox->currentItem() == 3 && tmedit[0]->dateTime().daysTo(tmedit[1]->dateTime()) > 365) {
		QMessageBox::warning(this, "查询失败", "日报文查询时间跨度不能超过1年!", "确定");
		return;
	}

	QStringList tblname = QStringList::split(",", "RtmTable,MomTable,HomTable,DomTable,DimTable,AomTable");
	rc = sqlite3_open_v2(ctx.cliarg[ARG_MSG_DB_PATH_OFFSET], &ctx.db, SQLITE_OPEN_READONLY, NULL);
	if (rc) {
		sqlite3_close(ctx.db);
		QMessageBox::warning(this, "查询失败", "数据库打开失败!", "确定");
		return;
	}

	buttonGroup->setEnabled(false);
	progressBar->setTotalSteps(20);
	progressBar->reset();
	progressBar->setShown(true);
	promptLabel->setShown(true);
	promptLabel->setText("正在查询,请稍候...");
	qApp->processEvents();
	timerA = startTimer(500);

	ctx.sql = sqlite3_mprintf("SELECT * FROM %q WHERE timestamp >= %ld AND timestamp <= %ld;", tblname[combox->currentItem()].data(), stm, etm);

	emit sendUICmd(UI_CMD_QRY_DATABASE);
}

void rkMsgView::showEvent( QShowEvent * )
{
	tmedit[0]->setDateTime(QDateTime::currentDateTime());
	tmedit[1]->setDateTime(QDateTime::currentDateTime());
}

void rkMsgView::showMsgList()
{
	for (int index = 0; index < 6; index++) {
		int pos = ((currentPage - 1) * 6 + index + 1) * ctx.col;
		//printf("index = %d\n", (currentPage - 1) * 6 + index + 1);

		if (currentPage == totalPage) {
			int listcnt = ctx.row - (currentPage - 1) * 6;
			//printf("index = %d, listCnt = %d\n", index, listcnt);
			if (index >= listcnt) {
				listBox->changeItem(QString(""), index);
				continue;
			}
		}

		time_t msg_tm = atol(ctx.qryRst[pos + 1]);
		struct tm *tm_s = localtime(&msg_tm);
		item[index].sprintf("    %d-%d-%d %d:%d:%d", tm_s->tm_year + 1900, tm_s->tm_mon + 1, tm_s->tm_mday, tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec);
		item[index].prepend(QString::number((currentPage - 1) * 6 + index + 1));
		listBox->changeItem(item[index], index);
	}

	lCDNumber1->display(currentPage);
	lCDNumber2->display(totalPage);
	listBox->setCurrentItem(0);
}


void rkMsgView::showMessage( int index )
{
	//printf("index = %d\n", index);
#if 1
	int pos = ((currentPage - 1) * 6 + index + 1) * ctx.col;
	//printf("pos = %d\n", pos);
	//printf("msg = %s\n", qryRst[pos + 2]);
	if (currentPage == totalPage && index >= ctx.row - ((currentPage - 1) * 6)) {
		textEdit->clear();
	} else {
		textEdit->setText(ctx.qryRst[pos + 2]);
	}
#endif
}
