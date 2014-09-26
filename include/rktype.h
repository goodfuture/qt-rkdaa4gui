#ifndef _RK_TYPE_H_
#define _RK_TYPE_H_

#include "rkcom.h"
#include "rkshm.h"
#include <stdint.h>
#include "sqlite3.h"

#define GUI_VERSION	"2.1.2"

#define ARG_CFG_DIR_OFFSET 0
#define ARG_DYN_LIB_OFFSET 1
#define ARG_RTD_DB_PATH_OFFSET 2
#define ARG_MSG_DB_PATH_OFFSET 3

#ifdef __ARM__
#define IMG_DIR "./pics"
#else
#define IMG_DIR "./pics/x86"
#endif

typedef char *(*Func)(void);

#define QUERY_FINISH_BIT_MASK	0x01
#define QUERY_SUCCESS_BIT_MASK	0x02

typedef struct context {
	char *cliarg[5];
	uint8_t usbflag;
	uint8_t scrflag;
	struct rtdInfo ai[AI_NUM];
	struct rtdInfo ei[EI_NUM];
	struct dioInfo dip[DI_NUM];
	struct dioInfo dop[DO_NUM];
	uint8_t	query_flag;
	sqlite3 *db;
	char	*sql;
	char	**qryRst;
	int		row;
	int		col;
	char	*err;
} context_t;

#endif /* _RK_TYPE_H_ */
