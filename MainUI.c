#include "MainUI.h"

/************************************************************************/
/* 显示主界面                                                                     */
/************************************************************************/
uint32 IMainUI_Show(IMainUI* pUI){
	CtlAddItem ai;
	AEERect rRect;
	boolean ok; 
	pUI->glbVar->appState = APP_STATE_MAINUI;
	IDisplay_ClearScreen(pUI->glbVar->pIDisplay);
	IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_MAINUI_TITLE);
	IMENUCTL_SetTitle(pUI->pMenu, NULL, 0, pUI->glbVar->pIResouce->idsResource);
	
	
	// Add Items in the Menu
	
	IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_MAINUI_LOCATE);
	
	// Add Items in the Menu
	//ai.pText = pUI->glbVar->pIResouce->idsResource;
	ai.pImage = NULL;
	ai.pszResImage = 
		ai.pszResText = NULL;
	ai.wImage = NULL;
	ai.wText = NULL;
	ai.dwData = 0;

	// Add New
	
	
	ai.wFont = AEE_FONT_NORMAL;
	ai.wItemID = IDS_MAINUI_LOCATE;
 

	//位置上报
	IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_MAINUI_LOCATE);
	ai.pText = pUI->glbVar->pIResouce->idsResource;
	ai.wItemID = IDS_MAINUI_LOCATE;
	ok = IMENUCTL_AddItemEx(pUI->pMenu, &ai);
 
	//拍照上传
	IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_MAINUI_PHOTO);
	ai.pText = pUI->glbVar->pIResouce->idsResource;
	ai.wItemID = IDS_MAINUI_PHOTO; 
	IMENUCTL_AddItemEx(pUI->pMenu, &ai);
	
	
	SETAEERECT(&rRect, 0, 5, pUI->glbVar->pDevice->cxScreen, 
		pUI->glbVar->pDevice->cyScreen -  IDISPLAY_GetFontMetrics( pUI->glbVar->pIDisplay, AEE_FONT_NORMAL, NULL, NULL ) - 10);
	IMENUCTL_SetRect(pUI->pMenu, &rRect);
	IMENUCTL_SetActive(pUI->pMenu, FALSE);
	IMENUCTL_Redraw(pUI->pMenu);
	
	IRESOURCE_LoadResource(pUI->glbVar->pIResouce, IDS_MAINUI_SETTINGS);
	IMENUCTL_AddItem(pUI->pSK, NULL, NULL, IDS_MAINUI_SETTINGS, pUI->glbVar->pIResouce->idsResource, 0);
	
	 IMENUCTL_Redraw(pUI->pSK);
	 
	pUI->focusCTL = MAINUI_INIT;
	return AEE_SUCCESS;
}


/************************************************************************/
/* 显示定位状态信息界面                                                                     */
/************************************************************************/
uint32 IMainUI_ShowPosUI(IMainUI* pUI){
	IPositionUI_New( pUI->glbVar,
		0, 
		(void **) &(pUI->pPosUI)
		);

	IPOSITIONUI_Show(pUI->pPosUI);
	
	return AEE_SUCCESS;
}

/************************************************************************/
/* 关闭定位状态信息界面                                                                     */
/************************************************************************/
uint32 IMainUI_ClosePosUI(IMainUI* pUI){
	if(pUI->glbVar->appState == APP_STATE_POSUI)
		IPOSITIONUI_Release(pUI->pPosUI);
	return AEE_SUCCESS;
}



boolean IMainUI_HandleEvent(IMainUI* pUI, AEEEvent eCode, uint16 wParam, uint32 dwParam){
	switch(pUI->glbVar->appState){
		case APP_STATE_POSUI:
			IPOSITIONUI_HandleEvent(pUI->pPosUI, eCode,wParam, dwParam );
			break;
		case APP_STATE_MAINUI:
			switch(eCode){
				case EVT_KEY:
					switch(wParam){
				case AVK_DOWN:
					IMAINUI_HandleEVKDOWNEvent(pUI);
					break;
				case AVK_UP:
					IMAINUI_HandleEVKUPEvent(pUI);
					break;
				case AVK_SELECT:
					IMAINUI_HandleSELECTEvent(pUI);
					break;
				default:
					break;
					}
					break;
				default:
					break;
			}
			break;
		default:
			
			break;
		
	}
	return TRUE;
	 
}



 



boolean IMainUI_HandleEVKDOWNEvent( IMainUI* pUI){
	switch(pUI->focusCTL){
		case MAINUI_INIT:
			IMENUCTL_SetActive(pUI->pMenu, TRUE);
			IMENUCTL_SetActive(pUI->pSK, FALSE);
			pUI->focusCTL = MAINUI_CTL_PMENU_LOCATE;
			break;
		case MAINUI_CTL_PMENU_LOCATE:
			IMENUCTL_SetActive(pUI->pMenu, TRUE);
			IMENUCTL_SetActive(pUI->pSK, FALSE);
			IMENUCTL_SetSel(pUI->pMenu, IDS_MAINUI_PHOTO) ;
			pUI->focusCTL = MAINUI_CTL_PMENU_PHOTO;
			break;
		case MAINUI_CTL_PMENU_PHOTO:
			IMENUCTL_SetActive(pUI->pMenu, FALSE);
			IMENUCTL_SetActive(pUI->pSK, TRUE);
			pUI->focusCTL = MAINUI_CTL_PSDK_SETTTINGS;

	}
	return TRUE;
}


boolean IMainUI_HandleEVKUPEvent( IMainUI* pUI){
	switch(pUI->focusCTL){
		case MAINUI_INIT:
			//IMENUCTL_SetActive(pUI->pMenu, TRUE);
			//IMENUCTL_SetActive(pUI->pSK, FALSE);
			//pUI->focusCTL = MAINUI_CTL_PMENU_LOCATE;
			break;
		case MAINUI_CTL_PMENU_LOCATE:
			IMENUCTL_SetActive(pUI->pMenu, TRUE);
			IMENUCTL_SetActive(pUI->pSK, FALSE);
			///IMENUCTL_SetSel(pUI->pMenu, IDS_MAINUI_PHOTO) ;
			//UI->focusCTL = MAINUI_CTL_PMENU_PHOTO;
			break;
		case MAINUI_CTL_PMENU_PHOTO:
			IMENUCTL_SetActive(pUI->pMenu, TRUE);
			IMENUCTL_SetActive(pUI->pSK, FALSE);
			IMENUCTL_SetSel(pUI->pMenu, IDS_MAINUI_LOCATE) ;
			pUI->focusCTL = MAINUI_CTL_PMENU_LOCATE;
			break;
		case MAINUI_CTL_PSDK_SETTTINGS:
			IMENUCTL_SetActive(pUI->pMenu, TRUE);
			IMENUCTL_SetActive(pUI->pSK, FALSE);
			IMENUCTL_SetSel(pUI->pMenu, IDS_MAINUI_PHOTO) ;
			pUI->focusCTL = MAINUI_CTL_PMENU_PHOTO;
		case MAINUI_POSUI:
			break;
		default:
			break;

	}
	return TRUE;
}



boolean IMainUI_HandleSELECTEvent( IMainUI* pUI){
	//int createOk;
	switch(pUI->focusCTL){
		case MAINUI_INIT: 
			DBGPRINTF("=============================MAINUI_INIT=");
			break;
		case MAINUI_CTL_PMENU_LOCATE:
			//pUI->focusCTL = MAINUI_POSUI;
			IMAINUI_ShowPosUI(pUI);
			break;
		case MAINUI_CTL_PMENU_PHOTO:
			 DBGPRINTF("==============================MAINUI_CTL_PMENU_PHOTO");
			break;
		case MAINUI_CTL_PSDK_SETTTINGS:
			 DBGPRINTF("==============================MAINUI_CTL_PSDK_SETTTINGS");
			 break;//SUCCESS  ENOMEMORY EBADPARM EFAILED
		default:
			break;
	}
	return TRUE;
}




uint32 IMainUI_Release(IMainUI* pUI){
	IMAINUI_ClosePosUI(pUI);
	IMENUCTL_Reset(pUI->pMenu);
	IMENUCTL_Release(pUI->pMenu);
	IMENUCTL_Reset(pUI->pSK);
	IMENUCTL_Release(pUI->pSK);
	FREEIF(pUI->pvt);
	FREEIF(pUI);
	return AEE_SUCCESS;
}

 



int IMainUI_New(IGlobalVariable* glbVar, AEECLSID clsID, void ** iMainUI){
	IMainUI * mainUI = (IMainUI *)MALLOC(sizeof(IMainUI));
	mainUI->pvt = (VTBL(IMainUI) * )MALLOC(sizeof(VTBL(IMainUI)));
	mainUI->clsID = clsID;
	mainUI->pvt->pFNShow = IMainUI_Show;
	mainUI->pvt->pFNShowPosUI = IMainUI_ShowPosUI;
	mainUI->pvt->pFNClosePosUI = IMainUI_ClosePosUI;
	mainUI->pvt->pFNHandleEvent = IMainUI_HandleEvent;
	mainUI->pvt->pFNHandleEVKDOWNEvent = IMainUI_HandleEVKDOWNEvent;
	mainUI->pvt->pFNHandleEVKUPEvent = IMainUI_HandleEVKUPEvent;
	mainUI->pvt->pFNHandleSELECTUPEvent = IMainUI_HandleSELECTEvent;
	mainUI->pvt->pFNRelease = IMainUI_Release;
	
	 mainUI->glbVar = glbVar;
	
	 ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_MENUCTL, (void**)(&mainUI->pMenu));
	 ISHELL_CreateInstance(glbVar->pIShell, AEECLSID_SOFTKEYCTL, (void**)(&mainUI->pSK));
	 
	 
	*iMainUI = mainUI;
	
	return AEE_SUCCESS;

}
