
#ifndef APPSTATE_H
#define  APPSTATE_H

#include "Resource.h"
#include "AEEComdef.h"
#include "AEEPosDet.h"
enum _eAppState
{
	APP_STATE_INIT,//初始化
	APP_STATE_MAINUI,//主界面
	APP_STATE_POSUI,
	BACKGROUND//后台
} ;


#define MIN_POSDET_INTERVAL 30//位置请求之间的最小时间间隔,以秒为单位

enum _eEVTCommand
{
	EVT_COMMAND_SHOWMAINUI,//显示主界面
	EVT_COMMAND_LOCATE,//定位
	EVT_COMMAND_LOCATE_RESTART//重启定位

} ;

typedef enum _eAppState EAppState;

enum _eTrackType
{
	TRACKTYPE_LOCAL,
	TRACKTYPE_NETWORK
};

typedef enum _eTrackType ETrackType;



 


/************************************************************************/
/* 用于全局的，将定位的数据传出来给界面                                                               */
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
	boolean		bLocateStartOrStop;/*true为用户点击了开始定位按钮,false为用户点击了停止定位按钮*/
	AEEGPSMode  gpsMode;//定位类型
}PositionData;






#define INHERIT_IGlobalVariable(iname)\
	uint32 (*pFNRelease)(iname*)


AEEINTERFACE_DEFINE(IGlobalVariable);


#define  IGLOBALVARIABLE_Release(p)\
	AEEGETPVTBL(p, IGlobalVariable)->pFNRelease(p)

/************************************************************************/
/* 用于保存一些全局共用的变量                                         */
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