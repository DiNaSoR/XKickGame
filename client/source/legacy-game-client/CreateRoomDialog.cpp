#include "StdAfx.h"
#include "CreateRoomDialog.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "../InterfaceLibrary/UIIMEEDITBox.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

CCreateRoomDialog::CCreateRoomDialog(void)
{
	m_nTabIndex	= 0;
}

CCreateRoomDialog::~CCreateRoomDialog(void)
{
}

void CCreateRoomDialog::Terminate()
{
}

void CCreateRoomDialog::CreateScene()
{
}

bool CCreateRoomDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return true;
}

void CCreateRoomDialog::FUNCTION_CreateRoomInit()
{
	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	FUNCTION_TabChange(m_nTabIndex);

	// 위장막 걷기
	UIFunction::Set_UIVisibility( "CR_Stc_Disable", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Position_Disable", N3UI_VISIBLE_FALSE );

	// 게임선택
	//UICommonMethod::GameChange(0);

	// 방제
	CUIIMEEditBox* pTitleEditBox = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Ime_Title"));
	assert(pTitleEditBox);
	pTitleEditBox->ClearText();

	// 비번방 체크
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Pass"));
	pChkBox->SetCheckValue(false);

	// 비번 에디트 박스
	CUIEditBox* pEditBox = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Pass"));
	assert(pEditBox);
	pEditBox->ClearText();
	UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_FALSE );

	// 인원설정
	UICommonMethod::ManCountChange(0);
	
	// 인공지능설정
	UICommonMethod::AiChange(1);

	// 레벨 범위 설정
	UIFunction::Set_ControlText( "CR_Min_Lv", "1");
	UIFunction::Set_ControlText( "CR_Max_Lv", "60");

	// 포지션 설정
	FUNCTION_CreateRoomFW(0, "");
	FUNCTION_CreateRoomMF(0, "");
	FUNCTION_CreateRoomDF(0, "");
	FUNCTION_CreateRoomALL1(0, "");
	FUNCTION_CreateRoomALL2(0, "");
	FUNCTION_CreateRoomGK(0, "");

	CUIComboBox* pComboBox = NULL;

	char sText[STRLEN_64]="";

	// 포지션 설정
	for(int i=0;i<6;i++) {
		_snprintf(sText, STRLEN_64, "GRATEROOM_POSITONSETTING%d_LISTBOX", i+1);
		pComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
        assert(pComboBox);
		pComboBox->SetSelectedByIndex(0);
	}

	// 선공 설정
	pComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Cb_Attack"));
	assert(pComboBox);
	pComboBox->SetSelectedByIndex(0);

	// 시간변화 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Time"));
	assert(pChkBox);
	pChkBox->SetCheckValue(false);

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Time");
	assert(pControl);

	CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon0");
	if(pStatic == NULL) return;
	pControl->SetElement(pStatic->m_pElement->Clone());	

	// 날씨변화 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Weather"));
	assert(pChkBox);
	pChkBox->SetCheckValue(false);

	pControl = CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Weather");
	assert(pControl);

	pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon0");
	if(pStatic == NULL) return;
	pControl->SetElement(pStatic->m_pElement->Clone());	
	
	// 클럽전용 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Club"));
	assert(pChkBox);
	pChkBox->SetCheckValue(false);

	// 관전 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_View"));
	assert(pChkBox);
	pChkBox->SetCheckValue(false);

	// 관전자채팅 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_ViewChat"));
	assert(pChkBox);
	pChkBox->SetCheckValue(false);

	// 버튼들
	UIFunction::Set_UIVisibility( "CR_Btn_Create", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "CR_Btn_Cancel", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "CR_Btn_Ok", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "CR_Btn_ViewOk", N3UI_VISIBLE_FALSE );
}

void CCreateRoomDialog::FUNCTION_CreateRoomUpdate(const char* szwindow)
{
	
}

void CCreateRoomDialog::FUNCTION_CreateRoomTerminate(const char* szwindow)
{

}

void CCreateRoomDialog::FUNCTION_CreateRoomFW(int index, const char* szwindow)
{
	switch( index )
	{
	case 0:		m_nPosition[0] = POSITION_FW;	break;
	case 1:		m_nPosition[0] = POSITION_ST;	break;
	case 2:		m_nPosition[0] = POSITION_CF;	break;
	case 3:		m_nPosition[0] = POSITION_WF;	break;
	case 4:		m_nPosition[0] = POSITION_SS;	break;
	case 5:		m_nPosition[0] = POSITION_ALL;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomMF(int index, const char* szwindow)
{
	switch( index )
	{
	case 0:		m_nPosition[1] = POSITION_MF;	break;
	case 1:		m_nPosition[1] = POSITION_AM;	break;
	case 2:		m_nPosition[1] = POSITION_CM;	break;
	case 3:		m_nPosition[1] = POSITION_SM;	break;
	case 4:		m_nPosition[1] = POSITION_DM;	break;
	case 5:		m_nPosition[1] = POSITION_ALL;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomDF(int index, const char* szwindow)
{
	switch( index )
	{
	case 0:		m_nPosition[2] = POSITION_DF;	break;
	case 1:		m_nPosition[2] = POSITION_SW;	break;
	case 2:		m_nPosition[2] = POSITION_CB;	break;
	case 3:		m_nPosition[2] = POSITION_SP;	break;
	case 4:		m_nPosition[2] = POSITION_ALL;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomALL1(int index, const char* szwindow)
{
	switch( index )
	{
	case 0:		m_nPosition[3] = POSITION_ALL;	break;
	case 1:		m_nPosition[3] = POSITION_FW;	break;
	case 2:		m_nPosition[3] = POSITION_ST;	break;
	case 3:		m_nPosition[3] = POSITION_CF;	break;
	case 4:		m_nPosition[3] = POSITION_WF;	break;
	case 5:		m_nPosition[3] = POSITION_SS;	break;
	case 6:		m_nPosition[3] = POSITION_MF;	break;
	case 7:		m_nPosition[3] = POSITION_AM;	break;
	case 8:		m_nPosition[3] = POSITION_CM;	break;
	case 9:		m_nPosition[3] = POSITION_SM;	break;
	case 10:	m_nPosition[3] = POSITION_DM;	break;
	case 11:	m_nPosition[3] = POSITION_DF;	break;
	case 12:	m_nPosition[3] = POSITION_SW;	break;
	case 13:	m_nPosition[3] = POSITION_CB;	break;
	case 14:	m_nPosition[3] = POSITION_SP;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomALL2(int index, const char* szwindow)
{
	switch( index )
	{
	case -1:	m_nPosition[4] = POSITION_NONE;	break;
	case 0:		m_nPosition[4] = POSITION_ALL;	break;
	case 1:		m_nPosition[4] = POSITION_FW;	break;
	case 2:		m_nPosition[4] = POSITION_ST;	break;
	case 3:		m_nPosition[4] = POSITION_CF;	break;
	case 4:		m_nPosition[4] = POSITION_WF;	break;
	case 5:		m_nPosition[4] = POSITION_SS;	break;
	case 6:		m_nPosition[4] = POSITION_MF;	break;
	case 7:		m_nPosition[4] = POSITION_AM;	break;
	case 8:		m_nPosition[4] = POSITION_CM;	break;
	case 9:		m_nPosition[4] = POSITION_SM;	break;
	case 10:	m_nPosition[4] = POSITION_DM;	break;
	case 11:	m_nPosition[4] = POSITION_DF;	break;
	case 12:	m_nPosition[4] = POSITION_SW;	break;
	case 13:	m_nPosition[4] = POSITION_CB;	break;
	case 14:	m_nPosition[4] = POSITION_SP;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomGK(int index, const char* szwindow)
{
	switch( index )
	{
	case 0:		m_nPosition[5] = POSITION_GK;	break;
	}	
}

void CCreateRoomDialog::FUNCTION_CreateRoomOK()
{
	SetRoomNormalOption();

	//m_pRoom.m_nMode = ROOM_MODE_NORMAL;

	CCSCreateRoom		cPacket;

	_snprintf(cPacket.m_sTitle, TITLE_NAME_SIZE, "%s", m_pRoom.m_sTitle );
	_snprintf(cPacket.m_sPass, 5, "%s", m_pRoom.m_sPass );
	cPacket.m_nState			= m_pRoom.m_nState;
	cPacket.m_nMode				= m_pRoom.m_nMode;
	cPacket.m_nAttackCode		= m_pRoom.m_nAttackCode;
	cPacket.m_nScaleCode		= m_pRoom.m_nScaleCode;
	cPacket.m_nAICode			= m_pRoom.m_nAICode;
	cPacket.m_nPointCode		= m_pRoom.m_nPointCode;
	cPacket.m_nStartLevel		= m_pRoom.m_nStartLevel;
	cPacket.m_nEndLevel			= m_pRoom.m_nEndLevel;

	cPacket.m_nCheckClub		= m_pRoom.m_nCheckClub;
	cPacket.m_nCheckTime		= m_pRoom.m_nCheckTime;
	cPacket.m_nCheckWeather		= m_pRoom.m_nCheckWeather;
	cPacket.m_nCheckViewChat	= m_pRoom.m_nCheckViewChat;
	cPacket.m_nCheckView		= m_pRoom.m_nCheckView;
	cPacket.m_nMaxCount			= m_pRoom.m_nScaleCode * 2;

	for(int i = 0; i < ROOM_SCALE_6; i++)
		cPacket.m_nHomePosition[i]	= cPacket.m_nAwayPosition[i] = m_nPosition[i];

	PutCreateRoom(&cPacket);	
}

void CCreateRoomDialog::SetRoomNormalOption()
{
	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	std::string sText;

	// 인원 설정
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_One"));
	assert(pChkBox);

	if( pChkBox->GetChecked() )
		m_pRoom.m_nMode = ROOM_MODE_SINGLE;
	else
		m_pRoom.m_nMode = ROOM_MODE_NORMAL;

	// 방제 설정
	CUIIMEEditBox* pTitleEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Ime_Title"));
	assert(pTitleEdit);
	sText = pTitleEdit->GetTextEx();

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900028 ,0));
	assert(pMTable);

	if(sText.length() == 0)
		_snprintf( m_pRoom.m_sTitle, TITLE_NAME_SIZE, "%s", pMTable->m_sMessage );
	else
		_snprintf( m_pRoom.m_sTitle, TITLE_NAME_SIZE, "%s", sText.c_str() );

	// 비밀방 설정
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Pass"));
	assert(pChkBox);
	CUIEditBox* pPassEdit = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Pass"));
	assert(pPassEdit);

	if( pChkBox->GetChecked() ) 
	{
		m_pRoom.m_nState	= ROOM_STATE_SECRET;
		// 패스워드 입력
		sText = pPassEdit->GetTextEx();
		_snprintf(m_pRoom.m_sPass, 5, "%s", sText.c_str() );
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_TRUE );
	}
	else 
	{
		m_pRoom.m_nState	= ROOM_STATE_NORMAL;
		m_pRoom.m_sPass[0]	= NULL;
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_FALSE );
	}

	// 인원 설정
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Six"));
	assert(pChkBox);
	if(pChkBox->GetChecked())
		m_pRoom.m_nScaleCode = ROOM_SCALE_6;
	else
		m_pRoom.m_nScaleCode = ROOM_SCALE_5;

	// 인공지능 설정
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Keep"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nAICode = ROOM_AI_KEEPER;

	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_All"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nAICode = ROOM_AI_ALL;
    
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_None"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nAICode = ROOM_AI_NONE;

	// 레벨 범위 설정
	CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("CR_Min_Lv");
	assert(pStatic);
	sText = pStatic->GetTextEx();
	m_pRoom.m_nStartLevel = atoi(sText.c_str());

	pStatic = CClientUIManager::GetPtr()->FindEventControl("CR_Max_Lv");
	assert(pStatic);
	sText = pStatic->GetTextEx();
	m_pRoom.m_nEndLevel = atoi(sText.c_str());

	SetRoomDetailOption();
}

void CCreateRoomDialog::SetRoomDetailOption()
{
	//포지션 설정
	CUIComboBox* pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING1_LISTBOX"));
	assert(pControl);
	int nSelect = pControl->GetSelectedItemIndex();
	FUNCTION_CreateRoomFW(nSelect, "");

	pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING2_LISTBOX"));
	assert(pControl);
	nSelect = pControl->GetSelectedItemIndex();
	FUNCTION_CreateRoomMF(nSelect, "");

	pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING3_LISTBOX"));
	assert(pControl);
	nSelect = pControl->GetSelectedItemIndex();
	FUNCTION_CreateRoomDF(nSelect, "");

	pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING4_LISTBOX"));
	assert(pControl);
	nSelect = pControl->GetSelectedItemIndex();
	FUNCTION_CreateRoomALL1(nSelect, "");

	if( m_pRoom.m_nScaleCode == ROOM_SCALE_6 )
	{
		pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING5_LISTBOX"));
		assert(pControl);
		nSelect = pControl->GetSelectedItemIndex();
		FUNCTION_CreateRoomALL2(nSelect, "");
	}
	else
		m_nPosition[4] = POSITION_NONE;
	
	pControl = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("GRATEROOM_POSITONSETTING6_LISTBOX"));
	assert(pControl);
	nSelect = pControl->GetSelectedItemIndex();
	FUNCTION_CreateRoomGK(nSelect, "");

	// 선공설정
	CUIComboBox* pAttack_ComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Cb_Attack"));
	assert(pAttack_ComboBox);
	
	switch(pAttack_ComboBox->GetSelectedItemIndex())
	{
	case 0:
		m_pRoom.m_nAttackCode	= ROOM_ATTACK_ALTERNATE;
		break;
	case 1:
		m_pRoom.m_nAttackCode	= ROOM_ATTACK_RANDOM;
		break;
	case 2:
		m_pRoom.m_nAttackCode	= ROOM_ATTACK_HANDICAP;
		break;
	case 3:
		m_pRoom.m_nAttackCode	= ROOM_ATTACK_HOME;
		break;
	case 4:
		m_pRoom.m_nAttackCode	= ROOM_ATTACK_AWAY;
		break;
	}

	// 시간변화 허용
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Time"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nCheckTime = true;
	else
		m_pRoom.m_nCheckTime = false;

	// 날씨변화 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Weather"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nCheckWeather = true;
	else
		m_pRoom.m_nCheckWeather = false;

	// 클럽전용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Club"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nCheckClub = true;
	else
		m_pRoom.m_nCheckClub = false;

	// 관전 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_View"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nCheckView = true;
	else
		m_pRoom.m_nCheckView = false;

	// 관전자채팅 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_ViewChat"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		m_pRoom.m_nCheckViewChat = true;
	else
		m_pRoom.m_nCheckViewChat = false;
}

void CCreateRoomDialog::FUNCTION_RR_Is_Detail()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "RoomConfig_View" );
	bool bView = pControl->GetVisible() == N3UI_VISIBLE_TRUE;

	if(bView)
	{
		// 아래 버튼들
		UIFunction::Set_UIVisibility( "CR_Btn_Create", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Cancel", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Ok", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_ViewOk", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "CR_Stc_Disable", N3UI_VISIBLE_TRUE );
	}
	else
	{
		// 아래 버튼들
		UIFunction::Set_UIVisibility( "CR_Btn_Create", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Cancel", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CR_Btn_Ok", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CR_Btn_ViewOk", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "CR_Stc_Disable", N3UI_VISIBLE_FALSE );
	}
}

void CCreateRoomDialog::FUNCTION_CreateRoomLevelRange(short range)
{
	char szbuff[STRLEN_4];
	
	if(range == 0) 
	{
		UIFunction::Set_ControlText( "CR_Min_Lv", "1");
		UIFunction::Set_ControlText( "CR_Max_Lv", "60");
	}
	else 
	{
		if(m_pObjPlayerInfo->m_cLevel.m_nLevel <= range)
			UIFunction::Set_ControlText( "CR_Min_Lv", "1");
		else
		{
			_snprintf(szbuff, STRLEN_4, "%d", m_pObjPlayerInfo->m_cLevel.m_nLevel-range);	
			UIFunction::Set_ControlText( "CR_Min_Lv", szbuff);
		}

		if(m_pObjPlayerInfo->m_cLevel.m_nLevel >= (60-range))
			UIFunction::Set_ControlText( "CR_Max_Lv", "60");
		else
		{
			_snprintf(szbuff, STRLEN_4, "%d", m_pObjPlayerInfo->m_cLevel.m_nLevel+range);	
			UIFunction::Set_ControlText( "CR_Max_Lv", szbuff);
		}
	}
}

void CCreateRoomDialog::FUNCTION_CreateRoomSecret()
{
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Pass"));
	assert(pChkBox);
	CUIEditBox* pEditBox = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Pass"));
	assert(pEditBox);

	if( pChkBox->GetChecked() )
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_TRUE );
	else 
	{
		pEditBox->ClearText();
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_FALSE );
	}
}

void CCreateRoomDialog::FUNCTION_TabChange(char idx)
{
	CUIButton* pBtn0 = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "CRTab0" ));
	assert(pBtn0);
	CUIButton* pBtn1 = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "CRTab1" ));
	assert(pBtn1);

	if( idx )
	{
		UIFunction::Set_UIVisibility( "BasicRoomConfig", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "DetailRoomConfig", N3UI_VISIBLE_TRUE );
	
		pBtn0->SetPressed(false);
		pBtn1->SetPressed(true);
	}
	else
	{
		UIFunction::Set_UIVisibility( "BasicRoomConfig", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "DetailRoomConfig", N3UI_VISIBLE_FALSE );
	
		pBtn0->SetPressed(true);
		pBtn1->SetPressed(false);
	}
}