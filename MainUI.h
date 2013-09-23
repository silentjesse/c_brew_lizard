#ifndef	MAINUI_H
#define MAINUI_H
#include "AEE.h"
#include "AEEStdLib.h"  // AEE Standard C library functions
#include "AEEShell.h"
#include "AEEMenu.h"
#include "Resource.h"
#include "lizard.brh"
#include "GlobalVariable.h"
#include "PositionUploadUI.h"

typedef enum _eFocusCTL4MainUI{
	MAINUI_INIT,
	MAINUI_CTL_PMENU_LOCATE,
	MAINUI_CTL_PMENU_PHOTO,
	MAINUI_CTL_PSDK_SETTTINGS,
	MAINUI_POSUI
}EFocusCTL4MainUI;



#define INHERIT_IMainUI(iname)\
	uint32 (*pFNShow)(iname*);\
	uint32 (*pFNShowPosUI)(iname*);\
	uint32 (*pFNClosePosUI)(iname*);\
	boolean (*pFNHandleEvent)(iname* p, AEEEvent, uint16, uint32);\
	boolean (*pFNHandleEVKDOWNEvent)(iname* p);\
	boolean (*pFNHandleEVKUPEvent)(iname* p);\
	boolean (*pFNHandleSELECTUPEvent)(iname* p);\
	uint32 (*pFNRelease)(iname*)
	

AEEINTERFACE_DEFINE(IMainUI);

#define  IMAINUI_Show( p)\
	AEEGETPVTBL(p, IMainUI)->pFNShow( p)

#define  IMAINUI_ShowPosUI( p)\
	AEEGETPVTBL(p, IMainUI)->pFNShowPosUI( p)

#define  IMAINUI_ClosePosUI(p)\
	AEEGETPVTBL(p, IMainUI)->pFNClosePosUI(p)

#define  IMAINUI_HandleEvent( p, eCode, wParam, dwParam )\
	AEEGETPVTBL(p, IMainUI)->pFNHandleEvent( p, eCode, wParam, dwParam)

#define  IMAINUI_HandleEVKDOWNEvent(p)\
	AEEGETPVTBL(p, IMainUI)->pFNHandleEVKDOWNEvent(p)

#define  IMAINUI_HandleEVKUPEvent(p)\
	AEEGETPVTBL(p, IMainUI)->pFNHandleEVKUPEvent(p)

#define  IMAINUI_HandleSELECTEvent(p)\
	AEEGETPVTBL(p, IMainUI)->pFNHandleSELECTUPEvent(p)


#define  IMAINUI_Release(p)\
	AEEGETPVTBL(p, IMainUI)->pFNRelease(p)




struct IMainUI{
	AEEVTBL(IMainUI) *pvt;
	AEECLSID clsID;
	IMenuCtl * pMenu;
	IMenuCtl * pSK; 
	EFocusCTL4MainUI focusCTL; 
	IPositionUI *pPosUI;
	IGlobalVariable* glbVar; 
};


int IMainUI_New(IGlobalVariable* glbVar,   AEECLSID clsID,  
				void ** iMainUI);

#endif