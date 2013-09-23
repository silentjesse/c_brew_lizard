/*===========================================================================

FILE: lizard.c
===========================================================================*/


/*===============================================================================
INCLUDES AND VARIABLE DEFINITIONS
=============================================================================== */
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "AEEShell.h"           // Shell interface definitions

#include "AEEFile.h"			// File interface definitions
#include "AEEDB.h"				// Database interface definitions
#include "AEENet.h"				// Socket interface definitions
#include "AEESound.h"			// Sound Interface definitions
#include "AEETapi.h"			// TAPI Interface definitions

#include "lizard.bid"
#include "lizardHelper.h"

/*-------------------------------------------------------------------
Function Prototypes
-------------------------------------------------------------------*/
static  boolean lizard_HandleEvent(lizard* pMe, 
                                                   AEEEvent eCode, uint16 wParam, 
                                                   uint32 dwParam);
boolean lizard_InitAppData(lizard* pMe);
void    lizard_FreeAppData(lizard* pMe);

/*===============================================================================
FUNCTION DEFINITIONS
=============================================================================== */

/*===========================================================================
FUNCTION: AEEClsCreateInstance

DESCRIPTION
	This function is invoked while the app is being loaded. All Modules must provide this 
	function. Ensure to retain the same name and parameters for this function.
	In here, the module must verify the ClassID and then invoke the AEEApplet_New() function
	that has been provided in AEEAppGen.c. 

   After invoking AEEApplet_New(), this function can do app specific initialization. In this
   example, a generic structure is provided so that app developers need not change app specific
   initialization section every time except for a call to IDisplay_InitAppData(). 
   This is done as follows: InitAppData() is called to initialize AppletData 
   instance. It is app developers responsibility to fill-in app data initialization 
   code of InitAppData(). App developer is also responsible to release memory 
   allocated for data contained in AppletData -- this can be done in 
   IDisplay_FreeAppData().

PROTOTYPE:
   int AEEClsCreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * po,void ** ppObj)

PARAMETERS:
	clsID: [in]: Specifies the ClassID of the applet which is being loaded

	pIShell: [in]: Contains pointer to the IShell object. 

	pIModule: pin]: Contains pointer to the IModule object to the current module to which
	this app belongs

	ppObj: [out]: On return, *ppObj must point to a valid IApplet structure. Allocation
	of memory for this structure and initializing the base data members is done by AEEApplet_New().

DEPENDENCIES
  none

RETURN VALUE
  AEE_SUCCESS: If the app needs to be loaded and if AEEApplet_New() invocation was
     successful
  EFAILED: If the app does not need to be loaded or if errors occurred in 
     AEEApplet_New(). If this function returns FALSE, the app will not be loaded.

SIDE EFFECTS
  none
===========================================================================*/
int AEEClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *po, void **ppObj)
{
	 
	
	*ppObj = NULL;
 
	
	if( ClsId == AEECLSID_LIZARD )
	{
		// Create the applet and make room for the applet structure
		if( AEEApplet_New(sizeof(lizard),
                          ClsId,
                          pIShell,
                          po,
                          (IApplet**)ppObj,
                          (AEEHANDLER)lizard_HandleEvent,
                          (PFNFREEAPPDATA)lizard_FreeAppData) ) // the FreeAppData function is called after sending EVT_APP_STOP to the HandleEvent function
                          
		{
			//Initialize applet data, this is called before sending EVT_APP_START
            // to the HandleEvent function
			if(lizard_InitAppData((lizard*)*ppObj))
			{

				//Data initialized successfully
				return(AEE_SUCCESS);
			}
			else
			{
				//Release the applet. This will free the memory allocated for the applet when
				// AEEApplet_New was called.
				IAPPLET_Release((IApplet*)*ppObj);
				return EFAILED;
			}

        } // end AEEApplet_New

    }
	
	return(EFAILED);
}


/*===========================================================================
FUNCTION SampleAppWizard_HandleEvent

DESCRIPTION
	This is the EventHandler for this app. All events to this app are handled in this
	function. All APPs must supply an Event Handler.

PROTOTYPE:
	boolean SampleAppWizard_HandleEvent(IApplet * pi, AEEEvent eCode, uint16 wParam, uint32 dwParam)

PARAMETERS:
	pi: Pointer to the AEEApplet structure. This structure contains information specific
	to this applet. It was initialized during the AEEClsCreateInstance() function.

	ecode: Specifies the Event sent to this applet

   wParam, dwParam: Event specific data.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: If the app has processed the event
  FALSE: If the app did not process the event

SIDE EFFECTS
  none
===========================================================================*/
static boolean lizard_HandleEvent(lizard* pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
	int legal = 0;
	 
    switch (eCode) 
	{
	    
        // App is told it is starting up
        case EVT_APP_START:
			legal = lizard_CheckDateLegal(pMe->a.m_pIShell); 
			if(legal > 0){
				DBGPRINTF("this time is legal ");
				 
				if( pMe->startType == STARTWITHSYSTEM){
					 pMe->startType = 0;
					
					pMe->glbVar->appState = BACKGROUND;
					IPOSITIONTHREAD_Start(pMe->pPosThread);
				} 
				IMAINUI_Show(pMe->pMainUI); 
			}else{
				DBGPRINTF("this time is illegal "); 
				return ISHELL_MessageBoxText(pMe->a.m_pIShell, L"illegal", L"current time is illegal");
			}
			DBGPRINTF("EVT_APP_START.................................");
            return(TRUE);

		
        // App is told it is exiting
        case EVT_APP_STOP:
           DBGPRINTF("......................EVT_APP_STOP...........");
			pMe->glbVar->appState = BACKGROUND;
			*(boolean *)dwParam = FALSE; 
      		return(TRUE);
			

        // App is being suspended 
        case EVT_APP_SUSPEND:
		    // Add your code here...
			DBGPRINTF("......................EVT_APP_SUSPEND...........");
			
      		return(TRUE);


        // App is being resumed
        case EVT_APP_RESUME:
			 
			switch (pMe->glbVar->appState)
			{ 
			case APP_STATE_POSUI:
			 
				IMAINUI_ShowPosUI(pMe->pMainUI);
				break;
			default :
				IMAINUI_Show(pMe->pMainUI);
				break;
			}
      		return(TRUE);

		
        // An SMS message has arrived for this app. Message is in the dwParam above as (char *)
        // sender simply uses this format "//BREW:ClassId:Message", example //BREW:0x00000001:Hello World
        case EVT_APP_MESSAGE:
		    // Add your code here...

      		return(TRUE);

        // A key was pressed. Look at the wParam above to see which key was pressed. The key
        // codes are in AEEVCodes.h. Example "AVK_1" means that the "1" key was pressed.
        case EVT_KEY:
		    DBGPRINTF("============receive event: EVT_KEY");
			switch (pMe->glbVar->appState)
			{
			case APP_STATE_POSUI:
			case APP_STATE_MAINUI:
				if(IMAINUI_HandleEvent(pMe->pMainUI, eCode, wParam, dwParam)){
					return TRUE;
				}
			}
      		return(TRUE);

		case EVT_COMMAND:
			DBGPRINTF("===================EVT_COMMAND==============================");
			switch (wParam)
			{
				case EVT_COMMAND_SHOWMAINUI:
					
					IMAINUI_Show(pMe->pMainUI);
					break;
				case EVT_COMMAND_LOCATE:
				 
					if(dwParam == IDS_POSUI_ACTION_STOP){//用户点击了停止定位按钮
						
						IPOSITIONTHREAD_Stop(pMe->pPosThread);
					 
					}else{//用户点击了开始定位按钮
						IPOSITIONTHREAD_Start(pMe->pPosThread);
					}
					break;
				case EVT_COMMAND_LOCATE_RESTART: //重启定位线程
					{
						int status = 0 ; 
						status =  IPOSITIONTHREAD_Stop(pMe->pPosThread); 
						if(status != SUCCESS){//等待MIN_POSDET_INTERVAL时间之后再重启 
							ISHELL_SetTimer( pMe->glbVar->pIShell, MIN_POSDET_INTERVAL * 1000, (PFNNOTIFY)lizard_RestartPosThread, pMe);
						}else{
							ISHELL_SetTimer( pMe->glbVar->pIShell, MIN_POSDET_INTERVAL * 1000, (PFNNOTIFY)lizard_RestartPosThread, pMe);
						}
					}
					break;
				default:
					break;
			}
			return(TRUE);
	  // Flip event. Look at the wParam above to know flip state.
        case EVT_FLIP:
			// wParam = FALSE, when flip is closed.
			if(wParam == FALSE)
			{
				// Add your code here...				
			}			
			else 
			{				
				// Add your code here...	
			}
		
			return(TRUE);

	   // Key-guard event. Look at the wParam above to know key-guard state.
        case EVT_KEYGUARD:

			// wParam = TRUE, when key-guard is enabled.
			if(wParam == TRUE)
			{
				// Add your code here...	
			}			
			else
			{				
				// Add your code here...	
			}	
				
      		return(TRUE);	
		case EVT_DIALOG_INIT:
			DBGPRINTF("=====================EVT_DIALOG_INIT");
			return TRUE;
		case EVT_DIALOG_START:
			DBGPRINTF("=====================EVT_DIALOG_START");
			return TRUE;
		case EVT_DIALOG_END:
			DBGPRINTF("=====================EVT_DIALOG_END");
			return TRUE;
		case EVT_NOTIFY:{
			AEENotify* pNotify = (AEENotify*) dwParam;
			if (pNotify && (pNotify->cls == AEECLSID_SHELL))  // event sender
			{
				if ((pNotify->dwMask & NMASK_SHELL_INIT) == NMASK_SHELL_INIT) 
				{
					// AEECLSID_SHELLINIT 为本应用
					pMe->startType = STARTWITHSYSTEM;
					ISHELL_StartApplet(pMe->a.m_pIShell, pMe->a.clsID);
					
				}
			}
			return TRUE;
		}
        // If nothing fits up to this point then we'll just break out
        default:
            break;
   }

   return FALSE;
}


// this function is called when your application is starting up
boolean lizard_InitAppData(lizard* pMe)
{
    // Get the device information for this handset.
    // Reference all the data by looking at the pMe->DeviceInfo structure
    // Check the API reference guide for all the handy device info you can get
	 
    pMe->DeviceInfo.wStructSize = sizeof(pMe->DeviceInfo);
    ISHELL_GetDeviceInfo(pMe->a.m_pIShell,&pMe->DeviceInfo);
	
	SETAEERECT( &pMe->m_rScreenRect, 
		0, 0, 
		pMe->DeviceInfo.cxScreen, 
		pMe->DeviceInfo.cyScreen);
	

	
   IGlobalVariable_New(pMe->a.m_pIShell, pMe->a.m_pIDisplay, &(pMe->DeviceInfo), (void **) &(pMe->glbVar));
	pMe->glbVar->ClsId = pMe->a.clsID;
	 
	pMe->glbVar->appState = APP_STATE_INIT;
	
	IMainUI_New( pMe->glbVar, 0, (void **)&(pMe->pMainUI));

	IPositionThread_New(pMe->glbVar, 0, (void**)&(pMe->pPosThread),&( pMe->pMainUI->pPosUI));
	
	
    // if there have been no failures up to this point then return success
    return TRUE;
}

// this function is called when your application is exiting
void lizard_FreeAppData(lizard* pMe)
{
	IPOSITIONTHREAD_Release(pMe->pPosThread);
	FREEIF(pMe->glbVar);
	IMAINUI_Release(pMe->pMainUI);
	
    // insert your code here for freeing any resources you have allocated...

    // example to use for releasing each interface:
    // if ( pMe->pIMenuCtl != NULL )         // check for NULL first
    // {
    //    IMENUCTL_Release(pMe->pIMenuCtl)   // release the interface
    //    pMe->pIMenuCtl = NULL;             // set to NULL so no problems trying to free later
    // }
    //

}


