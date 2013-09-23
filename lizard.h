#ifndef LIZARD_H
#define LIZARD_H
#include "AEEShell.h"           // Shell interface definitions
#include "AEEModGen.h"          // Module interface definitions
#include "AEEAppGen.h"          // Applet interface definitions
#include "Resource.h"
#include "MainUI.h"
#include "GlobalVariable.h"
#include "PositionThread.h"
/*-------------------------------------------------------------------
Applet structure. All variables in here are reference via "pMe->"
-------------------------------------------------------------------*/
// create an applet structure that's passed around. All variables in
// here will be able to be referenced as static.

//-100表示从随系统
#define STARTWITHSYSTEM -100

typedef struct _lizard {
	AEEApplet      a ;	       // First element of this structure must be AEEApplet
	AEEDeviceInfo  DeviceInfo; // always have access to the hardware device information 
	AEERect        m_rScreenRect;  // Screen Rect
	IMainUI *		pMainUI;
	IPositionThread * pPosThread;
	IGlobalVariable * glbVar;
	int				startType;//-100表示从随系统
	// add your own variables here...
} lizard;


#endif
