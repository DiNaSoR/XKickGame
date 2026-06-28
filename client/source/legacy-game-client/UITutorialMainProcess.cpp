#include "stdafx.h"
#include "UIFunction.h"
#include "TutorialMainProcess.h"
#include "TutorialPlay1Process.h"
#include "GuideTutorialProcess.h"

///////////////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_USERFUNCTION( CHOICE_TUTORIAL )
{
	int index = atol( pParam->GetValue( "Index" ) );

	if( index == 100 )
		CProcessManager::GetPtr()->SetChangeProcess(new CGuideTutorialProcess(14));
	else
		CProcessManager::GetPtr()->ChangeTutorialeMachine(index);
}