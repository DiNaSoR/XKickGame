#include "stdafx.h"
#include "UIFunction.h"
#include "GuideTutorialProcess.h"

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( GT_NEXT )
{
    CGuideTutorialProcess* pCurrentProcess = dynamic_cast<CGuideTutorialProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->GuideStepPlus();

	pCurrentProcess->FUNCTION_Init();
}

IMPLEMENT_USERFUNCTION( GT_PREV )
{
	CGuideTutorialProcess* pCurrentProcess = dynamic_cast<CGuideTutorialProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->GuideStepMinus();

	pCurrentProcess->FUNCTION_Init();
}

IMPLEMENT_USERFUNCTION( SHOW_GUIDE )
{
	char index = atoi( pParam->GetValue( "Index" ) );

	char sText[STRLEN_64]="";
	_snprintf( sText, STRLEN_64, "AbilityGroup0%d", index );
	UIFunction::SetChildToVisibility( sText, "Cover", N3UI_VISIBLE_FALSE );

	int i=0;
	int nCnt=0;
	for(;i<LIST5_SIZE;i++)
	{
		_snprintf( sText, STRLEN_64, "AbilityGroup0%d", i );
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pControl);

		CUIControl* pCover = pControl->FindEventControl( "Cover" );
		assert(pCover);

		if( pCover->GetVisible() == N3UI_VISIBLE_TRUE )
			nCnt++;
	}

	if( nCnt == 0 )
	{
		CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 100, 0));
		assert(pTable);

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
		
		if( (int)pObjPlayerInfo->m_vQuestList.size() )
		{
			for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++ )
			{
				if( pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == 100 )
					break;

				if( i == (int)pObjPlayerInfo->m_vQuestList.size()-1 )
				{
                    _snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
					UIFunction::Set_ControlText( "GainPoint", sText );		
				}
			}
		}
		else
		{
			_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
			UIFunction::Set_ControlText( "GainPoint", sText );
		}

		UIFunction::Set_UIVisibility( "NextTutorialBtn", N3UI_VISIBLE_TRUE );

		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900122 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "NpcExplain", pMTable->m_sMessage );
	}
}