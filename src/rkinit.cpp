#include "rkinit.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char *helpString = 
"Usage :\n"
" %s [options]\n"
"Options :\n"
" -C <dir>       Specify directory by 'dir' to store config files.\n"
"                Default use '%s'.\n"
" -R <path>      Specify the path of RTD database by 'path'.\n"
"                Default use '%s'.\n"
" -M <path>      Specify the path of MSG database by 'path'.\n"
"                Default use '%s'.\n"
" -L <dir>       Specify the dir of dynamic extension library by 'dir'.\n"
"                Default use '%s'.\n"
" -V             Show program version.\n"
" -d             Use debug mode.\n"
" -h             Get help information.\n";

void rkParseCliArg(struct context *ctx, int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "C:R:M:L:VHh")) != -1) {
		switch(opt) {
			case 'C': /* Directory Of Configure File */
				ctx->cliarg[ARG_CFG_DIR_OFFSET] = strdup(optarg);
				break;
			case 'R': /* RTD DataBase Storage Path */
				ctx->cliarg[ARG_RTD_DB_PATH_OFFSET] = strdup(optarg);
				break;
			case 'M': /* MSG DataBase Storage Path */
				ctx->cliarg[ARG_MSG_DB_PATH_OFFSET] = strdup(optarg);
				break;
			case 'L':
				ctx->cliarg[ARG_DYN_LIB_OFFSET] = strdup(optarg);
				break;
			case 'V':
				printf("version : %s\n", GUI_VERSION);
				exit(0);
			case 'h':
				printf(helpString, argv[0], DEF_CFG_FILE_DIR, DEF_RTD_DB_PATH, DEF_MSG_DB_PATH, DEF_DYN_LIB_DIR);
				exit(0);
			case 'd':
				break;
			default:
				printf("Use -h for a list of options.\n", opt);
				exit(0);
		}
	}

	if (!ctx->cliarg[ARG_CFG_DIR_OFFSET]) {
		ctx->cliarg[ARG_CFG_DIR_OFFSET] = DEF_CFG_FILE_DIR;
	}   

	if (!ctx->cliarg[ARG_RTD_DB_PATH_OFFSET]) { 
		ctx->cliarg[ARG_RTD_DB_PATH_OFFSET] = DEF_RTD_DB_PATH;
	}

	if (!ctx->cliarg[ARG_MSG_DB_PATH_OFFSET]) { 
		ctx->cliarg[ARG_MSG_DB_PATH_OFFSET] = DEF_MSG_DB_PATH;
	}

	if (!ctx->cliarg[ARG_DYN_LIB_OFFSET]) { 
		ctx->cliarg[ARG_DYN_LIB_OFFSET] = DEF_DYN_LIB_DIR;
	}   

	return;
}
