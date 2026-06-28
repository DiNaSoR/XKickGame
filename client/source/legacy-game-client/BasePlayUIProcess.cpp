#include "stdafx.h"
#include "BasePlayProcess.h"
#include "UICommonMethod.h"
#include "UIFunction.h"
#include "UIIMEEditBox.h"
#include "CoreApi.h"

void CBasePlayProcess::FUNCTION_UIInit()
{
	char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;

	// 스코어 보드 ON
	UIFunction::Set_UIVisibility( "GAMEPLAY_SCORE_BG", N3UI_VISIBLE_TRUE );

	// 스코어 보드 Home Away 색 표시(자신팀은 파랑, 적은 빨강) (관전자(View) 일경우 모두 파란색)
	if( myTeam == PLAYER_TEAM_HOME )
	{
		UIFunction::Set_UIVisibility("Team_Home01", N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility("Team_Home", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Team_Away01", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Team_Away", N3UI_VISIBLE_TRUE);
	}
	else if(myTeam == PLAYER_TEAM_AWAY)
	{
		UIFunction::Set_UIVisibility("Team_Home01", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Team_Home", N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility("Team_Away01", N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility("Team_Away", N3UI_VISIBLE_FALSE);
	}
	else if(myTeam == PLAYER_TEAM_VIEW)
	{
		UIFunction::Set_UIVisibility("Team_Home01", N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility("Team_Home", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Team_Away01", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Team_Away", N3UI_VISIBLE_TRUE);
	}

	// 관전자일 경우 파워, 스테미나 게이지와 스킬게이지 OFF
	if( myTeam == PLAYER_TEAM_VIEW )
	{
		UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "SkillGages", N3UI_VISIBLE_FALSE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "SkillGages", N3UI_VISIBLE_TRUE );
	}

	char sText[STRLEN_64]="";

	for(int i = 0; i < TEAM_SIZE*2; i++)
	{
		_snprintf(sText, STRLEN_64, "Effect_Animation%02d", i);
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	}

	if( m_bNewbieHelp && m_pRoom->m_nMode != ROOM_MODE_REPLAY && myTeam != PLAYER_TEAM_VIEW )
		UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_TRUE);
	else
		UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_FALSE);

	if( m_pRoom->m_nMode == ROOM_MODE_QUEST )
	{
		UIFunction::Set_UIVisibility( "MissionBG", N3UI_VISIBLE_TRUE );
		UIFunction::Set_ControlText( "MissionTitle", CResourceManager::GetPtr()->m_pStaticMap[120002 + (m_pRoom->m_nQuestCode-105)].c_str() );

		UIFunction::Set_ControlText( "MissionContent", CResourceManager::GetPtr()->m_pStaticMap[260005 + (m_pRoom->m_nQuestCode-105)].c_str() );
	}
	else
		UIFunction::Set_UIVisibility( "MissionBG", N3UI_VISIBLE_FALSE );
}

void CBasePlayProcess::FUNCTION_UIClear()
{
	char text[STRLEN_64]="";

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Dialog_CountNumber" );
	assert(pControl);
	pControl->SetVisible(N3UI_VISIBLE_FALSE);

	for(int i = 0; i < ROOM_SCALE_5; i++)
	{
		_snprintf(text, STRLEN_64, "FormMark_%d", i);
		CUIControl* pForm = CClientUIManager::GetPtr()->FindEventControl( text );
		assert(pForm);
		pForm->SetVisible(N3UI_VISIBLE_FALSE);
	}

	CChattingDialog::GetPtr()->ChattingListClear();
	CChattingDialog::GetPtr()->ChattingListDelete();

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	// 리플레이 컨트롤 초기화
	UIFunction::Set_UIVisibility( "Dialog_RemoteMovie", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_RemoteCamera", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_RemoteMin", N3UI_VISIBLE_FALSE );	

	char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;

	if(m_pRoom->m_nMode == ROOM_MODE_REPLAY)
		UIFunction::Set_UIVisibility( "Dialog_RemoteCamera", N3UI_VISIBLE_TRUE );
	else if( myTeam == PLAYER_TEAM_VIEW )
		UIFunction::Set_UIVisibility( "Dialog_RemoteMovie", N3UI_VISIBLE_TRUE );

	//리플레이 마크 초기화
	UIFunction::Set_UIVisibility("ReplayMark",N3UI_VISIBLE_FALSE);

	//골든골 표시 초기화(미니맵 위에)
	UIFunction::Set_UIVisibility("Static_GoldenGoalMark",N3UI_VISIBLE_FALSE);

	//Start 초기화
	UIFunction::Set_UIVisibility("Static_Start", N3UI_VISIBLE_FALSE);

	//골 UI 초기화
	UIFunction::Set_UIVisibility("Dialog_Goal", N3UI_VISIBLE_FALSE);

	// Front 미니맵 초기화
	m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_FRONTVIEW_BG");

	CUIControl* Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");
	assert(Map_Bg);

	CUIControl* MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	for(int i=0;i<TEAM_SIZE*2;i++)
	{
		sprintf(text, "GAMEPLAY_PLAYERMARK%d_IMAGE", i + 1);
		CUIControl* pControl = MiniMap_Bg->FindEventControl(text);
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}

	m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_SIDEVIEW_BG");

	// Side 미니맵 초기화
	Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");
	assert(Map_Bg);

	MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	for(int i=0;i<TEAM_SIZE*2;i++)
	{
		sprintf(text, "GAMEPLAY_PLAYERMARK%d_IMAGE", i + 1);
		CUIControl* pControl = MiniMap_Bg->FindEventControl(text);
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
		InitPositionMarkPoint(i, NULL);
	}

	// 포지션 마크, 네임 OFF
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);

	int	j = 0;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_VIEW)
				continue;

			sprintf(text, "NameMark_%d", j);
			CUIControl* pNameMark = CClientUIManager::GetPtr()->FindEventControl(text);
			if( (myTeam == pAnyPlayer->m_nTeam) && (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
				pNameMark->SetTextColor(0xffA2CFFF);
			else if( (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
				pNameMark->SetTextColor(0xffFF8F8F);
			else
				pNameMark->SetTextColor(0xffffffff);
			j++;
		}
	}

	// Front 미니맵 OFF
	UIFunction::Set_UIVisibility("GAMEPLAY_FRONTVIEW_BG",N3UI_VISIBLE_FALSE);

	// Side 미니맵 OFF
	UIFunction::Set_UIVisibility("GAMEPLAY_SIDEVIEW_BG",N3UI_VISIBLE_FALSE);

	// 파워, 스테미나 게이지 OFF
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );

	// 스킬게이지 OFF
	UIFunction::Set_UIVisibility( "SkillGages", N3UI_VISIBLE_FALSE );
	for(int i=0;i<5;i++)
	{
		_snprintf(text, STRLEN_64, "SkillGageIcon%d", i);
		UIFunction::Set_UIVisibility( text, N3UI_VISIBLE_FALSE );
	}

	// 스코어 보드 OFF
	UIFunction::Set_UIVisibility( "GAMEPLAY_SCORE_BG", N3UI_VISIBLE_FALSE );

	for( int i=0; i<m_snCoolTimeIconCnt; i++ )
	{
		CoolTimeInfo * pCoolTimeInfo = &m_cCoolTimeInfoArray[i];
		pCoolTimeInfo->m_cCoolTimeIcon.SetVisible( false );
	}

	for(int i=0;i<TEAM_SIZE*2;i++)
	{
		_snprintf(text, STRLEN_64, "Effect_Animation%02d", i);
		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(text);
		assert(pAni);
		pAni->SetPlay(false);
		pAni->SetVisible(N3UI_VISIBLE_FALSE);
	}

	// 임시
	if( m_pRoom->m_nMode != ROOM_MODE_REPLAY && m_nState < STATE_GAME_END_READY && myTeam != PLAYER_TEAM_VIEW )
		UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_TRUE);
	else
		UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_FALSE);

	UIFunction::Set_UIVisibility("Progress_Fury", N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility("Fury_Ball_BG", N3UI_VISIBLE_FALSE);

	UIFunction::Set_UIVisibility( "MissionBG", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "PassMe", N3UI_VISIBLE_FALSE );
}

void CBasePlayProcess::FUNCTION_QuickChattingPrint(int i) 
{
	char sText[MAX_PATH] = "";

	srand(time(NULL));
	int nSelect = rand()%2;

	TiXmlNode* pChild = m_pGameOptionXml->FirstChild( "Quick" );
	
	TiXmlElement* pElement = (TiXmlElement*)pChild;

	int j=0;

	while(true) 
	{
		pElement = (TiXmlElement*)pChild;

		if(i==j)
		{
			if( !strcmp(m_pGameOptionXml->Attribute( "UserConf" ), "false") )			
				CChattingDialog::GetPtr()->FUNCTION_ChattingFunctionKeyInGamePlay(pElement->Attribute( "DefaultStr" ));
			else
			{
				if( !strcmp(pElement->Attribute( "IsConf" ), "false") )
					CChattingDialog::GetPtr()->FUNCTION_ChattingFunctionKeyInGamePlay(pElement->Attribute( "Str1" ));	
				else
				{
					if(nSelect)
						CChattingDialog::GetPtr()->FUNCTION_ChattingFunctionKeyInGamePlay(pElement->Attribute( "Str1" ));	
					else
						CChattingDialog::GetPtr()->FUNCTION_ChattingFunctionKeyInGamePlay(pElement->Attribute( "Str2" ));	
				}
			}
			break;
		}
		j++;
		pChild = pChild->NextSibling( "Quick" );
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void CBasePlayProcess::FUNCTION_InitTutorialPlayUI()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	int	i = 0;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_VIEW)
				continue;

			InitPositionMarkPoint(i, pAnyPlayer);
			InitMiniMapMarkPoint(i, pAnyPlayer);
			i++;
		}
	}

	InitMiniMapMarkPoint(CObjBallManager::GetPtr()->GetBallPointer());
}

void CBasePlayProcess::FUNCTION_UpdateTutorialPlayUI()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	int	nIndex = 0;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_VIEW)
				continue;

			UpdatePositionMarkPoint(nIndex, pAnyPlayer);//글짜 버그로 인한 막음
			UpdateMiniMapMarkPoint(nIndex, pAnyPlayer);

			if( m_nState < STATE_REPLAY_READY)
			{
				FUNCTION_SkillEffect(nIndex, pAnyPlayer);
				FUNCTION_SkillEffectUpdate(nIndex, pAnyPlayer);
			}

			++nIndex;
		}
	}

	UpdateMiniMapMarkPoint(CObjBallManager::GetPtr()->GetBallPointer());
}

void CBasePlayProcess::FUNCTION_ChangeTutorialPlayUI()
{
	if(m_nCameraTeam == 0)//사이드 뷰
	{
		m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_FRONTVIEW_BG");
		if(!m_pCameraCurrentBG) return;//튜토리얼 1

		m_pCameraCurrentBG->SetVisible(N3UI_VISIBLE_FALSE);
		m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_SIDEVIEW_BG");
		m_pCameraCurrentBG->SetVisible(N3UI_VISIBLE_TRUE);
	}
	else
	{
		m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_SIDEVIEW_BG");
		if(!m_pCameraCurrentBG) return;//튜토리얼 1
		m_pCameraCurrentBG->SetVisible(N3UI_VISIBLE_FALSE);
		m_pCameraCurrentBG = CClientUIManager::GetPtr()->FindEventControl("GAMEPLAY_FRONTVIEW_BG");
		m_pCameraCurrentBG->SetVisible(N3UI_VISIBLE_TRUE);
	}

	_Assert(m_pCameraCurrentBG, "FUNCTION_ChangeTutorialPlayUI", 0, "", 0);

	FUNCTION_InitTutorialPlayUI();
}

void CBasePlayProcess::InitPositionMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	char szbuff[MAX_PATH];

	sprintf(szbuff, "PositionMark_%d", nPlayer);
	UIFunction::Set_UIVisibility(szbuff, N3UI_VISIBLE_FALSE);

	sprintf(szbuff, "NameMark_%d", nPlayer);
	UIFunction::Set_UIVisibility(szbuff, N3UI_VISIBLE_FALSE);
}

/////////////////////////////////////////////////////////////////////////////////////
void CBasePlayProcess::UpdatePositionMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	char szbuff[MAX_PATH];

	sprintf(szbuff, "PositionMark_%d", nPlayer);
	CUIControl* pPositionMark = CClientUIManager::GetPtr()->FindEventControl(szbuff);

	sprintf(szbuff, "NameMark_%d", nPlayer);
	CUIControl* pNameMark = CClientUIManager::GetPtr()->FindEventControl(szbuff);

	NiCamera* pkCamera = CCameraController::GetPtr()->GetCamera();
	const NiFrustum & kFrustum = pkCamera->GetViewFrustum();
	NiFrustumPlanes kFrustumPlanes( kFrustum, pkCamera->GetWorldTransform() );

	if(m_nState == STATE_CEREMONY_PLAY || !CoreApi_FrustumContains( pAnyPlayer->GetMarkPoint(), kFrustumPlanes ))
	{
		pPositionMark->SetVisible(N3UI_VISIBLE_FALSE);
		pNameMark->SetVisible(N3UI_VISIBLE_FALSE);
		return;
	}

	switch(m_pSelectPlayerInfo->m_cSetting.m_nLabel)
	{
	case 0:
		break;
	case 1:
		pPositionMark->SetVisible(N3UI_VISIBLE_TRUE);
		break;
	case 2:
		pPositionMark->SetVisible(N3UI_VISIBLE_TRUE);
		pNameMark->SetVisible(N3UI_VISIBLE_TRUE);
		break;
	}

	char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;
	char mySeq = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->GetObjSeq();

	if( m_pSelectPlayerInfo->m_nBallHaving == BALL_HAVING_MYTEAM &&	m_pRoom->m_nMode == ROOM_MODE_QUEST && ( 104 < m_pRoom->m_nQuestCode && m_pRoom->m_nQuestCode < 108 ) )
		UIFunction::Set_UIVisibility( "PassMe", N3UI_VISIBLE_TRUE );
	else
		UIFunction::Set_UIVisibility( "PassMe", N3UI_VISIBLE_FALSE );

	//포지션 마크를 캐릭터 위치로 이동시켜 준다
	int ScreenPointX = 0;	int ScreenPointY = 0;
	CCameraController::GetPtr()->WorldPtToScreenPt(pAnyPlayer->GetMarkPoint(), ScreenPointX, ScreenPointY);

	if(myTeam == PLAYER_TEAM_VIEW)
		myTeam = PLAYER_TEAM_HOME;

	if(pAnyPlayer->m_nTeam == myTeam)//
		UICommonMethod::PositionIconDraw2( pPositionMark, pAnyPlayer->m_nPosition, UICommonMethod::HOME_TEAM );
	else
		UICommonMethod::PositionIconDraw2( pPositionMark, pAnyPlayer->m_nPosition, UICommonMethod::AWAY_TEAM );

	int len = strlen(pAnyPlayer->m_sName);
	int Width = pNameMark->GetWidth()*len/14/2;
	pNameMark->SetText(pAnyPlayer->m_sName);

	if( (myTeam == pAnyPlayer->m_nTeam) && (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
		pNameMark->SetTextColor(0xffA2CFFF);
	else if( (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
		pNameMark->SetTextColor(0xffFF8F8F);
	else
		pNameMark->SetTextColor(0xffffffff);

	if(m_pSelectPlayerInfo->m_cSetting.m_nLabel == 1)
		pPositionMark->SetLocation(ScreenPointX - pPositionMark->GetWidth()/2, ScreenPointY + 22);
	else
		pPositionMark->SetLocation(ScreenPointX - Width - pPositionMark->GetWidth() + 3, ScreenPointY + 22);

	pNameMark->SetLocation(ScreenPointX - pNameMark->GetWidth()/2 + 8, ScreenPointY + 22);

	//// 윈도우 화면을 벗어낫을 경우 보이지 않음
	//RECT rt={0,};
	//POINT pt={0,};
	//pPositionMark->GetLocation(pt);
	//GetClientRect(CClientUIManager::GetPtr()->m_hWnd, &rt);
	//if( (rt.left > pt.x) || (rt.right < pt.x) || (rt.top > pt.y) || (rt.bottom < pt.y) )
	//	pNameMark->SetVisible(N3UI_VISIBLE_FALSE);
}

void CBasePlayProcess::InitMiniMapMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	char szbuff[MAX_PATH];
	char text[MAX_PATH];

	CUIControl* Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");

	_Assert(Map_Bg, "Map_Bg", 0, "", 0);

	CUIControl* MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	sprintf(szbuff, "GAMEPLAY_PLAYERMARK%d_IMAGE", nPlayer + 1);
	CUIControl* pControl = MiniMap_Bg->FindEventControl(szbuff);
	pControl->SetVisible(N3UI_VISIBLE_TRUE);

	//포지션 마크를 캐릭터 위치로 이동시켜 준다
	SetMiniMapMarkPoint(pControl, pAnyPlayer->GetMarkPoint());

	char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;

	if(myTeam == PLAYER_TEAM_VIEW)
		myTeam = PLAYER_TEAM_HOME;

	if(pAnyPlayer->m_nPosition == POSITION_GK)//키퍼
		sprintf(text, "GAMEPLAY_GKPLAYER_IMAGE");//녹색
	else if(myTeam == pAnyPlayer->m_nTeam)//같은팀
		sprintf(text, "GAMEPLAY_APLAYER_IMAGE");//파랑색
	else//상대팀
		sprintf(text, "GAMEPLAY_EPLAYER_IMAGE");//빨강색

	pControl->SetElement((CClientUIManager::GetPtr()->FindEventControl(text))->m_pElement->Clone());

	CUIControl* pHaving = MiniMap_Bg->FindEventControl("GAMEPLAY_BALLOWNPLAYER_BG");
	if(pAnyPlayer->m_nBallHaving == BALL_HAVING_MINE)//볼을 소유했을경우
	{
		pHaving->SetVisible(N3UI_VISIBLE_TRUE);
		SetMiniMapMarkPoint(pHaving, pAnyPlayer->GetMarkPoint());
	}
	else if(pAnyPlayer->m_nBallHaving == BALL_HAVING_NONE)
		pHaving->SetVisible(N3UI_VISIBLE_FALSE);
}

void CBasePlayProcess::InitMiniMapMarkPoint(CObjBallInfo* pBallInfo)
{
	CUIControl* Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");
	_Assert(Map_Bg, "Map_Bg", 0, "", 0);

	CUIControl* MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	CUIControl* pControl = MiniMap_Bg->FindEventControl("GAMEPLAY_BALLMARK_IMAGE");

	if(pBallInfo->m_nState != BALL_STATE_MOTION)//볼이 모션일땐 꺼야된다
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	else
	{
		pControl->SetVisible(N3UI_VISIBLE_TRUE);
		SetMiniMapMarkPoint(pControl, pBallInfo->m_cCurrentPoint);//포지션 마크를 캐릭터 위치로 이동시켜 준다
	}
}

void CBasePlayProcess::UpdateMiniMapMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	char szbuff[MAX_PATH];
	int	nHaving;

	CUIControl* Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");
	_Assert(Map_Bg, "Map_Bg", 0, "", 0);

	CUIControl* MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	sprintf(szbuff, "GAMEPLAY_PLAYERMARK%d_IMAGE", nPlayer + 1);
	CUIControl* pControl = MiniMap_Bg->FindEventControl(szbuff);
	pControl->SetVisible(N3UI_VISIBLE_TRUE);

	SetMiniMapMarkPoint(pControl, pAnyPlayer->GetMarkPoint());

	CUIControl* pHaving = MiniMap_Bg->FindEventControl("GAMEPLAY_BALLOWNPLAYER_BG");

	nHaving = GetBallHaving(pAnyPlayer);
	if(nHaving == BALL_HAVING_MINE)//볼을 소유했을경우
	{
		pHaving->SetVisible(N3UI_VISIBLE_TRUE);
		SetMiniMapMarkPoint(pHaving, pAnyPlayer->GetMarkPoint());
		FUNCTION_UpdatePlayerHasGoalNameUI(pAnyPlayer);
		m_pCameraCurrentBG->FindEventControl( "GAMEPLAY_PLAYERNAME_BG" )->SetVisible(N3UI_VISIBLE_TRUE);
	}
	else if(nHaving == BALL_HAVING_NONE)
	{
		m_pCameraCurrentBG->FindEventControl( "GAMEPLAY_PLAYERNAME_BG" )->SetVisible(N3UI_VISIBLE_FALSE);
		pHaving->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

void CBasePlayProcess::UpdateMiniMapMarkPoint(CObjBallInfo* pBallInfo)
{
	CUIControl* Map_Bg = m_pCameraCurrentBG->FindEventControl("GAMEPLAY_MAP_BG");
	_Assert(Map_Bg, "Map_Bg", 0, "", 0);

	CUIControl* MiniMap_Bg = Map_Bg->FindEventControl("GAMEPLAY_MINIMAP_BG");

	CUIControl* pControl = MiniMap_Bg->FindEventControl("GAMEPLAY_BALLMARK_IMAGE");

	if(pBallInfo->m_nState != BALL_STATE_MOTION)//볼이 모션일땐 꺼야된다
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	else
	{
		FUNCTION_UpdatePlayerHasGoalNameUI(NULL);
		pControl->SetVisible(N3UI_VISIBLE_TRUE);
		SetMiniMapMarkPoint(pControl, pBallInfo->m_cCurrentPoint);
	}
}

void CBasePlayProcess::SetMiniMapMarkPoint(CUIControl* pControl, NiPoint3 cPoint)
{
	float biX = 90.f/CFblArea::GetPtr()->GetGroundX();
	float biY = 60.f/CFblArea::GetPtr()->GetGroundY();

	if(m_nCameraTeam == 0)
		pControl->SetLocation(static_cast<int>(90.f + cPoint.x*biX) - 5, static_cast<int>(60.f - cPoint.y*biY) - 5, false);
	if(m_nCameraTeam == 1)
		pControl->SetLocation(static_cast<int>(60.f - cPoint.y*biY) - 5, static_cast<int>(90.f - cPoint.x*biX) - 5, false);
	if(m_nCameraTeam == 2)
		pControl->SetLocation(static_cast<int>(60.f + cPoint.y*biY) - 5, static_cast<int>(90.f + cPoint.x*biX) - 5, false);
}

void CBasePlayProcess::FUNCTION_UpdateTutorialPlayScoreUI()
{
	char text[MAX_PATH];
	//10의 자리
	sprintf(text, "Num_%d", (m_pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]/10)%10);
	UIFunction::ChangeTexture("GAMEPLAY_SCOREMARK1_IMAGE", text);
	//1의 자리
	sprintf(text, "Num_%d", (m_pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]/1)%10);
	UIFunction::ChangeTexture("GAMEPLAY_SCOREMARK2_IMAGE", text);
	//10의 자리
	sprintf(text, "Num_%d", (m_pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]/10)%10);
	UIFunction::ChangeTexture("GAMEPLAY_SCOREMARK3_IMAGE", text);
	//1의 자리
	sprintf(text, "Num_%d", (m_pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]/1)%10);
	UIFunction::ChangeTexture("GAMEPLAY_SCOREMARK4_IMAGE", text);
}

void CBasePlayProcess::FUNCTION_UpdateTutorialPlayTimeUI()
{
	char sText[STRLEN_32]="";

	_snprintf( sText, STRLEN_32, "SWNum_%d", m_pRoom->GetPlayTime()/600 );
	UIFunction::ChangeTexture( "PlayTimeH00", sText );
	_snprintf( sText, STRLEN_32, "SWNum_%d", (m_pRoom->GetPlayTime()/60)%10 ); 
	UIFunction::ChangeTexture( "PlayTimeH0", sText );
	_snprintf( sText, STRLEN_32, "SWNum_%d", (m_pRoom->GetPlayTime()%60)/10 );
	UIFunction::ChangeTexture( "PlayTimeM00", sText );
	_snprintf( sText, STRLEN_32, "SWNum_%d", (m_pRoom->GetPlayTime()%60)%10 );
	UIFunction::ChangeTexture( "PlayTimeM0", sText );
}

void CBasePlayProcess::FUNCTION_UpdatePlayerHasGoalNameUI(CObjPlayerInfo* pAnyPlayer)
{
	//if(m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_HOME && m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_AWAY) return;

	char sName[STRLEN_32]={0,};

	CUIControl* pPlayerNameSide=CClientUIManager::GetPtr()->FindEventControl("Static_PlayerName_Side");
	CUIControl* pPlayerNameDown=CClientUIManager::GetPtr()->FindEventControl("Static_PlayerName_Down");

	if(pAnyPlayer==NULL)
	{
		UIFunction::Set_UIVisibility("Static_PlayerPosition_Side",N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Static_PlayerPosition_Down",N3UI_VISIBLE_FALSE);
	}
	else
	{
		UIFunction::Set_UIVisibility("Static_PlayerPosition_Side",N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility("Static_PlayerPosition_Down",N3UI_VISIBLE_TRUE);

		char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;

		if( myTeam == PLAYER_TEAM_VIEW )
			myTeam = PLAYER_TEAM_HOME;


		UICommonMethod::DrawPositionIcon2("Static_PlayerPosition_Side",pAnyPlayer->m_nPosition,pAnyPlayer->m_nTeam==myTeam);
		UICommonMethod::DrawPositionIcon2("Static_PlayerPosition_Down",pAnyPlayer->m_nPosition,pAnyPlayer->m_nTeam==myTeam);

		if( (myTeam == pAnyPlayer->m_nTeam) && (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
		{
			pPlayerNameSide->SetTextColor(0xffA2CFFF);
			pPlayerNameDown->SetTextColor(0xffA2CFFF);
			
			UIFunction::ChangeTexture( "Shadow_Down", "ShadowBlue" );
			UIFunction::ChangeTexture( "Shadow_Side", "ShadowBlue" );
		}
		else if( (pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT || pAnyPlayer->GetKind() == PLAYER_KIND_USERBOT || pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT) )
		{
			pPlayerNameSide->SetTextColor(0xffff8F8F);
			pPlayerNameDown->SetTextColor(0xffff8F8F);

			UIFunction::ChangeTexture( "Shadow_Down", "ShadowRed" );
			UIFunction::ChangeTexture( "Shadow_Side", "ShadowRed" );
		}
		else
		{
			pPlayerNameSide->SetTextColor(0xffffffff);
			pPlayerNameDown->SetTextColor(0xffffffff);
		}

		sprintf(sName,"%s",pAnyPlayer->m_sName);
	}
	pPlayerNameSide->SetText(sName);
	pPlayerNameDown->SetText(sName);
}

void CBasePlayProcess::ShowGoalInfo()
{
	CObjBallInfo*		pBall			= CObjBallManager::GetPtr()->GetBallPointer();
	CObjPlayerInfo*		pKickPlayer		= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nKickSeq);
	CObjPlayerInfo*		pAssistPlayer	= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nAssistSeq);

	if(pKickPlayer == NULL)
		return;

	switch(pBall->m_nGoalType)
	{
	case GOAL_TYPE_NORMAL: //일반골
	case GOAL_TYPE_GOLDEN: //골든골
		{
			if(pAssistPlayer == NULL)
			{}
			else if(pKickPlayer->m_nTeam != pAssistPlayer->m_nTeam)
				pAssistPlayer = NULL;
		}
		break;
	case GOAL_TYPE_OWN: //자살골
		{
			pAssistPlayer = NULL;
		}
		break;
	default:
		return;
	}

	CUIControl* pStaticGoalPlayer=CClientUIManager::GetPtr()->FindEventControl("Static_GoalPlayer");
	CUIControl* pGoalPlayerPositionIcon=pStaticGoalPlayer->FindEventControl("Static_Position");
	bool bGoalPlayerTeam=(pKickPlayer->m_nTeam==PLAYER_TEAM_HOME);
	UICommonMethod::PositionIconDraw( "Static_GoalPlayerPosition", pKickPlayer->m_nPosition, bGoalPlayerTeam );

	CResMessageTableFormat* pMTable = NULL;

	char sGoalPlayerLevel[STRLEN_16];

	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);

	sprintf(sGoalPlayerLevel, pMTable->m_sMessage, pKickPlayer->m_cLevel.m_nLevel);
	pStaticGoalPlayer->FindEventControl("Static_Level")->SetText(sGoalPlayerLevel);
	pStaticGoalPlayer->FindEventControl("Static_Name")->SetText(pKickPlayer->m_sName);

	CUIControl* pStaticAssistPlayer=CClientUIManager::GetPtr()->FindEventControl("Static_AssistPlayer");
	CUIControl* pAssistPlayerPositionIcon=pStaticAssistPlayer->FindEventControl("Static_Position");

	char sText[STRLEN_32]="";

	if((pAssistPlayer == NULL) || (pAssistPlayer == pKickPlayer))
	{
		UIFunction::Set_UIVisibility("Static_AssistPlayerPosition",N3UI_VISIBLE_FALSE);
		pStaticAssistPlayer->FindEventControl("Static_Level")->SetText("");
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050 ,0));
		assert(pMTable);
		pStaticAssistPlayer->FindEventControl("Static_Name")->SetText(pMTable->m_sMessage);

		_snprintf( sText, STRLEN_32, "%d", m_pRoom->m_nGoalerScore );
		UIFunction::Set_ControlText( "Goal_History", sText );

		UIFunction::Set_UIVisibility( "Assist_History", N3UI_VISIBLE_FALSE );
	}
	else
	{
		UIFunction::Set_UIVisibility("Static_AssistPlayerPosition",N3UI_VISIBLE_TRUE);
		bool bAssistPlayerTeam=(pAssistPlayer->m_nTeam==PLAYER_TEAM_HOME);
		UICommonMethod::PositionIconDraw( "Static_AssistPlayerPosition", pAssistPlayer->m_nPosition, bAssistPlayerTeam );

		char sAssistPlayerLevel[STRLEN_16];
		
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
		assert(pMTable);

		sprintf(sAssistPlayerLevel, pMTable->m_sMessage, pAssistPlayer->m_cLevel.m_nLevel);
		pStaticAssistPlayer->FindEventControl("Static_Level")->SetText(sAssistPlayerLevel);
		pStaticAssistPlayer->FindEventControl("Static_Name")->SetText(pAssistPlayer->m_sName);

		UIFunction::Set_UIVisibility( "Assist_History", N3UI_VISIBLE_TRUE );

		_snprintf( sText, STRLEN_32, "%d", m_pRoom->m_nGoalerScore );
		UIFunction::Set_ControlText( "Goal_History", sText );

		_snprintf( sText, STRLEN_32, "%d", m_pRoom->m_nAssisterScore );
		UIFunction::Set_ControlText( "Assist_History", sText );
	}

	Set_GoalType(pKickPlayer->m_nTeam, pBall->m_nGoalType);
	Set_GoalInTime(m_pRoom->m_fCurrentTime+1.0f);

	UIFunction::Set_UIVisibility("Dialog_Goal",N3UI_VISIBLE_TRUE);
}

void CBasePlayProcess::Set_GoalType(const DWORD& nShootingPlayerTeam, const DWORD& nGoalType)
{
	UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",0, N3UI_VISIBLE_FALSE);
	UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",1, N3UI_VISIBLE_FALSE);
	UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",2, N3UI_VISIBLE_FALSE);
	UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",3, N3UI_VISIBLE_FALSE);

	switch(nGoalType)
	{
	case GOAL_TYPE_NORMAL:
		if(nShootingPlayerTeam==PLAYER_TEAM_HOME)
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_TRUE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_FALSE);
		}
		else
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_TRUE);
		}
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",0,N3UI_VISIBLE_TRUE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",1,N3UI_VISIBLE_TRUE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",2,N3UI_VISIBLE_FALSE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",3,N3UI_VISIBLE_FALSE);
		break;
	case GOAL_TYPE_GOLDEN:
		if(nShootingPlayerTeam==PLAYER_TEAM_HOME)
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_TRUE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_FALSE);;
		}
		else
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_TRUE);
		}
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",0,N3UI_VISIBLE_FALSE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",1,N3UI_VISIBLE_TRUE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",2,N3UI_VISIBLE_TRUE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",3,N3UI_VISIBLE_FALSE);
		break;
	case GOAL_TYPE_OWN:
		if(nShootingPlayerTeam==PLAYER_TEAM_HOME)
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_TRUE);
		}
		else
		{
			UIFunction::Set_UIVisibility( "Static_GoalTeam0", N3UI_VISIBLE_TRUE);
			UIFunction::Set_UIVisibility( "Static_GoalTeam1", N3UI_VISIBLE_FALSE);;
		}
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",0,N3UI_VISIBLE_FALSE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",1,N3UI_VISIBLE_FALSE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",2,N3UI_VISIBLE_FALSE);
		UIFunction::Set_ChildNumVisibility("Dialog_Goal","Static_Icon0",3,N3UI_VISIBLE_TRUE);
		break;
	default:
		break;
	}
}

void CBasePlayProcess::Set_GoalInTime(const float& fPlayTime)
{
	char text[MAX_PATH];
	//10의 자리
	sprintf(text, "BSNum_%d", static_cast<DWORD>(fPlayTime/600));
	UIFunction::ChangeTexture("GAMEPLAY_TIMEMARK1_IMAGE", text);
	//1의 자리
	sprintf(text, "BSNum_%d", static_cast<DWORD>(fPlayTime/60)%10);
	UIFunction::ChangeTexture("GAMEPLAY_TIMEMARK2_IMAGE", text);
	//10의 자리
	sprintf(text, "BSNum_%d", (static_cast<DWORD>(fPlayTime)%60)/10);
	UIFunction::ChangeTexture("GAMEPLAY_TIMEMARK3_IMAGE", text);
	//1의 자리
	sprintf(text, "BSNum_%d", (static_cast<DWORD>(fPlayTime)%60)%10);
	UIFunction::ChangeTexture("GAMEPLAY_TIMEMARK4_IMAGE", text);
}

void CBasePlayProcess::HideGoalInfo()
{
	UIFunction::Set_UIVisibility("Dialog_Goal",N3UI_VISIBLE_FALSE);
}

void CBasePlayProcess::DisplayRemainTime(const DWORD& nRemainTime)
{
	char sNumOne[STRLEN_32];
	char sNumTen[STRLEN_32];

	if(nRemainTime <=10)
	{
		sprintf(sNumOne,"RNum_%d",nRemainTime%10);
		sprintf(sNumTen,"RNum_%d",nRemainTime/10);
		UIFunction::ChangeTexture("Static_RNumber1", sNumOne);
		UIFunction::ChangeTexture("Static_RNumber0", sNumTen);
	}
	else
	{
		sprintf(sNumOne,"ONum_%d",nRemainTime%10);
		sprintf(sNumTen,"ONum_%d",nRemainTime/10);
		UIFunction::ChangeTexture("Static_RNumber1", sNumOne);
		UIFunction::ChangeTexture("Static_RNumber0", sNumTen);
	}
}

void CBasePlayProcess::CoolTimeIconInit()
{
	char buf[256];

	LPDIRECT3DDEVICE9 pDevice = CClientUIManager::GetPtr()->GetD3DDevice();
	assert( pDevice != 0 );

	CUIControl * pSkillGages = CClientUIManager::GetPtr()->FindEventControl( "SkillGages" );
	for( int i=0; i<m_snCoolTimeIconCnt; i++ )
	{
		sprintf( buf, "SkillGageIcon%d", i );

		CUIControl * pSkillGageIcon = pSkillGages->FindEventControl( buf );
		assert( pSkillGageIcon != 0 );        

		CUIControl * pSkillCoolTimeImage = pSkillGageIcon->FindEventControl( "Static_SkillImage" );

		assert( pSkillCoolTimeImage != 0 );
		if( pSkillCoolTimeImage == 0 ) continue;

		//pSkillCoolTimeImage->SetVisible( N3UI_VISIBLE_FALSE );

		CoolTimeInfo * pCoolTimeInfo = &m_cCoolTimeInfoArray[i];

		pCoolTimeInfo->m_cCoolTimeIcon.SetRotate( CoolTimeIcon::ROTATE::eLeft );
		pCoolTimeInfo->m_cCoolTimeIcon.SetUIControl( pSkillCoolTimeImage );
		pCoolTimeInfo->m_pControl = pSkillCoolTimeImage;

		pCoolTimeInfo->m_cCoolTimeIcon.SetDevice( pDevice );
        pCoolTimeInfo->m_cCoolTimeIcon.SetColor( D3DCOLOR_ARGB( 125, 0, 0, 0 ) );
	}
}

void CBasePlayProcess::CoolTimeIconUpdate()
{
	//return;
	if(m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_HOME && m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_AWAY) return;

	VectorSkillList * pSkillList = &m_pSelectPlayerInfo->m_vSkillList;

	int nCatchCount = 0;
	CUIControl * pSkillGages = CClientUIManager::GetPtr()->FindEventControl( "SkillGages" );
	for( size_t i=0; i<pSkillList->size(); i++ )
	{
		CObjSkillInfo * pSkillInfo =  (*pSkillList)[i];
		if( pSkillInfo == 0 ) continue;
		if( pSkillInfo->m_fCoolTime == 0.0f ) continue;

		if( nCatchCount >= m_snCoolTimeIconCnt ) break;

		CoolTimeInfo * pCoolTimeInfo = &m_cCoolTimeInfoArray[ nCatchCount ];

		char buf[128];   

		sprintf( buf, "SkillGageIcon%d", nCatchCount );

		CUIControl * pSkillGageIcon = pSkillGages->FindEventControl( buf );
		CUIControl * pSkillImage    = pSkillGageIcon->FindEventControl( "Static_SkillImage" );

		CUIControl * pSkillCoolTimeSecond = pSkillGageIcon->FindEventControl( "Static_SkillSecond" );

		pSkillGageIcon->SetVisible( N3UI_VISIBLE_TRUE );
		pCoolTimeInfo->m_cCoolTimeIcon.SetVisible( true );

		sprintf( buf, "%d", int( pSkillInfo->m_fEndTime - pSkillInfo->m_fCoolTime) );

		float fRemainTime = pSkillInfo->m_fEndTime - pSkillInfo->m_fCoolTime;
		if( fRemainTime <= 3.0f  ) 
			pSkillCoolTimeSecond->SetTextColor( D3DCOLOR( D3DCOLOR_ARGB(255, 255, 0, 0 )) );
		else
			pSkillCoolTimeSecond->SetTextColor( D3DCOLOR( D3DCOLOR_ARGB(255, 255, 255, 255 )) );

		pSkillCoolTimeSecond->SetText( buf );

		pCoolTimeInfo->m_bEnable = true;        

		CResSkillTableFormat * pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pSkillInfo->m_nCode, 0));
		assert(pSkillTable);
		char szIconFile[MAX_PATH];
		sprintf( szIconFile, "Icon/Skill/%s.png", pSkillTable->m_sIconFile );

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pSkillImage, szIconFile );

		CoolTimeIcon * pCoolTimeIcon = &pCoolTimeInfo->m_cCoolTimeIcon;

		pCoolTimeIcon->SetStart( 0.0f );
		pCoolTimeIcon->SetEnd( pSkillInfo->m_fEndTime );

		pCoolTimeIcon->Update( pSkillInfo->m_fCoolTime );

		nCatchCount++;
	}

	for( int i=nCatchCount; i<m_snCoolTimeIconCnt; i++ )
	{
		CoolTimeInfo * pCoolTimeInfo = &m_cCoolTimeInfoArray[ i ];

		char buf[128];

		sprintf( buf, "SkillGageIcon%d", i );
		CUIControl * pSkillGageIcon = pSkillGages->FindEventControl( buf );

		pSkillGageIcon->SetVisible( N3UI_VISIBLE_FALSE );
		pCoolTimeInfo->m_cCoolTimeIcon.SetVisible( false );

		pCoolTimeInfo->m_bEnable = false;        
	}
}

void CBasePlayProcess::DrawSkillCoolTimeInfo()
{
	for( int i=0; i<m_snCoolTimeIconCnt; i++ )
	{
		CoolTimeInfo * pCoolTimeInfo = &m_cCoolTimeInfoArray[ i ];
		if( pCoolTimeInfo->m_bEnable == false ) continue;

		CoolTimeIcon * pCoolTimeIcon = &pCoolTimeInfo->m_cCoolTimeIcon;
		
		CUIControl * pSkillGages = CClientUIManager::GetPtr()->FindEventControl( "SkillGages" );
		if( pSkillGages->GetVisible() == N3UI_VISIBLE_TRUE )
			pCoolTimeIcon->Draw();
	}	
}

void CBasePlayProcess::SetShadowCamera()
{
	NiCamera * pCamera = CCameraController::GetPtr()->GetCamera();
	CResGroundFormat * pGround = CObjectModelManager::GetPtr()->GetGround();

	pGround->SetViewCamera( pCamera );
}


void CBasePlayProcess::FUNCTION_SkillEffect(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	if(!strlen(pAnyPlayer->m_sHeadwordFile))	return;

	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Effect_Animation%02d", nPlayer);
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pAni);

	CUIControl* pEffect0 = pAni->FindEventControl("Excellent00");
	assert(pEffect0);

	CUIControl* pSkillStatic = CClientUIManager::GetPtr()->FindEventControl(pAnyPlayer->m_sHeadwordFile);
	assert(pSkillStatic);

	pEffect0->SetElement(pSkillStatic->m_pElement->Clone());

	memset(pAnyPlayer->m_sHeadwordFile, 0, sizeof(pAnyPlayer->m_sHeadwordFile));

	//스킬이펙트를 캐릭터 위치로 이동시켜 준다
	int ScreenPointX = 0;	int ScreenPointY = 0;
	CCameraController::GetPtr()->WorldPtToScreenPt(pAnyPlayer->GetSkillMarkPoint(), ScreenPointX, ScreenPointY);

	pAni->SetFrame(0);
	pAni->SetVisible(N3UI_VISIBLE_TRUE);
	pAni->SetLocation(ScreenPointX-pAni->GetWidth()/2, ScreenPointY-pAni->GetHeight(), true);
	pAni->SetPlay(true);
}

void CBasePlayProcess::FUNCTION_SkillEffectUpdate(int nPlayer, CObjPlayerInfo* pAnyPlayer)
{
	//스킬이펙트를 캐릭터 위치로 이동시켜 준다
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Effect_Animation%02d", nPlayer);
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(sText);

	if(pAni->GetVisible() == N3UI_VISIBLE_FALSE)	return;

	int ScreenPointX = 0;	int ScreenPointY = 0;
	CCameraController::GetPtr()->WorldPtToScreenPt(pAnyPlayer->GetSkillMarkPoint(), ScreenPointX, ScreenPointY);
	pAni->SetLocation(ScreenPointX-pAni->GetWidth()/2, ScreenPointY-pAni->GetHeight(), true);
}

void CBasePlayProcess::FUNCTION_CameraChange(int nType)
{
	switch(nType)
	{
	case D2WAYTILTMOVE:
		UIFunction::Set_ControlText( "Static_Camera", "2WAY" );
		UIFunction::Set_ControlText( "Static_Cameras", "2WAY" );
		break;
	case D4WAYTILTMOVE:
		UIFunction::Set_ControlText( "Static_Camera", "4WAY" );
		UIFunction::Set_ControlText( "Static_Cameras", "4WAY" );
		break;
	case D4WAY45TURNMOVE:
		UIFunction::Set_ControlText( "Static_Camera", "4WAY 45˚" );
		UIFunction::Set_ControlText( "Static_Cameras", "4WAY 45˚" );
		break;
	case D4WAY315TURNMOVE:
		UIFunction::Set_ControlText( "Static_Camera", "4WAY -45˚" );
		UIFunction::Set_ControlText( "Static_Cameras", "4WAY -45˚" );
		break;
	case D4WAYTOPMOVE:
		UIFunction::Set_ControlText( "Static_Camera", "4WAY TOP" );
		UIFunction::Set_ControlText( "Static_Cameras", "4WAY TOP" );
		break;
	case ROTATECORNERFIX:
		UIFunction::Set_ControlText( "Static_Camera", "회전코너뷰" );
		UIFunction::Set_ControlText( "Static_Cameras", "회전코너뷰" );
		break;
	case ROTATEENDFIX:
		UIFunction::Set_ControlText( "Static_Camera", "회전뷰" );
		UIFunction::Set_ControlText( "Static_Cameras", "회전뷰" );
		break;
	}
}

void CBasePlayProcess::FUNCTION_ZoomChange(int nType)
{
	switch(nType)
	{
	case CAMERAZOOM_0:
		UIFunction::Set_ControlText( "Static_Zoom", "Zoom 1" );
		UIFunction::Set_ControlText( "Static_Zooms", "Zoom 1" );
		break;
	case CAMERAZOOM_1:
		UIFunction::Set_ControlText( "Static_Zoom", "Zoom 2" );
		UIFunction::Set_ControlText( "Static_Zooms", "Zoom 2" );
		break;
	case CAMERAZOOM_2:
		UIFunction::Set_ControlText( "Static_Zoom", "Zoom 3" );
		UIFunction::Set_ControlText( "Static_Zooms", "Zoom 3" );
		break;
	case CAMERAZOOM_3:
		UIFunction::Set_ControlText( "Static_Zoom", "Zoom 4" );
		UIFunction::Set_ControlText( "Static_Zooms", "Zoom 4" );
		break;
	}
}

void CBasePlayProcess::FUNCTION_CameraTeamChange(int nType)
{
	CUIButton* pHome = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("HomeButton"));
	assert(pHome);
	CUIButton* pSide = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("SideButton"));
	assert(pSide);
	CUIButton* pAway = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("AwayButton"));
	assert(pAway);

	pHome->SetPressed(false);
	pSide->SetPressed(false);
	pAway->SetPressed(false);

	switch(nType)
	{
	case CAMERATEAM_NONE:
		pSide->SetPressed(true);
		break;
	case CAMERATEAM_HOME:
		pHome->SetPressed(true);
		break;
	case CAMERATEAM_AWAY:
		pAway->SetPressed(true);
		break;
	}
}

void CBasePlayProcess::FUNCTION_CameraTargetChange(int nType)
{
	switch(nType)
	{
	case CAMERATARGET_BALL:
		UIFunction::Set_ControlText( "Static_Target", "BALL" );
		UIFunction::Set_ControlText( "Static_Targets", "BALL" );
		break;
	case CAMERATARGET_BALLTOCHAR:
		UIFunction::Set_ControlText( "Static_Target", "BALL + USER" );
		UIFunction::Set_ControlText( "Static_Targets", "BALL + USER" );
		break;
	case CAMERATARGET_CHAR:
		UIFunction::Set_ControlText( "Static_Target", "USER" );
		UIFunction::Set_ControlText( "Static_Targets", "USER" );
		break;
	case CAMERATARGET_REGION:
		UIFunction::Set_ControlText( "Static_Target", "REGION" );
		UIFunction::Set_ControlText( "Static_Targets", "REGION" );
		break;
	}
}

void CBasePlayProcess::UpdateFormNumber(bool bView, char nTeam, int nObjSeq, char nHave)
{
	char sText[MAX_PATH]="";

	int nLimit=0;

	switch(nHave)
	{
	case 1:
		nLimit = 0;
		break;
	case 2:
		nLimit = 1;
		break;
	case 3:
		nLimit = 4;
		break;
	}

	int nViewCnt=0;

	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->m_nTeam != nTeam)
				continue;
			if( pAnyPlayer->GetObjSeq() == nObjSeq )
				continue;
			
			if(pAnyPlayer->m_nForm == 5)
				continue;

			if( nLimit<nViewCnt )
				continue;

			_snprintf(sText, MAX_PATH, "FormMark_%d", pAnyPlayer->m_nForm);
			CUIControl* pForm = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pForm);
			_snprintf(sText, MAX_PATH, "Num_%d", pAnyPlayer->m_nForm+1);
			CUIControl* pNumber = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pNumber);

			pForm->SetElement(pNumber->m_pElement->Clone());

			if( bView )
			{
				//포지션 마크를 캐릭터 위치로 이동시켜 준다
				int ScreenPointX = 0;	int ScreenPointY = 0;
				CCameraController::GetPtr()->WorldPtToScreenPt(pAnyPlayer->GetMarkPoint(), ScreenPointX, ScreenPointY);

				pForm->SetLocation(ScreenPointX-8, ScreenPointY+5);
				pForm->SetVisible(N3UI_VISIBLE_TRUE);
			}
			else
				pForm->SetVisible(N3UI_VISIBLE_FALSE);

			RECT rt={0,};
			POINT pt={0,};
			pForm->GetLocation(pt);
			GetClientRect(CClientUIManager::GetPtr()->m_hWnd, &rt);
			if( (rt.left > pt.x) || (rt.right+16 < pt.x) || (rt.top > pt.y) || (rt.bottom+16 < pt.y) )
				pForm->SetVisible(N3UI_VISIBLE_FALSE);

			nViewCnt++;
		}
	}
}

void CBasePlayProcess::AngerAnimation()
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl( "AngerAni" );
	assert(pAni);

	if( 15 == pAni->GetFrame() )
		NormalSound(51001);

	if( 20 < (int)m_pRoom->m_fTotalAngerTime )
	{
		if( 18 <= pAni->GetFrame() )
		{
			pAni->SetVisible(N3UI_VISIBLE_TRUE);
			pAni->SetPlay(true, true, false);
		}
	}
	
}