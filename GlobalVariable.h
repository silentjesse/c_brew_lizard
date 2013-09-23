
#ifndef APPSTATE_H
#define  APPSTATE_H

#include "Resource.h"
#include "AEEComdef.h"
#include "AEEPosDet.h"
enum _eAppState
{
	APP_STATE_INIT,//��ʼ��
	APP_STATE_MAINUI,//������
	APP_STATE_POSUI,
	BACKGROUND//��̨
} ;


#define MIN_POSDET_INTERVAL 30//λ������֮�����Сʱ����,����Ϊ��λ

enum _eEVTCommand
{
	EVT_COMMAND_SHOWMAINUI,//��ʾ������
	EVT_COMMAND_LOCATE,//��λ
	EVT_COMMAND_LOCATE_RESTART//������λ

} ;

typedef enum _eAppState EAppState;

enum _eTrackType
{
	TRACKTYPE_LOCAL,
	TRACKTYPE_NETWORK
};

typedef enum _eTrackType ETrackType;



 


/************************************************************************/
/* ����ȫ�ֵģ�����λ�����ݴ�����������                                                               */
/************************************************************************/
typedef struct {
	int    nErr;      /* SUCCESS or AEEGPS_ERR_* */
	uint32 dwFixNum;  /* fix number in this tracking session. */
	double lat;       /* latitude on WGS-84 Geoid */
	double lon;       /* longitude on WGS-84 Geoid */
	short  height;    /* Height from WGS-84 Geoid */
	AEEGPSServer server;
	AEEGPSQos qos;
	AEEGPSOpt optim;
	AECHAR gpsPosHintBuffer[MAX_RES_STRING_BUF_SIZE];

	
	boolean     bInNotification;     /* When the state machine is notifying the client */
	boolean     bSetForCancellation; /* Track is meant to be cancelled. do so when it is safe. */
	boolean     bInProgress;         /* when tracking is in progress. */
	boolean		bLocateStartOrStop;/*trueΪ�û�����˿�ʼ��λ��ť,falseΪ�û������ֹͣ��λ��ť*/
	AEEGPSMode  gpsMode;//��λ����
}PositionData;






#define INHERIT_IGlobalVariable(iname)\
	uint32 (*pFNRelease)(iname*)


AEEINTERFACE_DEFINE(IGlobalVariable);


#define  IGLOBALVARIABLE_Release(p)\
	AEEGETPVTBL(p, IGlobalVariable)->pFNRelease(p)

/************************************************************************/
/* ���ڱ���һЩȫ�ֹ��õı���                                         */
/************************************************************************/
 struct IGlobalVariable{
	VTBL(IGlobalVariable) *pvt; 
	EAppState appState;
	IShell * pIShell;
	IDisplay * pIDisplay;
	IResource * pIResouce;
	AEEDeviceInfo * pDevice; 
	PositionData * pPosData;
	AEECLSID ClsId;
};


int IGlobalVariable_New(IShell *, IDisplay *,
				AEEDeviceInfo  *device, void **);


#endif