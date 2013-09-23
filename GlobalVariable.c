#include "GlobalVariable.h"
 





uint32 IGlobalVariable_Release(IGlobalVariable * p){
	IRESOURCE_Release(p->pIResouce);
	FREEIF(p->pPosData);
	if(p != NULL)
	FREEIF(p->pvt);
	
	FREEIF(p);
	return AEE_SUCCESS;
}



int IGlobalVariable_New(IShell * shell, IDisplay * display, 
						AEEDeviceInfo  *device, void** formerGlobalVar){
	IGlobalVariable * globalVar = (IGlobalVariable *)MALLOC(sizeof(IGlobalVariable));
	globalVar->pvt = (VTBL(IGlobalVariable) *)MALLOC(sizeof(VTBL(IGlobalVariable)));
	globalVar->pvt->pFNRelease = IGlobalVariable_Release;
	globalVar->pDevice = device;
	globalVar->pIDisplay = display;
	globalVar->pIShell = shell;

	globalVar->pPosData = (PositionData *)MALLOC(sizeof(PositionData));
	
	// Insert your code here for initializing or allocating resources...
	IResource_New(globalVar->pIShell, 0, (void **)&globalVar->pIResouce);
	
	IRESOURCE_LoadResource(globalVar->pIResouce, IDS_POSUI_STATIC_DEFAULTTEXT);
	WSTRCPY(globalVar->pPosData->gpsPosHintBuffer, globalVar->pIResouce->idsResource);

	*formerGlobalVar = globalVar;
	return AEE_SUCCESS;
}