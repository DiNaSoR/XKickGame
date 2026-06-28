
#include "StdAfx.h"
#include "GameResultDialog.h"
#include "GameResultProcess.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

CObjRoomInfo*	CGameResultDialog::pRoom=NULL;

CGameResultDialog::CGameResultDialog(void)
{
}

CGameResultDialog::~CGameResultDialog(void)
{
	delete pRoom;
}
void CGameResultDialog::UpDateDialog(CObjRoomInfo*	_pRoom)
{
	if(pRoom==NULL)
		pRoom=new CObjRoomInfo;
	memcpy(pRoom,_pRoom,sizeof(CObjRoomInfo));
}

void CGameResultDialog::Init()
{
	if(pRoom==NULL)
	{
		CUIControl* pResultOpen = CClientUIManager::GetPtr()->FindEventControl("GameResultOpen");
		assert(pResultOpen);
		pResultOpen->SetVisible(N3UI_VISIBLE_FALSE);

		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 250301, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		CUIControl* pMESSAGE_CONFIRM = CClientUIManager::GetPtr()->FindEventControl( "MESSAGE_CONFIRM" );
		return;
	}
	
	char text[STRLEN_64]="";

	FUNCTION_HeadResult();		// 상위 결과 요약 사항 표시

	CUIControl* cPerson;

	int Hindex=0;
	int Aindex=6;

	CObjPlayerInfo* pUserCharacterInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);
	assert(pUserCharacterInfo);

	// MVP 플레이어 정보 표시
	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	
	_snprintf(text, STRLEN_64, pMTable->m_sMessage, pRoom->m_cMvpPlayer.m_nLevel);	
	UIFunction::Set_ControlText( "Static_MvpLevel", text );

	_snprintf( text, STRLEN_64, "%s",pRoom->m_cMvpPlayer.m_sName );	
	UIFunction::Set_ControlText( "Static_MvpName", text );

	// 각 플레이어 경기 이력 표시 반복문
	for(int i=0;i<TEAM_SIZE*2;++i)
	{
		// 캐릭별 컨트롤 라인 묶음
		if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_HOME)
		{
			sprintf(text,"Static_UserRecord%d",Hindex++);	
			cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			sprintf(text,"PUBLIC_HPOSITION_IMAGE%d",pRoom->m_cEachResult[i].m_nPosition);	
		}
		else if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_AWAY)
		{
			sprintf(text,"Static_UserRecord%d",Aindex++);	
			cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			sprintf(text,"PUBLIC_APOSITION_IMAGE%d",pRoom->m_cEachResult[i].m_nPosition);	
		}
		else
		{
			if(Hindex<6)
			{
				sprintf(text,"Static_UserRecord%d",Hindex++);	
				cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			}
			else
			{
				sprintf(text,"Static_UserRecord%d",Aindex++);	
				cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			}
			cPerson->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}

		// MVP 아이콘 표시
		if(pRoom->m_cMvpPlayer.m_nPlayerSeq != 0)
		{
			if(pRoom->m_cEachResult[i].m_nPlayerSeq==pRoom->m_cMvpPlayer.m_nPlayerSeq)
			{
				UIFunction::Set_UIVisibility( "Static_MvpPosition", N3UI_VISIBLE_TRUE );
				UICommonMethod::PositionIconDraw( "Static_MvpPosition", pRoom->m_cMvpPlayer.m_nPosition, UICommonMethod::AWAY_TEAM );
			}
		}
		else
			UIFunction::Set_UIVisibility( "Static_MvpPosition", N3UI_VISIBLE_FALSE );

        CUIControl* pStatic_M_bg = cPerson->FindEventControl("Static_M_bg");
		assert(pStatic_M_bg);
			
		if(pUserCharacterInfo->GetObjSeq() == pRoom->m_cEachResult[i].m_nPlayerSeq)
			pStatic_M_bg->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pStatic_M_bg->SetVisible(N3UI_VISIBLE_FALSE);

		FUNCTION_EachCharacterResult(i, cPerson);

		CUIControl* pStatic_Ai = cPerson->FindEventControl("Static_Ai");
		assert(pStatic_Ai);
		CUIControl* pStatic_Gained_Exp = cPerson->FindEventControl("Static_Gained_Exp");
		assert(pStatic_Gained_Exp);
		CUIControl* pStatic_Gained_Mileage = cPerson->FindEventControl("Static_Gained_Mileage");
		assert(pStatic_Gained_Mileage);
			
		// 플레이어일 경우 경험치 포인트 프로모션 표시
		if(pRoom->m_cEachResult[i].m_nPlayerSeq<200000000)
		{
			sprintf(text,"%d",pRoom->m_cEachResult[i].m_nExp);	
			pStatic_Gained_Exp->SetText(text);

			sprintf(text,"%d",pRoom->m_cEachResult[i].m_nPoint);	
			pStatic_Gained_Mileage->SetText(text);

			// 향후 피방 여부 표시
			CUIControl* pStatic_Gained_Promotion = cPerson->FindEventControl("Static_Gained_Promotion");
			assert(pStatic_Gained_Promotion);
			sprintf(text,"%d");
			pStatic_Ai->SetVisible(N3UI_VISIBLE_FALSE);
		}
		else	// AI 플레이어 아이콘 표시
		{
			pStatic_Gained_Exp->SetText("");
			pStatic_Gained_Mileage->SetText("");

			pStatic_Ai->SetVisible(N3UI_VISIBLE_TRUE);
		}
			
		cPerson->SetVisible(N3UI_VISIBLE_TRUE);
	}
}

void CGameResultDialog::Terminate()
{
}

void CGameResultDialog::FUNCTION_HeadResult()
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);
	char sText[STRLEN_64]="";

	// 스코어 표시
	_snprintf(sText, STRLEN_64, "BNum_%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]/LIST10_SIZE );
	UIFunction::ChangeTexture( "LSNum_00", sText );
	_snprintf(sText, STRLEN_64, "BNum_%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]%LIST10_SIZE );
	UIFunction::ChangeTexture( "LSNum_0", sText );
	_snprintf(sText, STRLEN_64, "BNum_%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]/LIST10_SIZE );
	UIFunction::ChangeTexture( "RSNum_00", sText );
	_snprintf(sText, STRLEN_64, "BNum_%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]%LIST10_SIZE );
	UIFunction::ChangeTexture( "RSNum_0", sText );
	
	// HomeTeam
	CUIControl* cHTeam = CClientUIManager::GetPtr()->FindEventControl("StaticH_TeamRecord");
	assert(cHTeam);

	// 총 득점(홈)
	CUIControl* pStatic_Marks = cHTeam->FindEventControl("Static_Marks");	
	assert(pStatic_Marks);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]);	
	pStatic_Marks->SetText(sText);

	// 총 어시스트(홈)
	CUIControl* pStatic_Assistance = cHTeam->FindEventControl("Static_Assistance");	
	assert(pStatic_Assistance);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_ASSIST]);	
	pStatic_Assistance->SetText(sText);

	// 총 컷팅(홈)
	CUIControl* pStatic_Cutting = cHTeam->FindEventControl("Static_Cutting");
	assert(pStatic_Cutting);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_CUT]);	
	pStatic_Cutting->SetText(sText);

	// 총 유효슈팅(홈)
	CUIControl* pStatic_Effactive_Shooting = cHTeam->FindEventControl("Static_Effactive_Shooting");	
	assert(pStatic_Effactive_Shooting);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_SHOOT]);	
	pStatic_Effactive_Shooting->SetText(sText);

	// 총 유효스틸(홈)
	CUIControl* pStatic_Effactive_Steal = cHTeam->FindEventControl("Static_Effactive_Steal");	
	assert(pStatic_Effactive_Steal);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_STEAL]);	
	pStatic_Effactive_Steal->SetText(sText);

	// 총 태클(홈)
	CUIControl* pStatic_Effactive_Tackle = cHTeam->FindEventControl("Static_Effactive_Tackle");
	assert(pStatic_Effactive_Tackle);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_TACKLE]);	
	pStatic_Effactive_Tackle->SetText(sText);

	// 총 볼점유율(홈)
	CUIControl* pStatic_BallShare = cHTeam->FindEventControl("Static_BallShare");	
	assert(pStatic_BallShare);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_POSSESSION]);	
	pStatic_BallShare->SetText(sText);


	// AwayTeam
	CUIControl* cATeam = CClientUIManager::GetPtr()->FindEventControl("StaticA_TeamRecord");
	assert(cATeam);

	// 총 득점수(어웨이)
	pStatic_Marks = cATeam->FindEventControl("Static_Marks");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]);	
	pStatic_Marks->SetText(sText);

	// 총 어시스트(어웨이)
	pStatic_Assistance = cATeam->FindEventControl("Static_Assistance");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_ASSIST]);	
	pStatic_Assistance->SetText(sText);

	// 총 컷팅(어웨이)
	pStatic_Cutting = cATeam->FindEventControl("Static_Cutting");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_CUT]);	
	pStatic_Cutting->SetText(sText);

	// 총 유효슈팅(어웨이)
	pStatic_Effactive_Shooting = cATeam->FindEventControl("Static_Effactive_Shooting");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_SHOOT]);	
	pStatic_Effactive_Shooting->SetText(sText);

	// 총 유효스틸(어웨이)
	pStatic_Effactive_Steal = cATeam->FindEventControl("Static_Effactive_Steal");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_STEAL]);	
	pStatic_Effactive_Steal->SetText(sText);

	// 총 태클(어웨이)
	pStatic_Effactive_Tackle = cATeam->FindEventControl("Static_Effactive_Tackle");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_TACKLE]);	
	pStatic_Effactive_Tackle->SetText(sText);

	// 총 볼점유율(어웨이)
	pStatic_BallShare = cATeam->FindEventControl("Static_BallShare");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_POSSESSION]);	
	pStatic_BallShare->SetText(sText);	

	if( pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL] < pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL] )
	{
		UIFunction::SetChildToVisibility( cHTeam, "WinIcon", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( cATeam, "WinIcon", N3UI_VISIBLE_TRUE );
	}
	else if( pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL] > pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL] )
	{
		UIFunction::SetChildToVisibility( cHTeam, "WinIcon", N3UI_VISIBLE_TRUE );
		UIFunction::SetChildToVisibility( cATeam, "WinIcon", N3UI_VISIBLE_FALSE );
	}
	else
	{
		UIFunction::SetChildToVisibility( cHTeam, "WinIcon", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( cATeam, "WinIcon", N3UI_VISIBLE_FALSE );
	}
}

void CGameResultDialog::FUNCTION_EachCharacterResult(int i, CUIControl* cPerson)
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);
	char sText[STRLEN_64]="";

	// 포지션 아이콘 표시
	if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_HOME)
		UICommonMethod::PositionIconDraw( cPerson, "Static_Position", pRoom->m_cEachResult[i].m_nPosition, UICommonMethod::HOME_TEAM);	
	else
		UICommonMethod::PositionIconDraw( cPerson, "Static_Position", pRoom->m_cEachResult[i].m_nPosition, UICommonMethod::AWAY_TEAM);	
		
	// 레벨
	CUIControl* pStatic_Level = cPerson->FindEventControl("Static_Level");	
	assert(pStatic_Level);
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_nLevel);	
	pStatic_Level->SetText(sText);

	// 레벨 업
	CUIControl* pStatic_LevelUp = cPerson->FindEventControl("LevelUpIcon");	
	assert(pStatic_LevelUp);
	if(pRoom->m_cEachResult[i].m_nIsLevelUp)
	{
		pStatic_LevelUp->SetVisible(N3UI_VISIBLE_TRUE);
		//pStatic_LevelUp->SetAnimationStart(N3UI_STATE_NORMAL, ANIMATION_LOOP);
	}
	else
		pStatic_LevelUp->SetVisible(N3UI_VISIBLE_FALSE);

	// 이름
	CUIControl* pStatic_Name = cPerson->FindEventControl("Static_Name");
	assert(pStatic_Name);
	_snprintf(sText, STRLEN_64, "%s", pRoom->m_cEachResult[i].m_sName);	
	pStatic_Name->SetText(sText);

	// 득점
	CUIControl* pStatic_Marks = cPerson->FindEventControl("Static_Marks");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_GOAL]);	
	pStatic_Marks->SetText(sText);

	// 어시스트
	CUIControl* pStatic_Assistance = cPerson->FindEventControl("Static_Assistance");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_ASSIST]);	
	pStatic_Assistance->SetText(sText);

	// 컷팅
	CUIControl* pStatic_Cutting = cPerson->FindEventControl("Static_Cutting");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_CUT]);	
	pStatic_Cutting->SetText(sText);

	// 유효슈팅
	CUIControl* pStatic_Effactive_Shooting = cPerson->FindEventControl("Static_Effactive_Shooting");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_SHOOT]);	
	pStatic_Effactive_Shooting->SetText(sText);

	// 유효스틸
	CUIControl* pStatic_Effactive_Steal = cPerson->FindEventControl("Static_Effactive_Steal");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_STEAL]);	
	pStatic_Effactive_Steal->SetText(sText);
	
	// 유효태클
	CUIControl* pStatic_Effactive_Tackle = cPerson->FindEventControl("Static_Effactive_Tackle");	
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_TACKLE]);	
	pStatic_Effactive_Tackle->SetText(sText);

	// 평점
	CUIControl* pStatic_Examination = cPerson->FindEventControl("Static_Examination");	
	_snprintf(sText, STRLEN_64, "%.1f",(((float)pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_MARK] )/10.0f));	
	pStatic_Examination->SetText(sText);
}