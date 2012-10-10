/* 本文件用于定义网关系统中用到的参数 */

#ifndef _NETBANK_H
#define _NETBANK_H

/* 读取配置文件时所用变量 */
#define MESSAGE_LENGTH		10240
#define CONFIG_FILE_NAME	"/home/netbank_mid/config/netbank.cfg"
#define	LOG_LEVEL		6
#define BUF_LEVEL               0
#define LOG_FILE_NAME           "./netbank.log"
#define BUF_FILE_NAME           "./netbank_buf.log"


/* 通讯定义 珠海版本不用这里的定义 */
#define MID_ADD			"182.39.6.211" 
#define MID_PORT	 	3000	

/* 数据库 */
#define SQLNOTFOUND		100

/* 日志级别 */
#define ERROR_LEVEL		0
#define STEP_LEVEL		2
#define INFO_LEVEL		4
#define DEBUG_LEVEL		6

/* 交易类型 */
#define QUERY_TRANSACTION	1
#define TRANSFER_TRANSACTION	0

/* 报文类型 */
#define REQUEST_DG		0
#define RESPONSE_DG		1

/* 落地处理标识 */
#define NEED_SUPPLY		1			/* 需要落地处理 */
#define NEED_NOT_SUPPLY		0			/* 无需落地处理 */

/* 日志记录方向 */
#define WRITE_TO_TDQ		1			/* 写到TDQ中 */
#define WRITE_TO_FILE		0			/* 写到文件中 */

/* 写流水标识 */
#define JNL_WRITED		1			/* 已记录流水 */
#define JNL_NOT_WRITED		0			/* 未记录流水 */

/* 交易缓存标识 */
#define TRANSACTION_SAVED	1			/* 交易被缓存 */
#define TRANSACTION_NOT_SAVED	0			/* 交易不被缓存 */

#define TIMEOUT			120			/* 通讯超时 */

#endif
