#ifndef RESOURCE_H
#define RESOURCE_H
#include "AEE.h"
#include "AEEStdLib.h"  // AEE Standard C library functions
#include "AEEShell.h"
 
#include "lizard.brh"
#define MAX_RES_STRING_BUF_SIZE      256                  // Max buffer size for string buffers

#define INHERIT_IResource(iname)\
	uint32 (*pFNLoadResString)(iname*, int16 IDS);\
	uint32 (*Release)(iname*)

AEEINTERFACE_DEFINE(IResource);

#define  IRESOURCE_LoadResource( p, IDS)\
	AEEGETPVTBL(p, IResource)->pFNLoadResString( p , IDS)

#define  IRESOURCE_Release(p)\
	AEEGETPVTBL(p, IResource)->Release(p)




struct IResource{
	AEEVTBL(IResource) *pvt;
	IShell * pIShell;
	AECHAR * idsResource;

	AEECLSID clsID;
};

int IResource_New(IShell * pIShell,AEECLSID clsID,  void ** iResource);

#endif