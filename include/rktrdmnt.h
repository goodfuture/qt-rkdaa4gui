#ifndef _RK_MONITOR_THREAD_H_
#define _RK_MONITOR_THREAD_H_

#include <qthread.h>
#include <qobject.h>
#include "tslib.h"

class rkTrdMnt : public QObject, public QThread {
	Q_OBJECT
	public :
		rkTrdMnt() {};
	protected:
		virtual void run();
	private:
};

#endif /* _RK_MONITOR_THREAD_H_ */
