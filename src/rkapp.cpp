#include "rkapp.h"
#include "rkclr.h"
#include <qimage.h>
#ifdef __ARM__
#include <qwindowsystem_qws.h>
#endif

rkApp::rkApp(int argc, char **argv)
	: QApplication(argc, argv, QApplication::GuiServer)
{
#ifdef __ARM__
	qwsServer->setDesktopBackground(rkClr(47, 63, 95));
	system("echo 255 > /sys/class/backlight/backlight/brightness");
	isBacklightOn = true;
	interval = 600000; /* 600 seconds */
	timer = startTimer(interval);
#endif
}

#ifdef __ARM__
bool rkApp::qwsEventFilter(QWSEvent *event)
{
	switch(event->type) {
		case QWSEvent::Mouse:
			if (isBacklightOn) {
				killTimer(timer);
			} else if (!isBacklightOn) {
				isBacklightOn = true;
				system("echo 255 > /sys/class/backlight/backlight/brightness");
			}
			timer = startTimer(interval);
			break;
		case QWSEvent::Key:
			break;
		default:
			break;
	}

	return QApplication::qwsEventFilter(event);
}

void rkApp::timerEvent(QTimerEvent *e)
{
	if (isBacklightOn) {
		//system("echo 0 > /sys/class/backlight/backlight/brightness");
	}
	isBacklightOn = false;
	timer = 0;
}
#endif /* __ARM__ */
