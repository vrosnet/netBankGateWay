/****************************************
 *������:trans482101.c
 *��  ��:���Žɷ�ǰ��ѯ
 *��  ��:2007.05.21
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


int ics_proc_482101(char *send_buff,char *recv_buff)
{
  /*˵����send_buff-�ϴ����ģ�
          recv_buff-�´�����;
          ics_send_buff-���̨ͨѶ�ķ��ʹ�;
          ics_recv_buff-���̨ͨѶ�Ľ��մ�;
 */

  int			i;	
  int 			len;
  int                   ret;
  int			offset;
  int			i_biz_id;

  ICS_DEF_TIA		*pICS_TIA;
  ICS_DEF_TOA		*pICS_TOA;
  ICS_DEF_482101_I	*pICS_482101_I;
  ICS_DEF_482101_N	*pICS_482101_N;
  ICS_DEF_482101_E	*pICS_482101_E;

  char			ics_send_buff[LEN_ICS_PROC_BUF];
  char			ics_recv_buff[LEN_ICS_PROC_BUF];
  char			ics_482101i_buff[38];
  char			ics_482101n_buff[118];
  char			ics_482101e_buff[75];
  char			ics_tia_buff[171];
  char			ics_toa_buff[114];
  char 			tmp_val_str[LEN_TMP_VAL_STR];
  char 			tmp_val_str2[LEN_TMP_VAL_STR];
  char 			tmp_val_str3[LEN_TMP_VAL_STR];
  char 			display_str[LEN_ICS_PROC_BUF];
  char                  tmpvalue[40]; 	/*���ϴ�������ȡ�õ�ĳ��ֵ*/
  char			sLen[8];
  char			sLeft[14];
  char			sRight[3];
  char			ics_port[6];

  FILE 			*fp;

  /*-------------------STEP1:ͨѶǰ����-����ϴ���--------------------*/

  /* STEP1-1:�����ṹ�ͱ��� */

        pICS_482101_I=(ICS_DEF_482101_I *)ics_482101i_buff;
        pICS_482101_N=(ICS_DEF_482101_N *)ics_482101n_buff;
        pICS_482101_E=(ICS_DEF_482101_E *)ics_482101e_buff;
        pICS_TIA=(ICS_DEF_TIA *)ics_tia_buff;
        pICS_TOA=(ICS_DEF_TOA *)ics_toa_buff;

	memset(ics_send_buff,'\0',sizeof(ics_send_buff));
	memset(ics_recv_buff,'\0',sizeof(ics_recv_buff));
	memset(ics_482101i_buff,'\0',sizeof(ics_482101i_buff));
	memset(ics_482101n_buff,'\0',sizeof(ics_482101n_buff));
	memset(ics_482101e_buff,'\0',sizeof(ics_482101e_buff));
	memset(ics_tia_buff,'\0',sizeof(ics_tia_buff));
	memset(ics_toa_buff,'\0',sizeof(ics_toa_buff));

        memset(tmp_val_str,'\0',sizeof(tmp_val_str));
        memset(tmp_val_str2,'\0',sizeof(tmp_val_str2));
        memset(tmp_val_str3,'\0',sizeof(tmp_val_str3));
        memset(display_str,'\0',sizeof(display_str));
        memset(tmpvalue,'\0',sizeof(tmpvalue));
  
flog( STEP_LEVEL,"--482101 ����[%s]-------------------------------",send_buff);

 /* ע�⣺���������ð��սṹ�����Ⱥ�˳������������ݸ������� */
  /* STEP1-2:���ϴ����Ĺ̶�ͷ */
  strcpy(pICS_TIA->CCSCod,"TLU6");   /* CICS���״��� */
  strcpy(pICS_TIA->TTxnCd,"482101");
  strcpy(pICS_TIA->FeCod,"482101");
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

  /* STEP1-3: ���ϴ����еĹ̶�Ԫ��ֵ*/
  strcpy(pICS_482101_I->RsFld1,"P001"); /*������������(��ѯ)*/

  /* STEP1-4: ���ϴ������л������ֵ-��getValueOf����*/
  getValueOfStr(send_buff,"biz_id",tmpvalue); /*������ҵ������*/
  i_biz_id = atoi(tmpvalue);
  switch(i_biz_id)
  {
    case 1:	/*���л���*/
	strcpy(pICS_482101_I->RsFld2,"0001");
	break;
    case 2:	/*С��ͨԤ���ѳ�ֵ*/
	strcpy(pICS_482101_I->RsFld2,"0064");
	break;
    case 3:	/*����16300������*/
	strcpy(pICS_482101_I->RsFld2,"0010");
	break;
    case 4:	/*����ADSL������*/
	strcpy(pICS_482101_I->RsFld2,"0013");
	break;
    case 5:	/*����Ԥ�ɷѿ���ֵ*/
	strcpy(pICS_482101_I->RsFld2,"0046");
	break;
    case 6:	/*���ſ���Ԥ�ɷ�*/
	strcpy(pICS_482101_I->RsFld2,"0071");	
	break;
	  case 22:	/*����CDMA�ɷ�*/
	strcpy(pICS_482101_I->RsFld2,"0241");
	break;
		case 23:	/*����CDMAԤ�ɷ�*/
	strcpy(pICS_482101_I->RsFld2,"0242");
	break;
    default:
	break;
  }
  memset(tmpvalue,'\0',sizeof(tmpvalue));
  getValueOfStr(send_buff,"CTSQ",tmpvalue); /*�ɷѺ���*/
  strcpy(pICS_482101_I->TCusID,tmpvalue);

  /*STEP1-4:�ѽṹ�еĽ������滻Ϊ�ո��ϴ���ĩβ�ӽ�����.*/
  len=sizeof(ICS_DEF_TIA);
  for(i=0;i<len;i++)
  {
  	if(ics_tia_buff[i]==0)
   		ics_tia_buff[i]=' ';
  }

  /*����ICS���8λ���ĳ���*/
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

  len=sizeof(ICS_DEF_482101_I);
  for(i=0;i<len;i++)
  {
  	if(ics_482101i_buff[i]==0)
   		ics_482101i_buff[i]=' ';
  }
  memcpy(ics_send_buff+offset,ics_482101i_buff,len);
  offset=offset+sizeof(ICS_DEF_482101_I);

  /*����ICS���8λ���ĳ�����ͷ*/
  memcpy(sLen,'\0',8);
  sprintf(sLen,"%08d",offset-8);
  memcpy(ics_send_buff,sLen,8);

  ics_send_buff[offset] = '\0';  

  /*----------------------STEP2:���м�ҵ��ǰ�û�ͨѶ-----------------*/
   /* ��ICSͨѶ */
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

  /*--------------------STEP3:ͨѶ����:��ɻش�����------------------*/

  if(memcmp(pICS_TOA->RspCod,"000000",sizeof(pICS_TOA->RspCod))==0)/*�ɹ�*/
  {
  /* STEP3-1����ҳ����ʾҪ��: ��������д���ֶΣ�������ҳ������ʾ���ֶ� */
  /* ע�⣬<br>��ҳ����ʾ�Ļ��з��� */

	memcpy(pICS_482101_N,ics_recv_buff+sizeof(ICS_DEF_TOA),sizeof(ICS_DEF_482101_N));

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482101_N->TCusID,sizeof(pICS_482101_I->TCusID));

	sprintf(tmp_val_str,"�绰���룺 %s  <br>",tmp_val_str2);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482101_N->TCusNm,2);
	memcpy(tmp_val_str3,pICS_482101_N->TCusNm+4,sizeof(pICS_482101_N->TCusNm)-4);
	sprintf(tmp_val_str,"�ͻ������� %s**%s  <br>",tmp_val_str2,tmp_val_str3);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482101_N->TxnAmt,sizeof(pICS_482101_N->TxnAmt));
	memset(sLeft,0,sizeof(sLeft));
	memset(sRight,0,sizeof(sRight));
	memcpy(sLeft,tmp_val_str2,13);
	memcpy(sRight,tmp_val_str2+13,2);
	if(i_biz_id==2 | i_biz_id==23)
	    sprintf(tmp_val_str,"������ %d.%s  <br>",atoi(sLeft),sRight);
	else
	    sprintf(tmp_val_str,"�ɷѽ� %d.%s  <br><br>",atoi(sLeft),sRight);
	strcat(display_str,tmp_val_str);
	strcat(display_str,"��ܰ��ʾ�� ȷ�Ͻɷ��ύ�󣬽��޷��˿����ϸ�˶����Ľɷ���Ϣ!<br>");

	/* ����setValueOf������� */
    	setValueOfStr(recv_buff,"display_zone",display_str);

 /* STEP3-2 ����ҳ������Ҫ��: ������д���ֶΣ�������ҳ����Ϊ����input��Ҫ�� */
 /* ע�⣬1.�������ò�����˳���������ݿ��в�����˳��һ�£�����س����� 
          2.�������ֶ�������ע����WEB��һ��*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
	setValueOfStr(recv_buff,"MGID",tmp_val_str); /*������*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_482101_N->TxnAmt,sizeof(pICS_482101_N->TxnAmt));
	setValueOfStr(recv_buff,"AMT1",tmp_val_str);/*�ɷ��ܽ��*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_482101_I->TCusID,sizeof(pICS_482101_I->TCusID));
	setValueOfStr(recv_buff,"CTSQ",tmp_val_str);/*�绰����*/ 

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,"######",6);
	setValueOfStr(recv_buff,"passWord",tmp_val_str);/*�绰����*/ 
	
	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_482101_N->TCusNm,sizeof(pICS_482101_N->TCusNm));
	setValueOfStr(recv_buff,"CusNm",tmp_val_str);/*�ͻ�����*/

  }
  else
  {	/*ʧ��*/
	memcpy(pICS_482101_E,ics_recv_buff+sizeof(ICS_DEF_TOA),sizeof(ICS_DEF_482101_E));
	strcpy(tmp_val_str,"<font color=ff0000>--->����ʧ��</font><br>");
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
	sprintf(tmp_val_str,"--->ʧ��ԭ��[%s] ",tmp_val_str2);
	strcat(display_str,tmp_val_str);

        memset(tmp_val_str2,0,sizeof(tmp_val_str2));
	memcpy(tmp_val_str2,pICS_482101_E->RspMsg,sizeof(pICS_482101_E->RspMsg));
	sprintf(tmp_val_str,"[%s] <br>",tmp_val_str2);
	strcat(display_str,tmp_val_str);

    	setValueOfStr(recv_buff,"display_zone",display_str);

	memset(tmp_val_str,'\0',sizeof(tmp_val_str));
	memcpy(tmp_val_str,pICS_TOA->RspCod,sizeof(pICS_TOA->RspCod));
 	setValueOfStr(recv_buff,"MGID",tmp_val_str);/*������*/
  }
flog( STEP_LEVEL,"**482101 ����[%s]******************************",recv_buff);

  return 0;
}