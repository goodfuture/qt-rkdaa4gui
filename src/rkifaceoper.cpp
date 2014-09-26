#include "rkifaceoper.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

static const char *path = "/etc/network/interfaces";
static const char *tmp = ".tmp.txt";
static char arg[4][128];

char *RkIfaceGetParam(const char *iface, const char *param)
{
	char buf[128] = { 0 };
	char curIface[128] = { 0 }; /* Current Interface */
	int line = 0;

	FILE *fp = fopen(path, "r+");
	if (NULL == fp) {
		return NULL;
	}

	while(fgets(buf, sizeof(buf), fp) != NULL) {
		line++;
		if (buf[0] == '#') {
			continue;
		}
		sscanf(buf, "%s%s%s%s", arg[0], arg[1], arg[2], arg[3]);

		if (!strcmp(arg[0], "iface")) {
			bzero(curIface, sizeof(curIface));
			strcpy(curIface, arg[1]);
		}

		if (!strcmp(iface, curIface) && !strcmp(arg[0], param)) {
			fclose(fp);
			return arg[1];
		}

		bzero(buf, sizeof(buf));
		bzero(arg[0], sizeof(arg[0]));
		bzero(arg[1], sizeof(arg[1]));
		bzero(arg[2], sizeof(arg[2]));
		bzero(arg[3], sizeof(arg[3]));
	}

	fclose(fp);

	return NULL;
}

int RkIfaceSetParam(const char *iface, const char *param, const char *val)
{
	FILE *ptrIn, *ptrOut;
	char buf[128] = { 0 };
	char curIface[128] = { 0 };
	int flag = 0; 

	ptrIn = fopen(path, "r");
	ptrOut = fopen(tmp, "w+");
	if (!ptrIn || !ptrOut) {
		return -1;
	}

	while(fgets(buf, sizeof(buf), ptrIn)) {
		if (buf[0] == '#') {
			fputs(buf, ptrOut);
			continue;
		}

		if (!flag) {
			sscanf(buf, "%s%s%s%s", arg[0], arg[1], arg[2], arg[3]);
			if (!strcmp(arg[0], "iface")) {
				if (!strcmp(curIface, iface)) {
					fprintf(ptrOut, "%s %s\n", param, val);
					fputs(buf, ptrOut);
					flag = 1;
					continue;
				}
				bzero(curIface, sizeof(curIface));
				strcpy(curIface, arg[1]);
				fputs(buf, ptrOut);
			} else if (!strcmp(arg[0], param)) {
				fprintf(ptrOut, "%s %s\n", param, val);
				flag = 1;
			} else {
				fputs(buf, ptrOut);
			}
		} else {
			fputs(buf, ptrOut);
			continue;
		}
	}

	fclose(ptrIn);
	fclose(ptrOut);
	remove(path);
	rename(tmp, path);

	return 0;
}
