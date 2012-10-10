/****************************************
 *程序名:trans482111.c
 *功  能:联通缴费前查询
 *日  期:2008.01.11
 ****************************************/

#include        <stdio.h>
#include        <stdlib.h>
#include        <memory.h>
#include        <signal.h>
#include        <sys/types.h>
#include        <time.h>
#include        <errno.h>
#include        "trans_macro.h"
#include        "trans_struct.h"
#include        "netbank_mid.h"
#include        "error.h"


int ics_proc_482111(char *send_buff,char *recv_buff)
{
  /*说明：send_buff-上传报文；
          recv_buff-下传报文;
          ics_send_buff-与后台通讯的发送串;
          ics_recv_buff-与后台通讯的接收串;
 */

  int			i;	
  int 			len;
  int                   ret;
  int			offset;
  int			i_biz_id;

  ICS_DEF_TIA		*pICS_TIA;
  ICS_DEF_TOA		*pICS_TOA;
  ICS_DEF_482111_I	*pICS_482111_I;
  ICS_DEF_482111_N	*pICS_482111_N;
  ICS_DEF_482111_E	*pICS_482111_E;

  char			ics_send_buff[LEN_ICS_PROC_BUF];
  char			ics_recv_buff[LEN_ICS_PROC_BUF];
  char			ics_482111i_buff[38];
  char			ics_482111n_buff[118];
  char			ics_482111e_buff[75];
  char			ics_tia_buff[171];
  char			ics_toa_buff[114];
  char 			tmp_val_str[LEN_TMP_VAL_STR];
  char 			tmp_val_str2[LEN_TMP_VAL_STR];
  char 			tmp_val_str3[LEN_TMP_VAL_STR];
  char 			display_str[LEN_ICS_PROC_BUF];
  char                  tmpvalue[40]; 	/*从上传报文中取得的某项值*/
  char                  tmp1[10]; 	
  char			sLen[8];
  char			sLeft[14];
  char			sRight[3];
  char			ics_port[6];

  FILE 			*fp;

  /*-------------------STEP1:通讯前处理-组成上传串--------------------*/

  /* STEP1-1:清理结构和变量 */

        pICS_482111_I=(ICS_DEF_482111_I *)ics_482111i_buff;
        pICS_482111_N=(ICS_DEF_482111_N *)ics_482111n_buff;
        pICS_482111_E=(ICS_DEF_482111_E *)ics_482111e_buff;
        pICS_TIA=(ICS_DEF_TIA *)ics_tia_buff;
        pICS_TOA=(ICS_DEF_TOA *)ics_toa_buff;

	memset(ics_send_buff,'\0',sizeof(ics_send_buff));
	memset(ics_recv_buff,'\0',sizeof(ics_recv_buff));
	memset(ics_482111i_buff,'\0',sizeof(ics_482111i_buff));
	memset(ics_482111n_buff,'\0',sizeof(ics_482111n_buff));
	memset(ics_482111e_buff,'\0',sizeof(ics_482111e_buff));
	memset(ics_tia_buff,'\0',sizeof(ics_tia_buff));
	memset(ics_toa_buff,'\0',sizeof(ics_toa_buff));

        memset(tmp_val_str,'\0',sizeof(tmp_val_str));
        memset(tmp_val_str2,'\0',sizeof(tmp_val_str2));
        memset(tmp_val_str3,'\0',sizeof(tmp_val_str3));
        memset(display_str,'\0',sizeof(display_str));
        memset(tmpvalue,'\0',sizeof(tmpvalue));
  
flog( STEP_LEVEL,"--482111 接收[%s]-------------------------------",send_buff);

 /* 注意：填充数据最好按照结构定义先后顺序，以免出现数据覆盖问题 */
  /* STEP1-2:填上传串的固定头 */
  strcpy(pICS_TIA->CCSCod,"TLU6");   /* CICS交易代码 */
  strcpy(pICS_TIA->TTxnCd,"482111");
  strcpy(pICS_TIA->FeCod,"482111");
  strcpy(pICS_TIA->TrmNo,"DVID");
  strcpy(pICS_TIA->TxnSrc,"T0001");   
  strcpy(pICS_TIA->NodTrc,"200704100044191");
  strcpy(pICS_TIA->TlrId,"AFAM020");
  strcpy(pICS_TIA->TIATyp,"T");
  strcpy(pICS_TIA->AthLvl,"00");
  strcpy(pICS_TIA->Sup1Id,"       ");
  strcpy(pICS_TIA->Sup2Id,"       ");
  strcpy(pICS_TIA->Sup1Pw,"      ");
  strcpy(pICS_TIA->Sup2Pw,"      ");
  strcpy(pICS_TIA->Sup1Dv," ");
  strcpy(pICS_TIA->Sup2Dv," ");
  strcpy(pICS_TIA->AthTbl,"                                                            ");
  strcpy(pICS_TIA->AthLog," ");
  strcpy(pICS_TIA->HLogNo,"         ");
  strcpy(pICS_TIA->CprInd,"0");
  strcpy(pICS_TIA->EnpInd,"0");
  strcpy(pICS_TIA->NodNo,"444086");
  strcpy(pICS_TIA->OprLvl," ");
  strcpy(pICS_TIA->TrmVer,"v0000001");
  strcpy(pICS_TIA->OutSys," ");
  strcpy(pICS_TIA->Fil,"  ");

  /* STEP1-3: 填上传串中的固定元素值*/
  strcpy(pICS_482111_I->RsFld1,"P001"); /*第三方交易码(查询)*/

  /* STEP1-4: 从上传报文中获得其余值-用getValueOf函数*/
  getValueOfStr(send_buff,"biz_id",tmpvalue); /*第三方业务类型*/
  i_biz_id = atoi(tmpvalue);

  strcpy(pICS_482111_I->RsFld2,"0049");

  memset(tmpvalue,'\0',sizeof(tmpvalue));
  getValueOfStr(send_buff,"CTSQ",tmpvalue); /*缴费号码*/
  strcpy(pICS_482111_I->TCusID,tmpvalue);

  memset(tmpvalue,'\0',sizeof(tmpvalue));
  getValueOfStr(send_buff,"PSWD",tmpvalue); /*此域充当充值缴费标志0欠费,1充值*/
  memset( tmp1, '\0', sizeof( tmp1 ) ) ;
  memcpy( tmp1, tmpvalue, 4 ) ;
  sprintf( pICS_482111_I->Feeflg, "%1d", atoi( tmp1 ) ) ;

  memset( tmp1, '\0', sizeof( tmp1 ) ) ;
  memcpy( tmp1, tmpvalue + 4, 2 ) ;
  strcpy( pICS_482111_I->Feefld, tmp1 ) ; /*联通业务类型*/

  /*STEP1-4:把结构中的结束符替换为空格，上传串末尾加结束符.*/
  len=sizeof(ICS_DEF_TIA);
  for(i=0;i<len;i++)
  {
  	if(ics_tia_buff[i]==0)
   		ics_tia_buff[i]=' ';
  }

  /*发往ICS需加8位报文长度*/
  offset=0;
  offset=offset+8;

  len=sizeof(ICS_DEF_TIA);
  for(i=0;i<len;i++)
  {
        if(ics_tia_buff[i]==0)
                ics_tia_buff[i]=' ';
  }
  memcpy(ics_send_buff+offset,ics_tia_buff,len);
  offset=offset+sizeof(ICS_DEF_TIA);

  len=sizeof(ICS_DEF_482111_I);
  for(i=0;i<len;i++)
  {
  	if(ics_482111i_buff[i]==0)
   		ics_482111i_buff[i]=' ';
  }
  memcpy(ics_send_buff+offset,ics_482111i_buff,len);
  offset=offset+sizeof(ICS_DEF_482111_I);

  /*发往ICS需加8位报文长度在头*/
  memcpy(sLen,'\0',8);
  sprintf(sLen,"%08d",offset-8);
  memcpy(ics_send_buff,sLen,8);

  ics_send_buff[offset] = '\0';  

  /*----------------------STEP2:与中间业务前置机通讯-----------------*/
   /* 与ICS通讯 */
   memset(ics_port,'\0',sizeof(ics_port));

   ret = get_config_value(CONFIG_FILE_NAME, "ICS_PORT_TRANS", ics_port);
   if (ret != RETURN_OK)
   {
	return -2;
   }

   ret=clientics( ics_send_buff,ics_recv_buff, atoi(ics_port) );
   if(ret != RETURN_OK)
	return -1;

    memcpy(pICS_TOA,ics_recv_buff,sizeof(ICS_DEF_TOA));

  /*--------------------STEP3:通讯后处理:组成回传报文------------------*/

  if(memcmp(pICS_TOA->RspCod,"000000",sizeof(pICS_TOA->RspCod))==0)/*成功*/
  {
  /* STEP3-1处理页面显示要素: 在这里填写的字段，就是在页面上显示的字段 */
  /* 注意，<br>是页面显示的换行符号 */

	memcpy(pICS_482111_N,ics_recv_buff+sizeof(ICS_DEF_TOA),sizeof(ICS_DEF_482111_N));

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482111_N->TCusID,sizeof(pICS_482111_I->TCusID));

	sprintf(tmp_val_str,"电话号码： %s  <br>",tmp_val_str2);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482111_N->TCusNm,2);
	memcpy(tmp_val_str3,pICS_482111_N->TCusNm+4,sizeof(pICS_482111_N->TCusNm)-4);
	sprintf(tmp_val_str,"客户姓名： %s##%s  <br>",tmp_val_str2,tmp_val_str3);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482111_N->TxnAmt,sizeof(pICS_482111_N->TxnAmt));
	memset(sLeft,0,sizeof(sLeft));
	memset(sRight,0,sizeof(sRight));
	memcpy(sLeft,tmp_val_str2,10);
	memcpy(sRight,tmp_val_str2+10,2);
	if( memcmp( pICS_482111_I->Feeflg, "1", 1 ) == 0 )
	    sprintf(tmp_val_str,"话费余额： %d.%s  <br>",atoi(sLeft),sRight);
	else
	    sprintf(tmp_val_str,"缴费金额： %d.%s  <br><br>",atoi(sLeft),sRight);
	strcat(display_str,tmp_val_str);
	strcat(display_str,"温馨提示： 请仔细核对您的缴费号码，以免缴错后无法退款！ <br>");

	/* 调用setValueOf函数填充 */
    	setValueOfStr(recv_buff,"display_zone",display_str);

 /* STEP3-2 处理页面隐含要素: 这里填写的字段，就是在页面作为隐含input的要素 */
 /* 注意，1.这里设置参数的顺序必须和数据库中参数的顺序一致，否则回出错！ 
          2.返回码字段名定义注意与WEB上一致*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
	setValueOfStr(recv_buff,"MGID",tmp_val_str); /*返回码*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
        memset(tmpvalue,'\0',sizeof(tmpvalue));
	memcpy(tmpvalue,pICS_482111_N->TxnAmt,sizeof(pICS_482111_N->TxnAmt));
	sprintf( tmp_val_str, "%015s", trim( tmpvalue ) ) ;
	setValueOfStr(recv_buff,"AMT1",tmp_val_str); /*缴费总金额*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_482111_I->TCusID,sizeof(pICS_482111_I->TCusID));
	setValueOfStr(recv_buff,"CTSQ", trim( tmp_val_str ) ) ; /*电话号码*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_482111_N->Arefld,2);
	memcpy(tmp_val_str+2,pICS_482111_N->BusId,20);
	setValueOfStr(recv_buff,"passWord",tmp_val_str); /*Arefld+BusId*/ 

  }
  else
  {	/*失败*/
	memcpy(pICS_482111_E,ics_recv_buff+sizeof(ICS_DEF_TOA),sizeof(ICS_DEF_482111_E));
	strcpy(tmp_val_str,"<font color=ff0000>--->交易失败</font><br>");
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
	sprintf(tmp_val_str,"--->失败原因：[%s] ",tmp_val_str2);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482111_E->RspMsg,sizeof(pICS_482111_E->RspMsg));
	sprintf(tmp_val_str,"[%s] <br>",tmp_val_str2);
	strcat(display_str,tmp_val_str);

    	setValueOfStr(recv_buff,"display_zone",display_str);

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
 	setValueOfStr(recv_buff,"MGID",tmp_val_str);/*返回码*/
  }
flog( STEP_LEVEL,"**482111 返回[%s]******************************",recv_buff);

  return 0;
}
