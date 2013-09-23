#ifndef HTTP_H
#define HTTP_H


#include "GlobalVariable.h"
#include "Resource.h"
#include "BREWVersion.h"

#define INHERIT_IHttp(iname)\
	int (*pFNConnect)(iname*);\
	int (*pFNPost)(iname*);\
	int (*pFNClose)(iname* p);\
	int (*pFNRelease)(iname*)

AEEINTERFACE_DEFINE(IHttp ); 

#define  IHTTP_Connect( p)\
	AEEGETPVTBL(p, IHttp)->pFNConnect( p)

#define  IHTTP_Close( p)\
	AEEGETPVTBL(p, IHttp)->pFNClose( p)

#define  IHTTP_Post( p)\
	AEEGETPVTBL(p, IHttp)->pFNPost( p)


#define  IHTTP_Release(p)\
	AEEGETPVTBL(p, IHttp)->pFNRelease(p)


struct IHttp{
	AEEVTBL(IHttp) *pvt; 
	AEECLSID clsID;
};


int IHttpd_New(IGlobalVariable* glbVar,  
				AEECLSID clsID,  
				void ** iHttp);



#endif
