#include "UICommon.h"
#include "stdafx.h"
#include "UICommonMethod.h"
#include "UIFunction.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "PlayModeManager.h"
#include "CoreApi.h"
#include "SkillBagProcess.h"

int UpdateSkillExplan( int nIndex, int * pCodes, int nArea )
{
	if( nIndex < 0 || pCodes[nIndex] == 0 )	return nIndex;

	char sText[MAX_PATH]="";

    CUIControl* pDetailBG = CClientUIManager::GetPtr()->FindEventControl("STC_BG_00");
	assert(pDetailBG);

    CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pCodes[nIndex], 0));	
	assert(pSkillTable);

	_snprintf( sText, MAX_PATH, "Icon/Skill/%s.png", pSkillTable->m_sIconFile );
	CUIControl* pImg = pDetailBG->FindEventControl( "SkillImg" );
	assert(pImg);
	CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
	
	UIFunction::Set_ChildControlText( pDetailBG, "SkillName", pSkillTable->m_sName );
	UIFunction::Set_ChildControlText( pDetailBG, "SkillExplain", pSkillTable->m_sExplainText );
	UIFunction::Set_ChildControlText( pDetailBG, "SkillUsekey", pSkillTable->m_sKeyText );

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	_snprintf( sText, STRLEN_64, pMTable->m_sMessage, pSkillTable->m_nLimit );
	UIFunction::Set_ChildControlText( pDetailBG, "SkillLevelLimit", sText );

    UpdateSkillDesc( 1, pSkillTable, pDetailBG );
    UpdateSkillDesc( 2, pSkillTable, pDetailBG );
    UpdateSkillDesc( 3, pSkillTable, pDetailBG );

	if( 0 <= nArea )
	{
		_snprintf( sText, MAX_PATH, "Skill_Area_%d", pSkillTable->m_nArea[nArea] );
		CUIControl * pAreaImage = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pAreaImage);
		CUIControl * pSkillArea = pDetailBG->FindEventControl( "SkillArea" );
		assert(pSkillArea);
		pSkillArea->SetElement(pAreaImage->m_pElement->Clone());
	}

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, MAX_PATH, "SkillLevel%d", i+1);
		CUIControl* pLine = pDetailBG->FindEventControl(sText);
		assert(pLine);

		CUIControl* pArea = pLine->FindEventControl("AreaViewBtn");
		assert(pArea);

		if(pSkillTable->m_nArea[0] == 0)
			pArea->SetVisible(N3UI_VISIBLE_FALSE);
		else
			pArea->SetVisible(N3UI_VISIBLE_TRUE);

		if( CUIControl::Get_MouseOveredControl() == pArea )
		{
			_snprintf( sText, MAX_PATH, "Skill_Area_%d", pSkillTable->m_nArea[i] );
			CUIControl * pAreaImage = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pAreaImage);
			CUIControl * pSkillArea = pDetailBG->FindEventControl( "SkillArea" );
			assert(pSkillArea);
			pSkillArea->SetElement(pAreaImage->m_pElement->Clone());

			pAreaImage->SetVisible( N3UI_VISIBLE_TRUE );
		}

		if( CUIControl::Get_PressedControl() == pArea )
			nArea = i;
	}
	return nArea;
}

void UpdateSkillDesc( int nIndex, CResSkillTableFormat * pSkillTable, CUIControl * pParentControl )
{
    char sText[STRLEN_64];

    _snprintf( sText, STRLEN_64, "SkillLevel%d", nIndex );
    CUIControl * pStatic = pParentControl->FindEventControl( sText );
	assert(pStatic);

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	_snprintf( sText, STRLEN_64, pMTable->m_sMessage, nIndex );
	UIFunction::Set_ChildControlText( pStatic, "Level", sText);

	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900016 ,0));
	assert(pMTable);
	_snprintf( sText, STRLEN_64, pMTable->m_sMessage, pSkillTable->m_fCool[nIndex-1] );

	UIFunction::Set_ChildControlText( pStatic, "CoolTime", sText);
	UIFunction::Set_ChildControlText( pStatic, "Adjust", pSkillTable->m_sOptionText );
}

void UpdateTrainingExplan( int nIndex, int * pCodes )
{
    if( nIndex < 0 || pCodes[nIndex] == 0 )	return;

	char sText[MAX_PATH]="";

    CUIControl* pDetailBG = CClientUIManager::GetPtr()->FindEventControl("STC_BG_01");
	assert(pDetailBG);

    CResTrainingTableFormat *pTrainingTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, pCodes[nIndex], 0));	
	assert(pTrainingTable);

	_snprintf( sText, MAX_PATH, "Icon/Training/%s.png", pTrainingTable->m_sIconFile );
	CUIControl* pImg = pDetailBG->FindEventControl( "SkillImg" );
	assert(pImg);
	CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
	
	UIFunction::Set_ChildControlText( pDetailBG, "SkillName", pTrainingTable->m_sName );
	UIFunction::Set_ChildControlText( pDetailBG, "SkillExplain", pTrainingTable->m_sExplain );
}

void UpdateCeremonyExplan( int nIndex, int * pCodes )
{
    if( nIndex < 0 || pCodes[nIndex] == 0 )	return;

	char sText[MAX_PATH]="";

    CUIControl* pDetailBG = CClientUIManager::GetPtr()->FindEventControl("STC_BG_02");
	assert(pDetailBG);

    CResCeremonyTableFormat *pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, pCodes[nIndex], 0));	
	assert(pCeremonyTable);

	_snprintf( sText, MAX_PATH, "Icon/Ceremony/%s.png", pCeremonyTable->m_sIconFile );
	CUIControl* pImg = pDetailBG->FindEventControl( "SkillImg" );
	assert(pImg);
	CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
	
	UIFunction::Set_ChildControlText( pDetailBG, "SkillName", pCeremonyTable->m_sName );
	UIFunction::Set_ChildControlText( pDetailBG, "SkillExplain", pCeremonyTable->m_sExplain );
}

std::string GetAliasString( PLAYER_ALIAS eAlias )
{
    std::string cResultText;

	CResMessageTableFormat* pMTable = NULL;

    switch( eAlias )
    {
    case PLAYER_ALIAS_LIBERO:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900049 ,0));
		assert(pMTable);
		cResultText = pMTable->m_sMessage;
    break;

    default:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050 ,0));
		assert(pMTable);
		cResultText = pMTable->m_sMessage;
    break;
    }

    return cResultText;
}

std::string GetConditionString( PLAYER_CONDITION eCondition )
{
    std::string cResultString;

	CResMessageTableFormat* pMTable = NULL;

    switch( eCondition )
    {
    case PLAYER_CONDITION_ENTRANCE:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900051 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;

    case PLAYER_CONDITION_AWKWARD:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900052 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;

    case PLAYER_CONDITION_ADAPTATION:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900053 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;

    case PLAYER_CONDITION_EXPERT:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900054 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;
    
    case PLAYER_CONDITION_VETERAN:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900055 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;

    case PLAYER_CONDITION_PERFECTION:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900056 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;

    default:
        pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050 ,0));
		assert(pMTable);
		cResultString = pMTable->m_sMessage;
    break;
    }

    return cResultString;
}

std::string GetObjectKindString( OBJECT_KIND eObjectKind )
{
    std::string cResultText;

    switch( eObjectKind )
    {    
    case OBJECT_KIND_ITEM:
        cResultText = "ITEM";
    break;

    case OBJECT_KIND_SKILL:
        cResultText = "SKILL";
    break;

    case OBJECT_KIND_CEREMONY:
        cResultText = "CEREMONY";
    break;

    case OBJECT_KIND_TRAINING:
        cResultText = "TRAINING";
    break;

    case OBJECT_KIND_EXCHANGE:
        cResultText = "EXCHANGE";
    break;

    case OBJECT_KIND_CASH:
        cResultText = "CASH";
    break;

    case OBJECT_KIND_POINT:
        cResultText = "POINT";
    break;

    case OBJECT_KIND_EXP:
        cResultText = "EXP";
    break;

    case OBJECT_KIND_SKILLBONUS:
        cResultText = "SKILL BONUS";
    break;

    default:
        cResultText = "Unknown";
    break;
    }

    return cResultText;
}

void ShowQuestGiftItem( int nCode )
{
    CUIControl * pGiftItemIcon = CClientUIManager::GetPtr()->FindEventControl( "GiftItemIcon" );
    assert( pGiftItemIcon);
    if( pGiftItemIcon == 0 ) return;

    CUIControl * pGiftItemName = CClientUIManager::GetPtr()->FindEventControl( "GiftItemName" );
    assert(pGiftItemName);

    CResItemTableFormat * pItemTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, nCode, 0 ));
	//assert(pItemTable);

    char szIconFileName[ MAX_PATH ];

    if( pItemTable != 0 )
    {
        sprintf(szIconFileName, "Icon/Item/%s.png", pItemTable->m_sIconFile);
        pGiftItemName->SetText( pItemTable->m_sName );
    }
    else
    {

		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050 ,0));
		assert(pMTable);
        sprintf( szIconFileName, "Xkick_questitem.png" );
		pGiftItemName->SetText( pMTable->m_sMessage );
    }

    CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pGiftItemIcon, szIconFileName );
}

void ShowQuestGiftAmount( const char * pControlName, int nAmount )
{
    CUIControl * pControl = CClientUIManager::GetPtr()->FindEventControl( pControlName );
    assert( pControl != 0 );
    if( pControl == 0 ) return;

    char buf[512];
    sprintf( buf, "+ %d", nAmount );
    pControl->SetText( buf );
}

void ShowQuestGift( int nKind, int nCode, int nAmount )
{
    switch( nKind )
    {
	case OBJECT_KIND_NONE:
		break;

	case OBJECT_KIND_ITEM:
        ShowQuestGiftItem( nCode );
        break;

    case OBJECT_KIND_POINT:
        ShowQuestGiftAmount( "GiftPoint", nAmount );
        break;

    case OBJECT_KIND_EXP:
        ShowQuestGiftAmount( "GiftExp", nAmount );
        break;

    default:
        assert( !"Gift Kind Error!" );
        break;
    }
}

void ViewQuestDetail( int nQuestCode, bool bIsCR )
{
	CResQuestTableFormat * pQuestTable = dynamic_cast<CResQuestTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, nQuestCode, 0));
	assert(pQuestTable);

	if( bIsCR )
	{
		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		for(int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++)
		{
			if( pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == nQuestCode && pQuestTable->m_nRepeat == 0 )
			{
				CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20800, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
				return;
			}
		}
	}

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "CreateQuest" );

	if( pControl != NULL )
	{
		pControl->SetVisible( N3UI_VISIBLE_TRUE );

		pControl->Set_FocusedPage(pControl);
		CUIControl::RequestFocus(pControl);
	}


	// 퀘스트 명
	CUIControl* pQuestName = CClientUIManager::GetPtr()->FindEventControl( "QuestName" );
	assert(pQuestName);

	pQuestName->SetText(pQuestTable->m_sTitle);

	// 퀘스트 종류
	CUIControl* pQuestType = CClientUIManager::GetPtr()->FindEventControl( "QuestType" );
	assert(pQuestType);

	CResMessageTableFormat* pMTable = NULL;

	switch(pQuestTable->m_nKind)
	{
	case 0:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900084, 0));
		assert(pMTable);
		pQuestType->SetText(pMTable->m_sMessage);
		break;
	case 1:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900085, 0));
		assert(pMTable);
		pQuestType->SetText(pMTable->m_sMessage);
		break;
	case 2:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900086, 0));
		assert(pMTable);
		pQuestType->SetText(pMTable->m_sMessage);
		break;
	}

	// 반복 수행여부
	CUIControl* pQuestRepeat = CClientUIManager::GetPtr()->FindEventControl( "QuestRepeat" );
	assert(pQuestRepeat);

	switch(pQuestTable->m_nRepeat)
	{
	case 0:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900060, 0));
		assert(pMTable);
		pQuestRepeat->SetText(pMTable->m_sMessage);
		break;
	case 1:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900061, 0));
		assert(pMTable);
		pQuestRepeat->SetText(pMTable->m_sMessage);
		break;
	}

	//// 수행 가능 포지션
	//CUIControl* pQuestPosition = CClientUIManager::GetPtr()->FindEventControl( "QuestPosition" );
	//assert(pQuestPosition);

	//switch(pQuestTable->m_nPosition)
	//{
	//	case POSITION_ALL:
	//		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900087, 0));
	//		assert(pMTable);
	//		pQuestPosition->SetText(pMTable->m_sMessage);
	//		break;
	//	case POSITION_FW:	pQuestPosition->SetText("FW");	break;
	//	case POSITION_ST:	pQuestPosition->SetText("ST");	break;
	//	case POSITION_CF:	pQuestPosition->SetText("CF");	break;
	//	case POSITION_WF:	pQuestPosition->SetText("WF");	break;
	//	case POSITION_SS:	pQuestPosition->SetText("SS");	break;
	//	case POSITION_MF:	pQuestPosition->SetText("MF");	break;
	//	case POSITION_AM:	pQuestPosition->SetText("AM");	break;
	//	case POSITION_CM:	pQuestPosition->SetText("CM");	break;
	//	case POSITION_SM:	pQuestPosition->SetText("SM");	break;
	//	case POSITION_DM:	pQuestPosition->SetText("DM");	break;
	//	case POSITION_DF:	pQuestPosition->SetText("DF");	break;
	//	case POSITION_SW:	pQuestPosition->SetText("SW");	break;
	//	case POSITION_CB:	pQuestPosition->SetText("CB");	break;
	//	case POSITION_SP:	pQuestPosition->SetText("SP");	break;
	//}

	// 수행 가능 레벨
	CUIControl* pQuestLevel = CClientUIManager::GetPtr()->FindEventControl( "QuestLevel" );
	assert(pQuestLevel);

	char sText[MAX_PATH]="";

	_snprintf( sText, MAX_PATH, "%d - %d", pQuestTable->m_nStart_Level, pQuestTable->m_nEnd_Level );
	pQuestLevel->SetText(sText);

	// 참여 인원
	CUIControl* pQuestPerson = CClientUIManager::GetPtr()->FindEventControl( "QuestPerson" );
	assert(pQuestPerson);

	_snprintf( sText, MAX_PATH, "%d", pQuestTable->m_nJoin );
	pQuestPerson->SetText(sText);

	// 수행 조건
	CUIControl* pQuestClearCondition = CClientUIManager::GetPtr()->FindEventControl( "QuestClearCondition" );
	assert(pQuestClearCondition);
	
	if( pQuestTable->m_nCondition )
	{
		CResQuestTableFormat * pQuestConditionT = dynamic_cast<CResQuestTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, pQuestTable->GetObjCode(), 0));
		assert(pQuestConditionT);
		pQuestClearCondition->SetText(pQuestConditionT->m_sTitle);
	}
	else
	{
		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900088 ,0));
		assert(pMTable);
		pQuestClearCondition->SetText(pMTable->m_sMessage);
	}
		
	// 퀘스트 설명
	CUIControl* pQuestExplain = CClientUIManager::GetPtr()->FindEventControl( "QuestExplain" );
	assert(pQuestExplain);

	pQuestExplain->SetText( pQuestTable->m_sExplain );

	// 퀘스트 아이콘
	CUIControl* pQuestIcon = CClientUIManager::GetPtr()->FindEventControl( "QuestIconP" );
	assert(pQuestIcon);
	
	// 포지션 마크 출력
	CUIControl* pPosition = CClientUIManager::GetPtr()->FindEventControl(pQuestTable->m_sIconFile);
	assert(pPosition);
		
	pQuestIcon->SetElement(pPosition->m_pElement->Clone());

	CUIControl* pPositionMark = CClientUIManager::GetPtr()->FindEventControl( "PositionIconPD" );
	assert(pPositionMark);
	
	_snprintf(sText, MAX_PATH, "PUBLIC_APOSITION_IMAGE%d", pQuestTable->m_nPosition);
	CUIControl* pCPM = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pCPM);
	
	pPositionMark->SetElement(pCPM->m_pElement->Clone());

	CUIControl* pQuestGiftPoint = CClientUIManager::GetPtr()->FindEventControl( "QuestGiftPoint" );
	assert(pQuestGiftPoint);
	pQuestGiftPoint->SetText("0");

	CUIControl* pQuestGiftExp = CClientUIManager::GetPtr()->FindEventControl( "QuestGiftExp" );
	assert(pQuestGiftExp);
	pQuestGiftExp->SetText("0");

	CUIControl* pQuestGiftItemName = CClientUIManager::GetPtr()->FindEventControl( "GiftItemName" );
	assert(pQuestGiftItemName);
	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050, 0));
	assert(pMTable);
	pQuestGiftItemName->SetText(pMTable->m_sMessage);
	
	for(int i=0;i<3;i++)
	{
		switch(pQuestTable->m_cGift[i].m_nKind)
		{
		case OBJECT_KIND_ITEM:
			{
				CResItemTableFormat * pItemTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pQuestTable->m_cGift[i].m_nCode, 0 ));
				if(pItemTable == NULL) return;

				CUIControl* pQuestGiftIcon = CClientUIManager::GetPtr()->FindEventControl( "QuestGiftIcon" );
				assert(pQuestGiftIcon);
				
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pQuestGiftIcon, pItemTable->m_sIconFile );

				pQuestGiftItemName->SetText(pItemTable->m_sName);
			}
			break;
		case OBJECT_KIND_POINT:
			{
				_snprintf( sText, MAX_PATH, "%d", pQuestTable->m_cGift[i].m_nAmount );
				pQuestGiftPoint->SetText(sText);
			}
			break;
		case OBJECT_KIND_EXP:
			{
				_snprintf( sText, MAX_PATH, "%d", pQuestTable->m_cGift[i].m_nAmount );
				pQuestGiftExp->SetText(sText);
			}
			break;
		case OBJECT_KIND_NONE:
			continue;
		}
	}
}
