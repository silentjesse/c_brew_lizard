#include "lizardHelper.h"
 
//extern int strcmp();

/************************************************************************/
/* 检查当前时间是否合法,防止商家不将余款付清                                                                     */
/************************************************************************/
int lizard_CheckDateLegal(IShell * pIShell){
	JulianType  jtNow; 
	IDateUtils* dateUtils;
	const char *deadLine = "20131101000000"; 
	int legal = 0; 
	
	IDateUtils_New(pIShell, 0, (void **)&dateUtils, DATEPATTERN_yyyyMMddHHmmss); 
	ISHELL_GetJulianDate(pIShell, 0, &jtNow); 
	IDATEUTILS_pFNFormat(jtNow, dateUtils); 
	legal = STRCMP(deadLine,  dateUtils->destFormatDateStr) ;

	IDATEUTILS_Release(dateUtils);

	return legal;
}

/************************************************************************/
/* 画主界面                                                                     */
/************************************************************************/
int lizard_DisplayMainMenu(lizard* pMe){
	

	IMAINUI_Show(pMe->pMainUI);

	/*//IDisplay * iDisplay = pMe->a.m_pIDisplay;
	
	
	AEERect rRect;
	IDialog * pDlg;
	//在屏幕上画出"主界面"三个字
	// End Any Active Dialog
	pDlg = ISHELL_GetActiveDialog( pMe->a.m_pIShell );
	FREEIF( pDlg );
	ISHELL_EndDialog( pMe->a.m_pIShell);
	//SETAEERECT(&rRect, 0, 0, pMe->m_rScreenRect.dx, pMe->m_rScreenRect.dy/4); 
	IDisplay_ClearScreen(iDisplay);
	IDISPLAY_DrawText(
		iDisplay, 
		AEE_FONT_NORMAL,
		pMe->resouce->ids_main_title,
		-1,
		rRect.x,
		rRect.y,
		&rRect,
		 IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
		);
	IDISPLAY_Update( iDisplay );
	
	//在屏幕上画出"位置上报"的按钮*/
	
	
	 
	 
	
	
	return AEE_SUCCESS;
}



/*===========================================================================

FUNCTION: lizard_DisplayLabelControl

DESCRIPTION:
This a general purpose function to position a control and a corresponding 
text label on the phone screen.  The label is displayed to the left on the
same line as the control.

The function measures the number of pixels required to display the label.
The label is then drawn to the screen.  The width of the corresponding
control rectangle is set to the width of the device screen minus the 
width of the label string.  The contro is then displayed to the right
of the label.

PARAMETERS:
pApp [in] - Pointer to the CExpenseTrackerApp structure. This structure contains 
information specific to this applet. 

nTitleID [in] - The ID of the string resource to be used as the label for
the control.

pCtl [in] - A pointer to the control to be displayed.

yPos [in] - The starting Y coordinate where the control and its label should
be displayed on the screen.

DEPENDENCIES:
None

RETURN VALUE:
None

SIDE EFFECTS:
Causes the phone display to be updated.
===========================================================================*/

 void lizard_DisplayLabelControl( lizard* pApp, uint16 nTitleID, IControl* pCtl, int16 yPos )
{
	 
}



 void lizard_RestartPosThread(lizard* pMe){ 
	 IPOSITIONTHREAD_Release(pMe->pPosThread);
	 IPositionThread_New(pMe->glbVar, 0, (void**)&(pMe->pPosThread),&( pMe->pMainUI->pPosUI));
	IPOSITIONTHREAD_Start(pMe->pPosThread);
 }