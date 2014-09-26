#ifndef _RK_MESSAGER_THREAD_H_
#define _RK_MESSAGER_THREAD_H_

#include <qthread.h>
#include <qobject.h>
#include "rkshm.h"

class rkTrdMsg : public QObject, public QThread
{
	Q_OBJECT
	public:
		rkTrdMsg(){};
		virtual void run();

	signals:
		void sendCoreStatus(bool run, const char *ver);
	public slots:
		void procUICmd(int cmd);
};

#endif /* _RK_MESSAGER_THREAD_H_ */
