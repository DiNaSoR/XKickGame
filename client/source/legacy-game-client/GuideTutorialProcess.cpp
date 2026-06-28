#include "stdafx.h"
#include "GuideTutorialProcess.h"
#include "ResBallFormat.h"
#include "UICommonMethod.h"
#include "MovieManager.h"
#include "GetPacket.h"
#include "PutPacket.h"
#include "UtlLog.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UIButton.h"
#include "ChoiceServerProcess.h"
#include "ChoiceRoomProcess.h"
#include "UIFunction.h"
#include "TutorialMainProcess.h"

CGuideTutorialProcess::CGuideTutorialProcess(char nStep) : CBaseProcess()
{
	m_nProcessCmd = PROCESS_GUIDETUTORIAL;
	m_bIsRender = true;
	m_nGuideTutorialStep = nStep;
	m_bGT = false;
}

CGuideTutorialProcess::~CGuideTutorialProcess(void)
{
}

void CGuideTutorialProcess::Terminate()
{
	CBaseProcess::Terminate();
}

void CGuideTutorialProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, ".\\Script\\GameScript.xml", m_nProcessCmd*10000);

	FUNCTION_Init();

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CGuideTutorialProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
}

void CGuideTutorialProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();
	}
}

void CGuideTutorialProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_INIT_LOAD;
		}
		break;
	case STATE_INIT_LOAD:
		{
			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{
			LoadGround();
			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
			m_bIsRender = true;
			m_nState = STATE_PLAY;
		}
		break;
	case STATE_PLAY:
		{
			m_nState = STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

CBaseProcess* CGuideTutorialProcess::GetSameProcess()
{
	return new CGuideTutorialProcess(0);
}

bool CGuideTutorialProcess::OnDefault(NiEventRef pEventRecord)
{	
	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CGuideTutorialProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CGuideTutorialProcess::GuideStepPlus()
{
	if( m_nGuideTutorialStep == GT_ABILITY )
	{
		CObjQuestInfo* pQuestList = new CObjQuestInfo();
		pQuestList->m_nAmount	+= 1;
		pQuestList->m_nCode		= 100;
		
		if( (int)m_pObjPlayerInfo->m_vQuestList.size() )
		{
			for( int i=0;i<(int)m_pObjPlayerInfo->m_vQuestList.size();i++ )
			{
				if( m_pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == pQuestList->m_nCode )
					break;

				if( i == (int)m_pObjPlayerInfo->m_vQuestList.size()-1 )
				{
					m_pObjPlayerInfo->m_vQuestList.push_back(pQuestList);
					PutQuestReward((short)pQuestList->m_nCode);
				}
			}
		}
		else
		{
			m_pObjPlayerInfo->m_vQuestList.push_back(pQuestList);
			PutQuestReward((short)pQuestList->m_nCode);
		}

		CProcessManager::GetPtr()->SetNextProcess(new CTutorialMainProcess);
		return;
	}

	char sBG[STRLEN_32]="";
	int nBG = 0;

	switch(m_nGuideTutorialStep)
	{
	case GT_INTRODUCE:
	case GT_INTRODUCE2:
	case GT_CLASSTREE2:
	case GT_AREA:
	case GT_FINAL:
		nBG = 0;
		break;
	case GT_CLASSTREE:
	case GT_ATTACKAREA:
	case GT_SHORTSHOOT:
	case GT_LONGSHOOT:
	case GT_CROSSAREA:
	case GT_DEFENCEAREA:
	case GT_KEYCONTROL:
	case GT_KEYCONTROL2:
	case GT_KEYCONFIG:
		nBG = 1;
		break;
	case GT_ABILITY:
		nBG = 2;
		break;
	}

	if( nBG < 2 )
	{
		_snprintf( sBG, STRLEN_32, "Static0%d_Bg", nBG );
		CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( sBG );
		assert(pBG);
		CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
		assert(pShadowLine);

		if( pShadowLine->IsPlay() )
		{
			m_bGT = true;
			pShadowLine->SetInitUpdate(false);
			pShadowLine->SetPlay(true, false, false, 199);
		}
		else
			m_nGuideTutorialStep++;
	}

	if(GT_FINAL<m_nGuideTutorialStep)
	{
		m_pObjPlayerInfo = (CObjPlayerInfo*)CObjectManager::GetPtr()->GetSequenceField( OBJECT_TRIOLIST, 0 );

		CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTCHARACTER, m_pObjPlayerInfo);

		CProcessManager::GetPtr()->SetNextProcess(new CChoiceServerProcess);

		PutExecuteQuest(1, 0);
	}
}

void CGuideTutorialProcess::GuideStepMinus()
{
	m_nGuideTutorialStep--;

	if(m_nGuideTutorialStep<GT_INTRODUCE)
		m_nGuideTutorialStep = GT_INTRODUCE;
}

char CGuideTutorialProcess::GetGuideStep()
{
    return m_nGuideTutorialStep;
}

void CGuideTutorialProcess::SetGuideStep(char nStep)
{
    m_nGuideTutorialStep = nStep;
}

void CGuideTutorialProcess::FUNCTION_Init()
{
	char sText[STRLEN_32]="";

	if( m_bGT )
	{
		m_bGT = false;
		return;
	}

	UIFunction::Set_UIVisibility( "Static00_Bg", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Static01_Bg", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Static02_Bg", N3UI_VISIBLE_FALSE );

	int nBG = 0;

	switch(m_nGuideTutorialStep)
	{
	case GT_INTRODUCE:
	case GT_INTRODUCE2:
	case GT_CLASSTREE2:
	case GT_AREA:
	case GT_FINAL:
		UIFunction::Set_UIVisibility( "Static00_Bg", N3UI_VISIBLE_TRUE );
		nBG = 0;
		break;
	case GT_CLASSTREE:
	case GT_ATTACKAREA:
	case GT_SHORTSHOOT:
	case GT_LONGSHOOT:
	case GT_CROSSAREA:
	case GT_DEFENCEAREA:
	case GT_KEYCONTROL:
	case GT_KEYCONTROL2:
	case GT_KEYCONFIG:
		UIFunction::Set_UIVisibility( "Static01_Bg", N3UI_VISIBLE_TRUE );		
		nBG = 1;
		break;
	case GT_ABILITY:
		{
			for(int i=0;i<LIST5_SIZE;i++)
			{
				_snprintf( sText, STRLEN_32, "AbilityGroup0%d", i );
				CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pControl);

				UIFunction::SetChildToVisibility( pControl, "Cover", N3UI_VISIBLE_TRUE );
			}

			UIFunction::Set_ControlText( "GainPoint", "" );
			UIFunction::Set_UIVisibility( "NextTutorialBtn", N3UI_VISIBLE_FALSE );

			UIFunction::Set_ControlText( "NpcExplain", CResourceManager::GetPtr()->m_pStaticMap[300001].c_str() );

			UIFunction::Set_UIVisibility( "Static02_Bg", N3UI_VISIBLE_TRUE );

			nBG = 2;
		}
		break;
	}

	FUNCTION_GuideStart(nBG);
}

void CGuideTutorialProcess::FUNCTION_GuideStart(char nBG)
{
	char sText[STRLEN_32]="";
	char sBG[STRLEN_32]="";
	
	for(int i=1;i<GT_COUNT;i++)
	{
		_snprintf( sText, STRLEN_32, "Guide_Step%02d", i );

		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		if(pControl == NULL)	continue;

		if( m_nGuideTutorialStep == i )
			pControl->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}

	if( nBG < 2 )
	{
		_snprintf( sText, STRLEN_32, "GTMent%02d", m_nGuideTutorialStep );
		_snprintf( sBG, STRLEN_32, "Static0%d_Bg", nBG );
		UIFunction::ChangeChildTexture( sBG, "GT_Static", sText );
	
		CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( sBG );
		assert(pBG);
		CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
		assert(pShadowLine);
		
		pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, false, 0);

		//NormalSound("sample.wav");
	}

	switch( m_nGuideTutorialStep )
	{
	case GT_CLASSTREE:
		{
			_snprintf( sText, STRLEN_32, "Guide_Step%02d", m_nGuideTutorialStep );

			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);

			pControl->SetFrame(0);
			pControl->SetPlay(true, false, true);
		}
		break;
	case GT_ATTACKAREA:
	case GT_SHORTSHOOT:
	case GT_LONGSHOOT:
	case GT_CROSSAREA:
	case GT_DEFENCEAREA:
	case GT_KEYCONFIG:
		{
			_snprintf( sText, STRLEN_32, "Guide_Step%02d", m_nGuideTutorialStep );

			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);

			pControl->SetPlay(true,true);
		}
		break;
	case GT_INTRODUCE:
	case GT_INTRODUCE2:
	case GT_CLASSTREE2:
	case GT_AREA:
	case GT_KEYCONTROL:
	case GT_KEYCONTROL2:
	case GT_FINAL:
		break;
	case GT_ABILITY:
		break;
	}
}