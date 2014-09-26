#include "rktrdmnt.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void rkTrdMnt::run()
{
#if 0
	struct tsdev *ts;
	struct ts_sample sample;
	int count;
	char *tsdevice = getenv("TSLIB_TSDEVICE");
	if (tsdevice == NULL) {
		printf("rkTrdMnt : tsdevice is NULL!\n");
		return;
	}

	ts = ts_open(tsdevice, 1);
	if (!ts) {
		printf("rkTrdMnt : ts_open() failed!\n");
		return;
	}

	if (ts_config(ts)) {
		printf("rkTrdMnt : ts_config() failed!\n");
		return;
	}

	do {
		if (ts_read(ts, &sample, 1) < 0 && errno != EAGAIN) {
			printf("rkTrdMnt : ts_read_raw() failed!\n");
			return;
		}
		count = !sample.pressure ? count + 1 : 0;
		count = count > 60000 ? 60000 : count;
		if (count >= 60000) {
			system("echo 0 > /sys/class/backlight/backlight/brightness");
		} else {
			system("echo 255 > /sys/class/backlight/backlight/brightness");
		}

		printf("count = %d\n", count);
		usleep(1000);
	} while(1);
#endif

	return;
}
