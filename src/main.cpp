/*******************************************************************************
 * Copyright(C)		: Rockontrol Industrial Park.
 * Version			: V1.0
 * Author			: KangQi
 * Tel				: 18636181581/6581
 * Email			: goodfuturemail@gmail.com
 * File Name		: main.cpp
 * Created At		: 2013-09-22 14:00
 * Last Modified	: 2014-01-09 10:28
 * Description		: 
 * History			: 
*******************************************************************************/
#include "rktype.h"
#include "rkshm.h"
#include "rkinit.h"
#include "rkfmpri.h"
#include "rktrdmnt.h"
#include "rkapp.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <qtextcodec.h>
#include <qsplashscreen.h>
#include <qfont.h>
#include <qcursor.h>
#include <rkfmscrcalib.h>

struct context ctx;
rkFmScrCalib *scrcalib;

int main(int argc, char *argv[])
{
	rkParseCliArg(&ctx, argc, argv);
	rkApp app(argc, argv);

	ctx.scrflag = 2;

	QCursor cursor;
	cursor.setShape(Qt::BlankCursor);
	QApplication::setOverrideCursor(cursor, false);

	/* Set Application Codec */
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

	/* Set Application Font */
	QFont font = app.font();
	font.setPointSize(14);
	app.setFont(font);

	/* Set Image Files Path */
	QMimeSourceFactory *imageFactory = QMimeSourceFactory::defaultFactory();
	imageFactory->setFilePath(IMG_DIR);

	/* Create Splash Screen */
	QSplashScreen splashScreen(QPixmap(QString(IMG_DIR) + "/splash_arm.png"));
	splashScreen.show();
	app.processEvents();

	/* Initialize Share Memory */
	rkShmInit();

	scrcalib = new rkFmScrCalib();
	scrcalib->hide();

	/* Main Page */
	rkFmPri mainPage;
	mainPage.show();
	splashScreen.finish(&mainPage);

	/* Create Daemon Thread, Listen Mouse And Keyboard Event, then Decide Whether Or Not To Turn Off The LCD Screen */
#if 0
	rkTrdMnt monitor;
	monitor.start();
#endif

	app.connect(&app, SIGNAL(lastWindowClosed()), SLOT(quit()));

	return app.exec();
}
