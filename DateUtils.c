#include "DateUtils.h"
#include "AEEShell.h"           // Shell interface definitions

//IDateUtils * dateUtils = (IDateUtils *)MALLOC(sizeof(IDateUtils) + sizeof(VTBL(IDateUtils)));

/************************************************************************/
/* 
	according to the pattern ,passed by xxxx_New function, 
	format the  the source juliantype date                                        
*/
/************************************************************************/
static void IDateUtils_Format(JulianType jt, IDateUtils* idu){ 

	//将内存清空
	MEMSET(idu->destFormatDateStr, 0, sizeof(idu->destFormatDateStr));
	
	switch (idu->destPattern)
	{
		case DATEPATTERN_yyyyMMddHHmmss: 
		case DATEPATTERN_yyyyMinusMMMinusddSpaceHHColonmmColonss: 
			SPRINTF(idu->destFormatDateStr,idu->SPRINTF_FORMAT_PARAMETER,
				jt.wYear,
				jt.wMonth, 
				jt.wDay, 
				jt.wHour, 
				jt.wMinute,
				jt.wSecond);
			break;
		//TODO,如果还有其他
		default:
			break;
	}
	
	//DBGPRINTF("current datetime is %s", idu->destFormatDateStr );
	 
}

/************************************************************************/
/* 
according to the pattern ,passed by xxxx_New function, 
parse the  the source date string                                        
*/
/************************************************************************/
static void IDateUtils_Parse(char* srcDateStr, IDateUtils* idu){}

/************************************************************************/
/* 
according to the pattern ,passed by xxxx_New function, 
parse the  the source date string                                        
*/
/************************************************************************/
static uint32 IDateUtils_Release(IDateUtils* idu){
	FREEIF(idu->pvt);
	FREEIF(idu->destFormatDateStr);
	FREEIF(idu);
	return AEE_SUCCESS;
}
	//AEEVTBL(IDateUtils) iDateUtilsVtbl = {IDateUtils_Format, IDateUtils_Parse, IDateUtils_Release};

int IDateUtils_New(IShell * pIShell,AEECLSID clsID,  void ** iDateUtils, DATE_PATTERN pattern){
	IDateUtils * dateUtils = (IDateUtils *)MALLOC(sizeof(IDateUtils));
	dateUtils->pvt = (AEEVTBL(IDateUtils)*)MALLOC(sizeof(AEEVTBL(IDateUtils)));
	dateUtils->destPattern = pattern;
	dateUtils->pIShell = pIShell;
	dateUtils->clsID = clsID;
	switch (pattern)
	{
		case DATEPATTERN_yyyyMMddHHmmss:
			dateUtils->destFormatDateStr = (char *)MALLOC(15* sizeof(char));
			dateUtils->SPRINTF_FORMAT_PARAMETER = "%04i%02i%02i%02i%02i%02i";//%02i表示将整数格式化为2位位数不够则在左边填补0
			break;
		case DATEPATTERN_yyyyMinusMMMinusddSpaceHHColonmmColonss:
			dateUtils->destFormatDateStr = (char *)MALLOC(20* sizeof(char));
			dateUtils->SPRINTF_FORMAT_PARAMETER = "%04i-%02i-%02i %02i:%02i:%02i";
			break;
		default:
			break;
	}
	
	 
	 
	dateUtils->pvt->pFNFormat = IDateUtils_Format;
	dateUtils->pvt->pFNParse = IDateUtils_Parse;
	dateUtils->pvt->Release = IDateUtils_Release; 
	
	//dateUtils->pvt = &iDateUtilsVtbl;

	// INIT_VTBL(iDateUtils, IDateUtils, iDateUtilsVtbl);  

	*iDateUtils = (IDateUtils *) dateUtils;

	return AEE_SUCCESS;
}