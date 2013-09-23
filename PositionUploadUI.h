#ifndef POSITIONUPLOADUI_H
#define POSITIONUPLOADUI_H

#include "AEE.h"
#include "AEEStdLib.h"  // AEE Standard C library functions
#include "AEEShell.h"
#include "AEEMenu.h"
#include "Resource.h"
#include "lizard.brh"
#include "GlobalVariable.h"
/************************************************************************/
/*here positionui is the abbreviation of position upload ui                                                                      */
/************************************************************************/
typedef enum _eFocusCTL4PositionUI{
	POSUI_INIT,
	 POSUI_CTL_SK_LEFT,
	 POSUI_CTL_SK_RIGHT
}EFocusCTL4PositionUI;


#define INHERIT_IPositionUI(iname)\
	int (*pFNShow)(iname*);\
	boolean (*pFNHandleEvent)(iname* p, AEEEvent, uint16, uint32);\
	boolean (*pFNHandleEVKDOWNEvent)(iname* p);\
	boolean (*pFNHandleEVKUPEvent)(iname* p);\
	boolean (*pFNHandleSELECTUPEvent)(iname* p);\
	void (*pFNUpdateUI)(iname* p);\
	int (*pFNRelease)(iname*)


AEEINTERFACE_DEFINE(IPositionUI);

#define  IPOSITIONUI_Show( p)\
	AEEGETPVTBL(p, IPositionUI)->pFNShow( p)

#define  IPOSITIONUI_UpdateUI( p)\
	AEEGETPVTBL(p, IPositionUI)->pFNUpdateUI( p)

#define  IPOSITIONUI_HandleEvent( p, eCode, wParam, dwParam )\
	AEEGETPVTBL(p, IPositionUI)->pFNHandleEvent( p, eCode, wParam, dwParam)

#define IPOSITIONUI_HandleEVKDOWNEvent(p)\
	AEEGETPVTBL(p, IPositionUI)->pFNHandleEVKDOWNEvent(p)

#define  IPOSITIONUI_HandleEVKUPEvent(p)\
	AEEGETPVTBL(p, IPositionUI)->pFNHandleEVKUPEvent(p)

#define  IPOSITIONUI_HandleSELECTEvent(p)\
	AEEGETPVTBL(p, IPositionUI)->pFNHandleSELECTUPEvent(p)


#define  IPOSITIONUI_Release(p)\
	AEEGETPVTBL(p, IPositionUI)->pFNRelease(p)


struct IPositionUI{
	AEEVTBL(IPositionUI) *pvt; 
	AEECLSID clsID;
	IStatic  * pStatic;
	IStatic  * pStatic4Mode;
	IMenuCtl * pSK;
 
	EFocusCTL4PositionUI focusCTL;
	 
	IGlobalVariable* glbVar; 
};



int IPositionUI_New(  
					IGlobalVariable* glbVar,  
					AEECLSID clsID,  
				void ** iPositionUI
				);



#endif