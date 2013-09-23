#ifndef LIZARDHELPER_H
#define LIZARDHELPER_H
#include "DateUtils.h"
#include "AEEShell.h"           // Shell interface definitions
#include "lizard.h"
#include "Resource.h"
#include "MainUI.h"
int lizard_CheckDateLegal(IShell * pIShell);

int lizard_DisplayMainMenu(lizard* pMe);

void	lizard_RestartPosThread(lizard* pMe);


#endif