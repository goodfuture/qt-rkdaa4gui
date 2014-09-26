#ifndef _RK_APPLICATION_H_
#define _RK_APPLICATION_H_

#include <qapplication.h>

#ifdef __ARM__
class QWSEvent;
#endif

class rkApp : public QApplication {
	Q_OBJECT
	public:
		rkApp(int argc, char **argv);
		virtual ~rkApp(){};

#ifdef __ARM__
	protected:
		virtual bool qwsEventFilter(QWSEvent *event);
		virtual void timerEvent(QTimerEvent *);

	private:
		int timer;
		int interval;
		bool isBacklightOn;
#endif /* __ARM__ */
};

#endif /* _RK_APPLICATION_H_ */
