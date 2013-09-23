#include "PositionThread.h"







int IPositionThread_Start(IPositionThread * ipt){
	IPOSITIONTHREAD_Run(ipt);
	return AEE_SUCCESS;
}


int IPositionThread_Stop(IPositionThread * ipt){
	DBGPRINTF("IPositionThread_Stop==============================");
	return Track_Stop(ipt);
}


void IPositionThread_ShowOnUI(IPositionThread * ipt){
	if(ipt->glbVar->appState == APP_STATE_POSUI){
		IPOSITIONUI_UpdateUI(*(ipt->pPosUI));
	}

}




int IPositionThread_Run(IPositionThread * ipt){
	Track_init(ipt);
	Track_start(ipt);
	
	return AEE_SUCCESS;
}



int IPositionThread_Release(IPositionThread * ipt){
	if(ipt == NULL) return AEE_SUCCESS;
	FREEIF(ipt->pvt);
	IPOSDET_Release(ipt->posdet);
	//CALLBACK_Cancel(ipt->cbStopTimer);
	FREEIF(ipt);
	return AEE_SUCCESS;
}


 

int IPositionThread_New(
						IGlobalVariable* glbVar,  
						AEECLSID clsID,  
						void ** iPosThd,
						IPositionUI ** posUI
						){
	IPositionThread * posThd = (IPositionThread*)MALLOC(sizeof(IPositionThread));
	posThd->pvt = (VTBL(IPositionThread) * )MALLOC(sizeof(VTBL(IPositionThread)));

	ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_POSDET, (void**)&posThd->posdet) ;
	
	posThd->glbVar = glbVar;
	posThd->pvt->pFNStart = IPositionThread_Start;
	posThd->pvt->pFNStop = IPositionThread_Stop;
	posThd->pvt->pFNRun = IPositionThread_Run;
	posThd->pvt->pFNRelease = IPositionThread_Release;
	posThd->pvt->pFNShowOnUI  = IPositionThread_ShowOnUI;
	posThd->pRespPosData = posThd->glbVar->pPosData;
	posThd->bInProgress = FALSE;
	posThd->bInTrackNotification = FALSE;
	posThd->bSetForCancellation = TRUE;
	posThd->pPosUI = posUI;
	*iPosThd = posThd;
	return AEE_SUCCESS;
}

int Track_init(IPositionThread * ipt){
	CALLBACK_Init( &ipt->cbIntervalTimer, Track_cbInterval, ipt );
	CALLBACK_Init( &ipt->cbPos, Track_cbInfo, ipt );
	CALLBACK_Init(&ipt->cbStopTimer, Track_Stop, ipt);
	return AEE_SUCCESS;
}

void Track_Local( IPositionThread * ipt ){
	AEEGPSConfig config;
	
	ipt->bModeLocal = TRUE;
	

	(void) IPOSDET_GetGPSConfig(ipt->posdet, &config );

	config.mode = AEEGPS_MODE_TRACK_LOCAL;

	(void) IPOSDET_SetGPSConfig(ipt->posdet , &config );
	ipt->pRespPosData->gpsMode = AEEGPS_MODE_TRACK_LOCAL;
}


void Track_Network(IPositionThread * ipt){
	AEEGPSConfig config;
	
	DBGPRINTF( "TRACK NETWORK" );

	(void) IPOSDET_GetGPSConfig(ipt->posdet, &config );

	config.mode = AEEGPS_MODE_TRACK_NETWORK;

	(void) IPOSDET_SetGPSConfig(ipt->posdet , &config );
	
	ipt->bModeLocal = FALSE;
	ipt->pRespPosData->gpsMode = AEEGPS_MODE_TRACK_NETWORK;
}



int Track_start(IPositionThread * ipt){
	PositionData * pRespPosData = ipt->pRespPosData;
	int nErr = SUCCESS;
	DBGPRINTF("Track_start==============================");
	if( TRUE == ipt->bInProgress ) {
		nErr = EALREADY;
	} else {
		AEEGPSConfig config; 
		nErr = IPOSDET_GetGPSConfig(ipt->posdet, &config);

		ipt->pRespPosData->nErr = nErr;
		if(nErr != SUCCESS){
			switch(nErr){
				case SUCCESS :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"get GPS config sucessfully");
					break;
				case EPRIVLEVEL :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"cann't get GPS config : application does not have PL_POS_LOCATION privilege" );
					break;
				case EBADPARM  :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"cann't get GPS config:parameter specified is invalid  ");
					break;
				case EUNSUPPORTED :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"cann't get GPS config:this device does not support the gps function ");
					break;
				default:
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"cann't get GPS config:unknown ERROR");
					break;
			}
		}
		IPOSITIONTHREAD_ShowOnUI(ipt);
		
		
		ipt->bModeLocal = TRUE;
		pRespPosData->gpsMode = AEEGPS_MODE_TRACK_LOCAL;
		config.mode = AEEGPS_MODE_TRACK_LOCAL;
		config.nFixes = MAX_INT16; //estimated number of fixes  
		config.nInterval = MIN_POSDET_INTERVAL;  //estimated interval in seconds 
		config.server.svrType = AEEGPS_SERVER_DEFAULT;
		
		nErr = IPOSDET_SetGPSConfig( ipt->posdet, &config );
		
		WSPRINTF(pRespPosData->gpsPosHintBuffer,
			MAX_RES_STRING_BUF_SIZE,
			L"try to track with the method MSB");
		IPOSITIONTHREAD_ShowOnUI(ipt);
		
		if( nErr == EUNSUPPORTED) {
				
			/* As TRACK_LOCAL is unsupported on certain devices. 
			**   change it network based tracking. */
			WSPRINTF(pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"this device does not support track mode MSB, try with MSA");
			IPOSITIONTHREAD_ShowOnUI(ipt);
			
			 ipt->bModeLocal = FALSE;
			config.mode = AEEGPS_MODE_TRACK_NETWORK; 
			 pRespPosData->gpsMode = AEEGPS_MODE_TRACK_NETWORK;
			nErr = IPOSDET_SetGPSConfig(ipt->posdet, &config);
		}

		ipt->pRespPosData->nErr = nErr;
		


		if( nErr == SUCCESS ) {
			WSPRINTF(pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"config GPS engine sucessfully");
			ipt->bInProgress = TRUE;
			ipt->bSetForCancellation = FALSE;
			ipt->pRespPosData->bLocateStartOrStop = TRUE;
			IPOSITIONTHREAD_ShowOnUI(ipt);
			Track_cbInterval( ipt );
		}else{
			ipt->bSetForCancellation = TRUE;
			ipt->bInProgress = FALSE;
			ipt->pRespPosData->bLocateStartOrStop = FALSE;
			switch(nErr){
				case EPRIVLEVEL :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"fail to config GPS engine: application does not have PL_POS_LOCATION privilege");
					break;
				case EBADPARM  :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"fail to config GPS engine:parameter specified is invalid ");
					break;
				case EUNSUPPORTED :
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"fail to config GPS engine:this device does not support the gps function");
					break;
				default:
					WSPRINTF(pRespPosData->gpsPosHintBuffer,
						MAX_RES_STRING_BUF_SIZE,
						L"fail to config GPS engine: unknown error ");
					break;
			}
			
			 IPOSITIONTHREAD_ShowOnUI(ipt);
		}
		

	}
	return nErr; 
}





void Track_cbInterval(IPositionThread * ipt){
	
	int ret;
	PositionData * pRespPosData = ipt->pRespPosData;
	/* Cancel if it was deferred. */
	if( TRUE == ipt->bSetForCancellation ) {
		ipt->bInTrackNotification = FALSE;
		Track_Stop(ipt);
		return;
	}
	
	ipt->bSetForCancellation = FALSE;
	ipt->bInTrackNotification = FALSE;

	ret = IPOSDET_GetGPSInfo(ipt->posdet, 
		AEEGPS_GETINFO_LOCATION,
		AEEGPS_ACCURACY_LEVEL1,
		&ipt->gpsinfo,
		&ipt->cbPos);
	
	switch (ret) {
			case EPRIVLEVEL:
				WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info: application does not have PL_POS_LOCATION privilege");
				break;
			case EBADPARM:
				WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info: parameter specified is invalid ");
				break;
			case EUNSUPPORTED:
				WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info: this device does not support the gps function");
				break;
			case EFAILED:
				WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info ");
				break;
			case SUCCESS:
				break;
			default:
				WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info for unknown error");
				break;
	}
	IPOSITIONTHREAD_ShowOnUI(ipt);
	
	if(ret != SUCCESS)
	{
		//WSTRCPY(pRespPosData->gpsPosHintBuffer, L"fail to get GPS info for unknown error");
		Track_Stop(ipt);
	}else{
		WSTRCPY(pRespPosData->gpsPosHintBuffer,  L"getting gps info....");
	}
	
	IPOSITIONTHREAD_ShowOnUI(ipt);
	
}



void Track_cbInfo(IPositionThread * ipt){
	AEEGPSConfig config;
	
 
	ipt->bInTrackNotification = TRUE;

	IPOSDET_GetGPSConfig(ipt->posdet, &config);
	ZEROAT(ipt->pRespPosData->gpsPosHintBuffer);
	if( ipt->gpsinfo.status == AEEGPS_ERR_NO_ERR 
		|| (ipt->gpsinfo.status == AEEGPS_ERR_INFO_UNAVAIL && ipt->gpsinfo.fValid) ) {
			AECHAR cLat[MAX_RES_STRING_BUF_SIZE];
			AECHAR cLng[MAX_RES_STRING_BUF_SIZE];
#if MIN_BREW_VERSION(2,1)
			ipt->pRespPosData->lat = WGS84_TO_DEGREES(ipt->gpsinfo.dwLat );
			ipt->pRespPosData->lon = WGS84_TO_DEGREES( ipt->gpsinfo.dwLon );
#else
			double    wgsFactor;
			wgsFactor = FASSIGN_STR("186413.5111");
			ipt->pRespPosData->lat = FASSIGN_INT(ipt->gpsinfo.dwLat);
			ipt->pRespPosData->lat = FDIV(ipt->pRespPosData->lat, wgsFactor);

			ipt->pRespPosData->lon = FASSIGN_INT(ipt->gpsinfo.dwLon);
			ipt->pRespPosData->lon = FDIV(ipt->pRespPosData->lon, wgsFactor);
#endif /* MIN_BREW_VERSION 2.1 */
			ipt->pRespPosData->height =ipt->gpsinfo.wAltitude - 500;

			ipt->pRespPosData->dwFixNum++;

			ipt->pRespPosData->nErr = SUCCESS;
			FLOATTOWSTR(ipt->pRespPosData->lat, cLat, MAX_RES_STRING_BUF_SIZE );
			FLOATTOWSTR(ipt->pRespPosData->lon, cLng, MAX_RES_STRING_BUF_SIZE );

			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"gpsinfo{lat:%S, lng:%S, nFixes:%i, nInterval:%i}", 
				cLat , cLng, config.nFixes, config.nInterval);
			IPOSITIONTHREAD_ShowOnUI(ipt);
			if(FALSE == ipt->bSetForCancellation ){
				if( ipt->bModeLocal == FALSE)//定位的模式不是msb,重新更改定位模式为msb
					Track_Local(ipt);
				ISHELL_SetTimerEx( ipt->glbVar->pIShell, MIN_POSDET_INTERVAL * 1000, &ipt->cbIntervalTimer );
			}else{
				Track_Stop(ipt);
			}



	}
	else { 
		switch (ipt->gpsinfo.status)
		{
		case AEEGPS_ERR_GENERAL_FAILURE :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO: GENERAL_FAILURE");
			break;
		case AEEGPS_ERR_TIMEOUT :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:TIMEOUT");
			break;
		case AEEGPS_ERR_INFO_UNAVAIL:
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:INFO_UNAVAIL, nFixes:%i, nInterval:%i",
				config.nFixes, config.nInterval);
			break;
		case AEEGPS_ERR_ACCURACY_UNAVAIL :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:ACCURACY_UNAVAIL");
			break;
		case AEEGPS_ERR_PRIVACY_REFUSED :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:PRIVACY_REFUSED");
			break;
		case AEEGPS_ERR_SRV_UNREACHABLE :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:SRV_UNREACHABLE");
			break;
		case AEEGPS_ERR_LINK_FAILED :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:LINK_FAILED");
			break;
		case AEEGPS_ERR_REJECTED :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:ERR_REJECTED");
			break;
		case AEEGPS_ERR_OUTOF_RESOURCES :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:OUTOF_RESOURCES");
			break;
		case AEEGPS_ERR_RECEIVER_BUSY :
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:RECEIVER_BUSY");
			break;
		default:
			WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
				MAX_RES_STRING_BUF_SIZE,
				L"error from GPS notify INFO:UNDEFINED");
			break;
		}

		//IPOSITIONTHREAD_ShowOnUI(ipt);
		/*
		WSPRINTF(ipt->pRespPosData->gpsPosHintBuffer,
			MAX_RES_STRING_BUF_SIZE,
			L"get error info from GPS notify");*/

		if(  ipt->bModeLocal ) {
			/* Retry with TRACK_NETWORK at once */
			WSTRCAT(ipt->pRespPosData->gpsPosHintBuffer, L", Retry with MSA 5 seconds later"); 
			IPOSITIONTHREAD_ShowOnUI(ipt);
			Track_Network( ipt );
			//Track_cbInterval( ipt );
			ISHELL_SetTimerEx( ipt->glbVar->pIShell, 5 * 1000, &ipt->cbIntervalTimer );
		}else {
			ISHELL_PostEvent(ipt->glbVar->pIShell, ipt->glbVar->ClsId, EVT_COMMAND, EVT_COMMAND_LOCATE_RESTART, 0);
			/* Inform the application of failure code. */ 
			/*ipt->pRespPosData->nErr = ipt->gpsinfo.status;*/
			/* On timeout re-try. For other reasons bailout. */
			/*if( ipt->gpsinfo.status == AEEGPS_ERR_TIMEOUT ) {
				//Track_cbInterval( ipt );
				WSTRCAT(ipt->pRespPosData->gpsPosHintBuffer, L", Retry 5 seconds later");
				IPOSITIONTHREAD_ShowOnUI(ipt);
				ISHELL_SetTimerEx( ipt->glbVar->pIShell, 5 * 1000, &ipt->cbIntervalTimer );
			}
			else {
				WSTRCAT(ipt->pRespPosData->gpsPosHintBuffer, L", Retry 30 seconds later");
				IPOSITIONTHREAD_ShowOnUI(ipt);
				ISHELL_SetTimerEx( ipt->glbVar->pIShell, MIN_POSDET_INTERVAL * 1000, &ipt->cbIntervalTimer );
			}*/
		}
	}



}


int Track_Stop( IPositionThread * ipt ){
	if( !ipt ) {
		return EBADPARM;
	}
	 
	if( ipt->bInTrackNotification == TRUE ) {
		/* It is not safe to cleanup from a notification. Defer it. */
		ipt->bSetForCancellation = TRUE;
		WSTRCPY(ipt->pRespPosData->gpsPosHintBuffer, L"trying to stop the track thread, please wait a minute"); 
		IPOSITIONTHREAD_ShowOnUI(ipt);
		return EBADSTATE;
	}
	
	 
	WSTRCPY(ipt->pRespPosData->gpsPosHintBuffer, L"trying to stop tracking.....");
	IPOSITIONTHREAD_ShowOnUI(ipt);
	
	
	 
	ipt->bInProgress = FALSE;
	ipt->bInTrackNotification = FALSE;
	ipt->pRespPosData->bLocateStartOrStop = FALSE;

	/* Kill any ongoing process */
	CALLBACK_Cancel( &ipt->cbPos );
	CALLBACK_Cancel( &ipt->cbIntervalTimer );

	/* Report that Tracking is halted */
	ipt->pRespPosData->nErr = EIDLE;
	
	WSTRCPY(ipt->pRespPosData->gpsPosHintBuffer, L"tracking has been stopped now.");

	IPOSITIONTHREAD_ShowOnUI(ipt);
	 
	
	
	

 

	return SUCCESS;

}
  