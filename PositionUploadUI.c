#include "PositionUploadUI.h"





/************************************************************************/
/* 显示位置信息界面                                                                  */
/************************************************************************/
int IPositionUI_Show(IPositionUI* pUI){
	IPOSITIONUI_UpdateUI(pUI);
	 pUI->focusCTL = POSUI_INIT;
	return AEE_SUCCESS;
}



/************************************************************************/
/* 更新位置信息                                                                   */
/************************************************************************/
void IPositionUI_UpdateUI(IPositionUI* pUI){
	 
	AEERect rect4Static;
	AEERect rect4ModeStatic;
	int16 posHintheight = 0;
 
	 //IDS_STRING_TRACKTYPE
	pUI->glbVar->appState = APP_STATE_POSUI;
	IDisplay_ClearScreen(pUI->glbVar->pIDisplay);
	
	
	 
	
	 IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_POSUI_TITLE);
	ISTATIC_SetText(pUI->pStatic,
		pUI->glbVar->pIResouce->idsResource ,
		pUI->glbVar->pPosData->gpsPosHintBuffer, 
		AEE_FONT_BOLD,
		AEE_FONT_NORMAL);
	posHintheight = pUI->glbVar->pDevice->cyScreen - 
		(IDisplay_GetFontMetrics(pUI->glbVar->pIDisplay,AEE_FONT_BOLD, NULL, NULL ) * 6 + 10);
	 SETAEERECT(&rect4Static,
		 0,
		 0,
		 pUI->glbVar->pDevice->cxScreen,
		// IDisplay_MeasureText(pUI->glbVar->pIDisplay,AEE_FONT_BOLD, pUI->glbVar->pPosData->gpsPosHintBuffer ),
		 //IDisplay_GetFontMetrics(pUI->glbVar->pIDisplay,AEE_FONT_BOLD, NULL, NULL ) * 2 + 10
		  posHintheight
		 );
	ISTATIC_SetRect(pUI->pStatic, &rect4Static);
	ISTATIC_Redraw(pUI->pStatic);
	
	 
	if(pUI->glbVar->pPosData->bLocateStartOrStop == TRUE){
	 
		IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_STRING_TRACKTYPE);

		if(pUI->glbVar->pPosData->gpsMode == AEEGPS_MODE_TRACK_LOCAL){
		 
			ISTATIC_SetText(pUI->pStatic4Mode,
				pUI->glbVar->pIResouce->idsResource ,
				L"MSB",
				AEE_FONT_BOLD,
				AEE_FONT_NORMAL);
		}else if(pUI->glbVar->pPosData->gpsMode == AEEGPS_MODE_TRACK_NETWORK){
			 
			ISTATIC_SetText(pUI->pStatic4Mode,
				pUI->glbVar->pIResouce->idsResource ,
				L"MSA",
				AEE_FONT_BOLD,
				AEE_FONT_NORMAL);
		}
		 
		SETAEERECT(&rect4ModeStatic,
			0,
			posHintheight,
			pUI->glbVar->pDevice->cxScreen,
			 
			 IDisplay_GetFontMetrics(pUI->glbVar->pIDisplay,AEE_FONT_BOLD, NULL, NULL )*2 + 10
			);
		ISTATIC_SetRect(pUI->pStatic4Mode, &rect4ModeStatic);
		ISTATIC_Redraw(pUI->pStatic4Mode);
	}
	



	
	IMENUCTL_DeleteAll(pUI->pSK);
	 
	 if(pUI->glbVar->pPosData->bLocateStartOrStop == TRUE){//表示在定位中 
		 IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_POSUI_ACTION_STOP);
		 IMENUCTL_AddItem(pUI->pSK, NULL, NULL, IDS_POSUI_ACTION_START, pUI->glbVar->pIResouce->idsResource, 
			 IDS_POSUI_ACTION_STOP);
		
	 }else{//不在定位
		 IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_POSUI_ACTION_START);
		 IMENUCTL_AddItem(pUI->pSK, NULL, NULL, IDS_POSUI_ACTION_START, pUI->glbVar->pIResouce->idsResource,
			 IDS_POSUI_ACTION_START);
	 }

	 IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_POSUI_ACTION_RETURN);
	 IMENUCTL_AddItem(pUI->pSK, NULL, NULL, IDS_POSUI_ACTION_RETURN, pUI->glbVar->pIResouce->idsResource, 0); 
	 IMENUCTL_Redraw(pUI->pSK);
	
}




boolean IPositionUI_HandleEvent(IPositionUI* pUI, AEEEvent eCode, uint16 wParam, uint32 dwParam){
	switch(eCode){
		case EVT_KEY:
			switch(wParam){
				case AVK_DOWN:
					 
					IPOSITIONUI_HandleEVKDOWNEvent(pUI);
					return TRUE;
				case AVK_UP:
					 
					IPOSITIONUI_HandleEVKUPEvent(pUI);
					return TRUE;
				case AVK_SELECT:
					
					IPOSITIONUI_HandleSELECTEvent(pUI);
					return TRUE;
				default:
					break;
			}
			return TRUE;
		default:
			break;
	}
	return TRUE;  
	 
}


boolean IPositionUI_HandleEVKUPEvent( IPositionUI* pUI){
	 
	IMENUCTL_SetActive(pUI->pSK, TRUE);
	
 

	if(pUI->focusCTL == POSUI_CTL_SK_RIGHT){
		IMENUCTL_SetSel(pUI->pSK, IDS_POSUI_ACTION_START); 
		pUI->focusCTL = POSUI_CTL_SK_LEFT;
	}else{
		 
		IMENUCTL_SetSel(pUI->pSK, IDS_POSUI_ACTION_RETURN);
		pUI->focusCTL = POSUI_CTL_SK_RIGHT;
	} 
	return TRUE;
}



boolean IPositionUI_HandleEVKDOWNEvent( IPositionUI* pUI){ 
	return IPositionUI_HandleEVKUPEvent(pUI);
 
}





boolean IPositionUI_HandleSELECTEvent( IPositionUI* pUI){
	uint16 itemId;
	uint32 ldata;

	
	switch (pUI->focusCTL)
	{
		case POSUI_CTL_SK_LEFT: 
			 
			itemId = IMENUCTL_GetSel(pUI->pSK);
			IMENUCTL_GetItemData(pUI->pSK, itemId, &ldata);
			
			ISHELL_SendEvent(pUI->glbVar->pIShell, pUI->glbVar->ClsId, EVT_COMMAND, EVT_COMMAND_LOCATE, ldata);

			IPOSITIONUI_UpdateUI(pUI);

			break;
		case POSUI_CTL_SK_RIGHT:
			 
			//ISHELL_SendEvent(pUI,AEECLSID_LIZARD,  );
			ISHELL_PostEvent(pUI->glbVar->pIShell, pUI->glbVar->ClsId, EVT_COMMAND, EVT_COMMAND_SHOWMAINUI, 0);
			//IPositionUI_Release(pUI); 
			IPOSITIONUI_Release(pUI);
			break;
		default:
			break;
	}
	 
	
	return TRUE;
}


int IPositionUI_Release(IPositionUI* pUI){
	if(pUI == NULL){
		return AEE_SUCCESS;
	}
	ISTATIC_Reset(pUI->pStatic);
	ISTATIC_Release(pUI->pStatic);
	ISTATIC_Reset(pUI->pStatic4Mode);
	ISTATIC_Release(pUI->pStatic4Mode);
	IMENUCTL_Reset(pUI->pSK);
	IMENUCTL_Release(pUI->pSK);
	FREEIF(pUI->pvt);
	FREEIF(pUI);
	return AEE_SUCCESS;
}




int IPositionUI_New(IGlobalVariable* glbVar,
					AEECLSID clsID,  
					void ** iUI){
	
	IPositionUI * ui = NULL; 
	ui = (IPositionUI *)MALLOC(sizeof(IPositionUI));
	ui->pvt = (VTBL(IPositionUI) * )MALLOC(sizeof(VTBL(IPositionUI)));
	ui->clsID = clsID;
	ui->pvt->pFNShow = IPositionUI_Show; 
	ui->pvt->pFNHandleEvent = IPositionUI_HandleEvent;
	 
	ui->pvt->pFNHandleEVKDOWNEvent = IPositionUI_HandleEVKDOWNEvent;
	ui->pvt->pFNHandleEVKUPEvent = IPositionUI_HandleEVKUPEvent;
	ui->pvt->pFNHandleSELECTUPEvent = IPositionUI_HandleSELECTEvent;
	ui->pvt->pFNUpdateUI = IPositionUI_UpdateUI;

	ui->pvt->pFNRelease = IPositionUI_Release;

	ui->glbVar = glbVar;
	
	ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_STATIC, (void**)(&ui->pStatic4Mode));
	ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_STATIC, (void**)(&ui->pStatic));
	ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_SOFTKEYCTL, (void**)(&ui->pSK));
	 
	*iUI = ui;
	
	return AEE_SUCCESS;

}
