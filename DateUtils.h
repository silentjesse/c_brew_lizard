#ifndef DATEUTILS_H
#define DATEUTILS_H

#include "AEE.h"
#include "AEEStdLib.h"  // AEE Standard C library functions
 

typedef enum _DATE_PATTERN{
	DATEPATTERN_yyyyMMddHHmmss,//yyyyMMddHHmmss
	DATEPATTERN_yyyyMinusMMMinusddSpaceHHColonmmColonss//yyyy-MM-dd HH:mm:ss
}DATE_PATTERN;

#define INHERIT_IDateUtils(iname)\
	void (*pFNFormat)(JulianType, iname *);\
	void (*pFNParse)(char* srcDateStr, iname * );\
	uint32 (*Release)(iname*)

AEEINTERFACE_DEFINE(IDateUtils);

#define  IDATEUTILS_pFNFormat(JulianTypeDate, p)\
	AEEGETPVTBL(p, IDateUtils)->pFNFormat(JulianTypeDate, p);

#define  IDATEUTILS_pFNParse(srcDateStr, p)\
	AEEGETPVTBL(p, IDateUtils)->pFNParse(srcDateStr ,p);
/*
#define  IDATEUTILS_AddRef( p)\
	AEEGETPVTBL(p, IDateUtils)->AddRef(p );
*/
#define  IDATEUTILS_Release( p)\
	AEEGETPVTBL(p, IDateUtils)->Release(p );


struct IDateUtils{
	AEEVTBL(IDateUtils) *pvt;
	IShell * pIShell;
	int  destPattern;//目的格式
	char * destFormatDateStr;//按格式转换后的字符串
	JulianType destJulianTypeDate;//按格式解决后的日期
	char * SPRINTF_FORMAT_PARAMETER;//SPRINTF转换时用的格式参数
	AEECLSID clsID;
};

int IDateUtils_New(IShell * pIShell,AEECLSID clsID,  void ** iDateUtils, DATE_PATTERN pattern);



#endif