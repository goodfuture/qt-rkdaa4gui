/*******************************************************************************
 * Copyright(C)	    : 2013 Rockontrol Industrial Park, Inc.
 * Version          : V1.0
 * Author           : KangQi
 * Tel              : 18636181581/6581
 * Email            : goodfuturemail@gmail.com
 * File Name        : rktrdmsg.cpp
 * Created At       : 2013-09-25 10:52
 * Last Modified    : 2013-11-20 16:04
 * Description      : 
 * History	        : 
*******************************************************************************/
#include <rktrdmsg.h>
#include <rktype.h>
#include <stdio.h>
#include <stdlib.h>

extern struct context ctx;

#define LCD_BACK_LIGHT_PATH "/sys/class/backlight/backlight/brightness"

void rkTrdMsg::procUICmd(int cmd)
{
	int rc;

	switch(cmd) {
		case UI_CMD_RELOAD_USR_CFG:
			rkSetReloadFlag();
			break;
		case UI_CMD_DISABLE_DTU:
			rkDisableDtu();
			break;
		case UI_CMD_ENABLE_DTU:
			rkEnableDtu();
			break;
		case UI_CMD_CORE_PROG_EXIT:
			rkReqCoreExit();
			break;
		case UI_CMD_QRY_DATABASE:
			rc = sqlite3_get_table(ctx.db, ctx.sql, &ctx.qryRst, &ctx.row, &ctx.col, &ctx.err);
			sqlite3_free(ctx.sql);
			if (rc || !ctx.row) {
				sqlite3_free_table(ctx.qryRst);
				sqlite3_close(ctx.db);
				ctx.query_flag &= ~QUERY_SUCCESS_BIT_MASK;
				ctx.query_flag |= QUERY_FINISH_BIT_MASK;
				return;
			}
			ctx.query_flag |= QUERY_SUCCESS_BIT_MASK; 
			ctx.query_flag |= QUERY_FINISH_BIT_MASK;
			break;
		default:
			break;
	}
}

void rkTrdMsg::run()
{
	int rc;
	FILE *fp;

	rkShmInit();

	while(1) {
		for (int i = 0; i < AI_NUM; i++) {
			rkShmGetData(TYPE_AI, i, &ctx.ai[i]);
		}

		for (int i = 0; i < EI_NUM; i++) {
			rkShmGetData(TYPE_EI, i, &ctx.ei[i]);
		}

		for (int i = 0; i < DI_NUM; i++) {
			rkShmGetData(TYPE_DI, i, &ctx.dip[i]);
			//printf("di%d->inuse = %d, code = %s, val = %d\n", i, ctx.dip[i].inuse, ctx.dip[i].code, ctx.dip[i].val);
		}

		for (int i = 0; i < DO_NUM; i++) {
			rkShmGetData(TYPE_DO, i, &ctx.dop[i]);
			//printf("do%d->inuse = %d, code = %s, val = %d\n", i, ctx.dop[i].inuse, ctx.dop[i].code, ctx.dop[i].val);
		}

		rc = access("/proc/scsi/usb-storage", F_OK); 
		if (!rc) {
			ctx.usbflag = 1;
		} else {
			ctx.usbflag = 0;
		}

		fp = fopen(LCD_BACK_LIGHT_PATH, "w");
		if (fp) {
			if (ctx.scrflag == 2 && fp) {
				fprintf(fp, "255");
			} else if (ctx.scrflag = 1) {
				fprintf(fp, "128");
			} else {
				fprintf(fp, "0");
			}
			fclose(fp);
		}

		rkPutGuiVersion(GUI_VERSION);
		if (rkIsCoreAlive()) {
			emit sendCoreStatus(true, rkGetCoreVersion());
			//printf("version = %s\n", rkGetCoreVersion());
		} else {
			emit sendCoreStatus(false, NULL);
		}

		sleep(3);
	}
}
