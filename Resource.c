#include "Resource.h"
 
/************************************************************************/
/* load resource from resource file , save in the struct of IResource                                                                     */
/************************************************************************/
uint32 IResource_LoadResString(IResource * resource, int16 IDS){
	IShell * iShell = resource->pIShell;
	MEMSET(resource->idsResource, 0, MAX_RES_STRING_BUF_SIZE);
	ISHELL_LoadResString(iShell, LIZARD_RES_FILE, IDS, resource->idsResource , MAX_RES_STRING_BUF_SIZE * sizeof(AECHAR));
	
	return AEE_SUCCESS;
}

/************************************************************************/
/* 
	release all resource(memory resource, file resource and other resources) , 
	this interface apply for          
*/
/************************************************************************/
uint32 IResource_Release(IResource * resource){
	FREE(resource->idsResource);
	FREEIF(resource);
	return AEE_SUCCESS;
}





//VTBL(IResource) resourceVTBL = {IResource_LoadResString, IResource_Release };

/************************************************************************/
/* new resource                                                                      */
/************************************************************************/
int IResource_New(IShell * pIShell,AEECLSID clsID,  void ** iResource){
	
	IResource * resource = (IResource *)MALLOC(sizeof(IResource));
	resource->pvt = (VTBL(IResource) * )MALLOC(sizeof(VTBL(IResource)));
	resource->pvt->pFNLoadResString = IResource_LoadResString;
	resource->pvt->Release = IResource_Release;
	
	resource->pIShell = pIShell;
	resource->clsID = clsID;
	resource->idsResource =	 (AECHAR *)MALLOC(MAX_RES_STRING_BUF_SIZE * sizeof(AECHAR));
	
	
	//TODO
	*iResource = resource;

	return AEE_SUCCESS;
}











