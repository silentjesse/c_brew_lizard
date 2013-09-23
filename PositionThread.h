#ifndef POSITIONTHREAD_H
#define POSITIONTHREAD_H

 
#include "GlobalVariable.h"
#include "Resource.h"
#include "BREWVersion.h"
#include "PositionUploadUI.h"





#define INHERIT_IPositionThread(iname)\
	int (*pFNStart)(iname*);\
	int (*pFNRun)(iname* p);\
	int (*pFNStop)(iname* p);\
	void (*pFNShowOnUI)(iname* p);\
	int (*pFNRelease)(iname*)


AEEINTERFACE_DEFINE(IPositionThread);

#define  IPOSITIONTHREAD_Start( p)\
	AEEGETPVTBL(p, IPositionThread)->pFNStart( p)

#define  IPOSITIONTHREAD_Stop( p)\
	AEEGETPVTBL(p, IPositionThread)->pFNStop( p)

#define  IPOSITIONTHREAD_Run( p)\
	AEEGETPVTBL(p, IPositionThread)->pFNRun( p)

#define  IPOSITIONTHREAD_ShowOnUI( p)\
	AEEGETPVTBL(p, IPositionThread)->pFNShowOnUI( p)
 

#define  IPOSITIONTHREAD_Release(p)\
	AEEGETPVTBL(p, IPositionThread)->pFNRelease(p)


struct IPositionThread{
	AEEVTBL(IPositionThread) *pvt; 
	AEECLSID clsID;
	IPosDet* posdet;
	IGlobalVariable* glbVar; 
	IPositionUI ** pPosUI;//used to show the position info on the posui
	
	
	/* Private members to work with IPosDet */
	AEECallback cbIntervalTimer;
	AEECallback cbPos;
	AEEGPSInfo gpsinfo;

	AEECallback cbStopTimer;
	

	/*assistant flag*/
	boolean bInTrackNotification;//the thread is in the notification function.
	boolean bSetForCancellation; /* Track is meant to be cancelled. do so when it is safe. */
	boolean bInProgress;         /* when tracking is in progress. */
	 boolean     bModeLocal;

	/* Clients response members. */
	AEECallback *pcbResp;
	PositionData *pRespPosData;
	/**************************/

	
};







int IPositionThread_New(
					IGlobalVariable* glbVar,  
					AEECLSID clsID,  
					void ** iPosThd,
					IPositionUI ** posUI);




/************************************************************************/
/*  the functions about track                                                                    */
/************************************************************************/
int Track_init(IPositionThread * ipt);
int Track_start(IPositionThread * ipt);
void Track_cbInterval(IPositionThread * ipt);
int Track_Stop( IPositionThread * ipt );
void Track_cbInfo(IPositionThread * ipt);
 
void Track_Local( IPositionThread * ipt );
void Track_Network(IPositionThread * ipt);
#endif