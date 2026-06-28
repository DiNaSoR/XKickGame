#include "uistdafx.h"
#include "stdafx.h"
#include "ClientUIManager.h"
#include "UICommonMethod.h"
#include "UIProgress.h"
#include "ObjMemberInfo.h"
#include "ObjMemberManager.h"
#include "ObjPlayerInfo.h"
#include "FblFunction.h"
#include <algorithm>
#include "ResTableStack.h"
#include "UtlLog.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "ObjServerInfo.h"
#include "PacketForm.h"
#include "UIFunction.h"

void UICommonMethod::OnClickTab(const char* sIndexstr, const char* sTabCommonName, const char* sDetailTabCommonName)
{
	const int nTabNum=atoi(sIndexstr)/100;

	const int nClickedTabIndex=atoi(sIndexstr)%10;

	char sControlName[STRLEN_64];

	for(int nIndex=0;nIndex<nTabNum;nIndex++)
	{
		_snprintf(sControlName, STRLEN_64, "%s%d", sTabCommonName, nIndex);
		CUIButton* pButtonTab = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sControlName));
		assert(pButtonTab);	
		pButtonTab->SetPressed(false);

		_snprintf(sControlName, STRLEN_64, "%s%d", sDetailTabCommonName, nIndex);
		CUIControl *pControlDetailTab = CClientUIManager::GetPtr()->FindEventControl(sControlName);
		if(pControlDetailTab == NULL)
			continue;
		pControlDetailTab->SetVisible(N3UI_VISIBLE_FALSE);
	}

	_snprintf(sControlName, STRLEN_64, "%s%d", sTabCommonName, nClickedTabIndex);
	CUIButton* pButtonTab = (CUIButton*)(CClientUIManager::GetPtr()->FindEventControl(sControlName));
	pButtonTab->SetPressed(true);

	_snprintf(sControlName, STRLEN_64, "%s%d", sDetailTabCommonName, nClickedTabIndex);
	CUIControl *pControl = CClientUIManager::GetPtr()->FindEventControl(sControlName);
	if(pControl == NULL)
		return;
	pControl->SetVisible(N3UI_VISIBLE_TRUE);
}

void UICommonMethod::OnClickTab(const char* sIndexstr, const char* sDetailTabCommonName)
{
	const int nTabNum=atoi(sIndexstr)/100;

	const int nClickedTabIndex=atoi(sIndexstr)%10;

	char sControlName[STRLEN_64]="";

	for(int nIndex=0;nIndex<nTabNum;nIndex++)
	{
		_snprintf(sControlName, STRLEN_64, "%s_DetailTab%d", sDetailTabCommonName, nIndex);
		CUIControl *pControlDetailTab = CClientUIManager::GetPtr()->FindEventControl(sControlName);
		if(pControlDetailTab == NULL)
			continue;
		pControlDetailTab->SetVisible(N3UI_VISIBLE_FALSE);
	}

	_snprintf(sControlName, STRLEN_64, "%s_DetailTab%d", sDetailTabCommonName, nClickedTabIndex);
	CUIControl *pControl = CClientUIManager::GetPtr()->FindEventControl(sControlName);
	if(pControl == NULL)
		return;
	pControl->SetVisible(N3UI_VISIBLE_TRUE);
}

void UICommonMethod::OnClickTabInvisible(const char* sIndexstr, const char* sDetailTabCommonName)
{
	const int nTabNum=atoi(sIndexstr)/100;

	const int nClickedTabIndex=atoi(sIndexstr)%10;

	char sControlName[STRLEN_64]="";

	for(int nIndex=0;nIndex<nTabNum;nIndex++)
	{
		_snprintf(sControlName, STRLEN_64, "%s_DetailTab%d", sDetailTabCommonName, nIndex);
		CUIControl *pControlDetailTab = CClientUIManager::GetPtr()->FindEventControl(sControlName);
		if(pControlDetailTab == NULL)
			continue;
		pControlDetailTab->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////////
CUIPopUp::CUIPopUp(const char* sTargetControl)
{
	m_cTargetControl=CClientUIManager::GetPtr()->FindEventControl(sTargetControl);
	m_cTargetControl->SetVisible(N3UI_VISIBLE_TRUE);
}

void CUIPopUp::Add_Control(CUIControl* cUIControl)
{
	if(find(m_vControlList.begin(),m_vControlList.end(),cUIControl)==m_vControlList.end())
		m_vControlList.push_back(cUIControl);
}

void CUIPopUp::Add_Control(const char* sControlName)
{
	CUIControl* cUIControl=CClientUIManager::GetPtr()->FindEventControl(sControlName);
	if(cUIControl == NULL) return;
	DWORD nIndex=0;
	DWORD nIndex_end=cUIControl->Get_ChildSize();

	if(nIndex_end==0)
	{
		Add_Control(cUIControl);
		return;
	}

	while(nIndex<nIndex_end)
	{
		Add_Control(cUIControl->Get_Child(nIndex));
		nIndex++;
	}

	return;
}

bool CUIPopUp::WhetherHoverControls()
{
	if( (m_cTargetControl==NULL) | (m_vControlList.size()==0) )
		return false;
	vector<CUIControl*>::iterator pIndex=m_vControlList.begin();
	vector<CUIControl*>::iterator pIndex_end=m_vControlList.end();

	while(pIndex != pIndex_end)
	{
		if((*pIndex)==CUIControl::Get_MouseOveredControl())
			return true;
		pIndex++;
	}
	m_cTargetControl->SetVisible(N3UI_VISIBLE_FALSE);
	return false;
}

CUIPopUp::CUIPopUp(CUIControl* cTargetControl)
{
	m_cTargetControl=cTargetControl;
}

CUIPopUp::~CUIPopUp()
{
}

void UICommonMethod::DrawPositionIcon( char* sTarget, DWORD nPositionCode, bool bHomeTeam )
{
	char sNewPositionCode[STRLEN_64];
	CUIControl* pAPosition = CClientUIManager::GetPtr()->FindEventControl(sTarget);
	if(pAPosition == NULL) return;
	if(bHomeTeam)
        sprintf(sNewPositionCode, "PUBLIC_HPOSITION_IMAGE%d", nPositionCode);
	else
		sprintf(sNewPositionCode, "PUBLIC_APOSITION_IMAGE%d", nPositionCode);
	pAPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sNewPositionCode))->m_pElement->Clone());
}

void UICommonMethod::DrawPositionIcon2( char* sTarget, DWORD nPositionCode, bool bHomeTeam )
{
	char sNewPositionCode[STRLEN_64];
	CUIControl* pAPosition = CClientUIManager::GetPtr()->FindEventControl(sTarget);
	if(pAPosition == NULL) return;
	if(bHomeTeam)
        sprintf(sNewPositionCode, "BPosition_%d", nPositionCode);
	else
		sprintf(sNewPositionCode, "RPosition_%d", nPositionCode);
	pAPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sNewPositionCode))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw( char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pPosition = CClientUIManager::GetPtr()->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "Position_H_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "Position_A_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw( CUIControl* pTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";

	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "Position_H_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "Position_A_%d", nPositionCode);
	pTarget->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw( char* sParent, char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pParent = CClientUIManager::GetPtr()->FindEventControl( sParent );
	assert(pParent);
	CUIControl* pPosition = pParent->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "Position_H_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "Position_A_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw( CUIControl* pParent, char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pPosition = pParent->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "Position_H_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "Position_A_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw2( char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pPosition = CClientUIManager::GetPtr()->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "BPosition_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "RPosition_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw2( CUIControl* pTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";

	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "BPosition_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "RPosition_%d", nPositionCode);
	pTarget->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw2( char* sParent, char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pParent = CClientUIManager::GetPtr()->FindEventControl( sParent );
	assert(pParent);
	CUIControl* pPosition = pParent->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "BPosition_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "RPosition_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::PositionIconDraw2( CUIControl* pParent, char* sTarget, char nPositionCode, bool bHomeTeam )
{
	char sText[STRLEN_64]="";
	CUIControl* pPosition = pParent->FindEventControl(sTarget);
	assert(pPosition);
	if(bHomeTeam)
        _snprintf(sText, STRLEN_64, "BPosition_%d", nPositionCode);
	else
		_snprintf(sText, STRLEN_64, "RPosition_%d", nPositionCode);
	pPosition->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
}

void UICommonMethod::TurnOffAllChild(char* sTarget)
{
	CUIControl* pTarget = CClientUIManager::GetPtr()->FindEventControl(sTarget);
	if(pTarget == NULL) return;
	DWORD nChildNum=pTarget->m_Controls.GetSize();
	DWORD nIndex=0;
	while(nIndex<nChildNum)
	{
		UIFunction::Set_ChildNumVisibility(pTarget,NULL,nIndex,N3UI_VISIBLE_FALSE);
		nIndex++;
	}
}

void UICommonMethod::DisPlayNotice()
{
	time_t c_time;
	time( &c_time );

	static time_t mem=0;

	CResGameTipTableStack *temp = dynamic_cast<CResGameTipTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_GAMETIPTABLE));
	assert(temp);
	
	char sText[MAX_PATH]="";
	int nCnt = 0;

	if(0==(c_time%8) && mem < c_time)
	{
		nCnt = CGameProject::s_pGameProject->s_Notice.m_nCount;
		if(nCnt !=0)
			_snprintf(sText, MAX_PATH, "%s", CGameProject::s_pGameProject->s_Notice.m_cNoticeList[rand()%nCnt].m_strText );
		mem = c_time;

		UIFunction::Set_ControlText( "Message", sText );
		UIFunction::ChangeTexture( "Notice_Icon", "Icon_Notice" );
	}
	else if(4==(c_time%8) && mem < c_time)
	{
		nCnt = temp->GetResourceSize();

		CResGameTipTableFormat *pGameTipTable = dynamic_cast<CResGameTipTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_GAMETIPTABLE, (rand()%nCnt)+10000, 0));
		_snprintf(sText, MAX_PATH, "%s", pGameTipTable->m_sTipText);
		mem = c_time;

		UIFunction::Set_ControlText( "Message", sText );
		UIFunction::ChangeTexture( "Notice_Icon", "Icon_Tip" );
	}
	
	if(3<(c_time%8))
		UIFunction::Set_Color( "Message", 0xffffae00 );
	else
		UIFunction::Set_Color( "Message", 0xfffc00ff );
}

void UICommonMethod::DisplayGameVersion()
{
	UIFunction::Set_ControlText( "Common_GameVersion", CGameProject::s_pGameProject->m_sClientVersion );
}

void UICommonMethod::DisplayMoney()
{
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	char sCoin[STRLEN_16];
	char sPoint[STRLEN_16];
	char sCredit[STRLEN_16];
	sprintf(sCoin,"%d",pMember->m_cMoney.m_nCash);
	sprintf(sPoint,"%d",pMember->m_cMoney.m_nPoint);
	sprintf(sCredit,"%d",pMember->m_cMoney.m_nCredit);
	UIFunction::Set_ControlText( "Common_Coin", sCoin );
	UIFunction::Set_ControlText( "Common_Point", sPoint );
	UIFunction::Set_ControlText( "Common_Credit", sCredit );
}

void UICommonMethod::DisplayBasicInfo(CObjPlayerInfo* pPlayer)
{
	char sText[MAX_PATH]="";
	char sLevel[STRLEN_16];
	char sName[STRLEN_16];
	char sExp[STRLEN_16];
	//_snprintf( sPosition, STRLEN_16, GetPositionString(pPlayer->m_nPosition, sPosition) );

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	
	sprintf(sLevel, pMTable->m_sMessage, pPlayer->m_cLevel.m_nLevel);
	sprintf(sName,"%s",pPlayer->m_sName);
	sprintf(sExp,"%d",pPlayer->m_cLevel.m_nExp);
	
	UICommonMethod::PositionIconDraw( "Common_Position", pPlayer->m_nPosition, UICommonMethod::HOME_TEAM );

	UIFunction::Set_ControlText( "Common_Level", sLevel );
	UIFunction::Set_ControlText( "Common_ID", sName );
	UIFunction::Set_ControlText( "Common_ExperienceNumber", sExp );

	CUIProgress* pExpProgress=dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Common_Experience"));
	assert(pExpProgress);	
	if(pExpProgress == NULL) return;
	
	CResLevelTableFormat *pLvTable = dynamic_cast<CResLevelTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_LEVELTABLE, pPlayer->m_cLevel.m_nLevel-1, 0));
	assert(pLvTable);
	pExpProgress->m_nMax = pLvTable->m_nNeedEXP;
	pExpProgress->SetValue(pPlayer->m_cLevel.m_nExp - pLvTable->m_nTargetEXP);
}

void UICommonMethod::DisplayAll(CObjPlayerInfo* pPlayer)
{
	UICommonMethod::DisplayGameVersion();
	UICommonMethod::DisplayMoney();
	UICommonMethod::DisplayBasicInfo(pPlayer);
}

bool UICommonMethod::IsRectInPoint(LPRECT rt, LPPOINT pt)
{
	if( (rt->left <= pt->x) && (pt->x <= rt->right) && (rt->top <= pt->y) && (pt->y <= rt->bottom) )
		return true;
	else
		return false;
}

void UICommonMethod::DisplayMenuToolTip()
{
	char sText[STRLEN_64]="";
	CUIControl* pMenu = NULL;
	CUIControl* pToolTip = NULL;

	for( int i=0;i<=LIST8_SIZE;i++)
	{
		sprintf(sText, "MenuTabButton0%d", i);
		pMenu = CClientUIManager::GetPtr()->FindEventControl(sText);
		if(pMenu==NULL)
			continue;
		else
		{
			pToolTip = pMenu->FindEventControl("Button_NameBox");
			assert(pToolTip);
			if(CUIControl::Get_MouseOveredControl() == pMenu)
				pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			else
				pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
		}
	}
}


void UICommonMethod::DisplayMenuToolTip(char idx)
{
	char sText[STRLEN_64]="";
	CUIControl* pMenu = NULL;
	CUIControl* pToolTip = NULL;

	int cnt=0;

	for( int i=0;i<=LIST8_SIZE;i++)
	{
		sprintf(sText, "MenuTabButto0%d", i);
		pMenu = CClientUIManager::GetPtr()->FindEventControl(sText);
		if(pMenu==NULL)
		{
			cnt++;
			continue;
		}
		else
		{
			pToolTip = pMenu->FindEventControl("Button_NameBox");
			assert(pToolTip);
			if(CUIControl::Get_MouseOveredControl() == pMenu)
				pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			else
			{
				pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
				cnt++;
			}
		}
	}

	if(cnt==LIST8_SIZE+1)
	{
		sprintf(sText, "MenuTabButto0%d", idx);
		pMenu = CClientUIManager::GetPtr()->FindEventControl(sText);
		pToolTip = pMenu->FindEventControl("Button_NameBox");
		pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
	}
}

void UICommonMethod::GameChange(int nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Normal"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Race"));
	assert(pChkBox1);
	CUIControl* pLevel = CClientUIManager::GetPtr()->FindEventControl("Set_Level");
	assert(pLevel);
	CUIControl* pPoint = CClientUIManager::GetPtr()->FindEventControl("Set_Point");
	assert(pPoint);

	switch(nCode)
	{
	case 0:
		UIFunction::Set_UIVisibility( "WarningMsg", N3UI_VISIBLE_FALSE );
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		pLevel->SetVisible(N3UI_VISIBLE_TRUE);
		pPoint->SetVisible(N3UI_VISIBLE_FALSE);
		break;
	case 1:
		UIFunction::Set_UIVisibility( "WarningMsg", N3UI_VISIBLE_TRUE );
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		pLevel->SetVisible(N3UI_VISIBLE_FALSE);
		pPoint->SetVisible(N3UI_VISIBLE_TRUE);
	}
}

void UICommonMethod::WeatherChange(int nCode)
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Weather");
	if(pControl == NULL) return;

	switch(nCode)
	{
	case ROOM_WEATHER_AUTO:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon0");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_WEATHER_FINE:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon0");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_WEATHER_CLOUDY:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon1");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_WEATHER_RAIN:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon2");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_WEATHER_SNOW:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("WeatherIcon2");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	}
}

void UICommonMethod::TimeChange(int nCode)
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Time");
	if(pControl == NULL) return;

	switch(nCode)
	{
	case ROOM_TIME_AUTO:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon1");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_TIME_SUNRISE:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon0");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_TIME_DAYLIGHT:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon1");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_TIME_SUNSET:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon2");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	case ROOM_TIME_NIGHT:
		{
			CUIControl* pStatic = CClientUIManager::GetPtr()->FindEventControl("TimeIcon3");
			if(pStatic == NULL) return;
			pControl->SetElement(pStatic->m_pElement->Clone());	
		}
		break;
	}
}

void UICommonMethod::AiChange(int nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Keep"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_All"));
	assert(pChkBox1);
	CUICheckBox* pChkBox2 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_None"));
	assert(pChkBox2);

	switch(nCode)
	{
	case 0:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(false);
		break;
	case 1:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		pChkBox2->SetCheckValue(false);
		break;
	case 2:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(true);
		break;
	}
}

void UICommonMethod::PointChange(int nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_P300"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_P500"));
	assert(pChkBox1);
	CUICheckBox* pChkBox2 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_P1000"));
	assert(pChkBox2);

	switch(nCode)
	{
	case 0:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(false);
		break;
	case 1:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		pChkBox2->SetCheckValue(false);
		break;
	case 2:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(true);
		break;
	}
}

void UICommonMethod::ManCountChange(int nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Five"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Six"));
	assert(pChkBox1);
	CUICheckBox* pChkBox2 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_One"));
	assert(pChkBox2);

	switch(nCode)
	{
	case 0:
		UIFunction::Set_UIVisibility( "Static_Disable6", N3UI_VISIBLE_TRUE );
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(false);
		break;
	case 1:
		UIFunction::Set_UIVisibility( "Static_Disable6", N3UI_VISIBLE_FALSE );
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		pChkBox2->SetCheckValue(false);
		break;
	case 2:
		UIFunction::Set_UIVisibility( "Static_Disable6", N3UI_VISIBLE_TRUE );
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(true);
		break;
	}
}

char UICommonMethod::RankNtoRankChar(char nRank)
{
	switch(nRank)
	{
	case 0:	return 'D';	break;
	case 1:	return 'C';	break;
	case 2:	return 'B';	break;
	case 3:	return 'A';	break;
	case 4:	return 'S';	break;
	}
	return 'X';
}

D3DCOLOR UICommonMethod::RankNtoRankColor(char nRank)
{
	switch(nRank)
	{
	case 0:	return 0xffffffff;		break;	// D RANK
	case 1:	return 0xffbdfb01;		break;	// C RANK
	case 2:	return 0xff1583ff;		break;	// B RANK
	case 3:	return 0xffea1bf2;		break;	// A RANK
	case 4:	return 0xfffba001;		break;	// S RANK
	}
	return COLOR_GRAY;
}

std::string UICommonMethod::PositionNtoPositionStr(char nPosition)
{
	switch(nPosition)
	{
	case 10:	return "FW";	break;	// FW
	case 11:	return "ST";	break;	// ST
	case 12:	return "CF";	break;	// CF
	case 13:	return "WF";	break;	// WFk
	case 14:	return "SS";	break;	// SS
	case 20:	return "MF";	break;	// MF
	case 21:	return "AM";	break;	// AM
	case 22:	return "CM";	break;	// CM
	case 23:	return "SM";	break;	// SM
	case 24:	return "DM";	break;	// DM
	case 30:	return "DF";	break;	// DF
	case 31:	return "SW";	break;	// SW
	case 32:	return "CB";	break;	// CB
	case 33:	return "SP";	break;	// SP
	case 40:	return "GK";	break;	// GK
	case 50:	return "NB";	break;	// NB
	}
	return "NONE";
}

void UICommonMethod::BGEffectAnimationInit(char* sWindow1)
{
	UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_FALSE );
	
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);
	pAni->SetVisible(N3UI_VISIBLE_TRUE);
	pAni->SetInitUpdate(false);
	pAni->SetPlay(true);
}

void UICommonMethod::BGEffectAnimationInit(char* sWindow1, char* sWindow2)
{
	UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( sWindow2, N3UI_VISIBLE_FALSE );

	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);
	pAni->SetVisible(N3UI_VISIBLE_TRUE);
	pAni->SetInitUpdate(false);
	pAni->SetPlay(true);
}

void UICommonMethod::BGEffectAnimationInit(char* sWindow1, char* sWindow2, char* sWindow3)
{
	UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( sWindow2, N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( sWindow3, N3UI_VISIBLE_FALSE );

	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);
	pAni->SetVisible(N3UI_VISIBLE_TRUE);
	pAni->SetInitUpdate(false);
	pAni->SetPlay(true);
}

void UICommonMethod::BGEffectAnimationPlay(char* sWindow1)
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);

	if( pAni->GetFrame() == 5 && pAni->GetVisible() == N3UI_VISIBLE_TRUE )
		UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_TRUE ); 	
}

void UICommonMethod::BGEffectAnimationPlay(char* sWindow1, char* sWindow2)
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);

	if( pAni->GetFrame() == 5 && pAni->GetVisible() == N3UI_VISIBLE_TRUE )
	{
		UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_TRUE ); 	
		UIFunction::Set_UIVisibility( sWindow2, N3UI_VISIBLE_TRUE ); 	
	}
}

void UICommonMethod::BGEffectAnimationPlay(char* sWindow1, char* sWindow2, char* sWindow3)
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Ani_Bg");
	assert(pAni);

	if( pAni->GetFrame() == 5 && pAni->GetVisible() == N3UI_VISIBLE_TRUE )
	{
		UIFunction::Set_UIVisibility( sWindow1, N3UI_VISIBLE_TRUE ); 	
		UIFunction::Set_UIVisibility( sWindow2, N3UI_VISIBLE_TRUE ); 	
		UIFunction::Set_UIVisibility( sWindow3, N3UI_VISIBLE_TRUE ); 	
	}	
}

void UICommonMethod::BasicInfoStop()
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Character_Information");
	assert(pAni);//보이기

	if( pAni->GetFrame() == 5 || pAni->GetFrame() == 10 )
		pAni->SetPlay(false, false, false, pAni->GetFrame());

	pAni->SetVisible(N3UI_VISIBLE_TRUE);
}

char* UICommonMethod::SituationCodeToStr( int nSituationCode )
{
	CResSituationTableFormat* pTable = dynamic_cast<CResSituationTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_SITUATIONTABLE, nSituationCode ,0));
	assert(pTable);

	return pTable->m_sTitle;
	//
	//switch(nSituationCode)
	//{
	//case SITUATION_KIND_ALWAYS:					return		"항상";
	//case SITUATION_KIND_WINNING:				return		"이기고 있을 때";
	//case SITUATION_KIND_LOSING:					return		"지고 있을 때";
	//case SITUATION_KIND_OVERTIME:				return		"연장전에서";
	//case SITUATION_KIND_2LOSE:					return		"2점+ 패배상황에";
	//case SITUATION_KIND_2WIN:					return		"2점+ 승리상황에";
	//case SITUATION_KIND_DRAWING:				return		"무승부 상황에서";
	//case SITUATION_KIND_CROSS:					return		"크로스 지역에";	
	//case SITUATION_KIND_MID:					return		"중거리 지역에서";
	//case SITUATION_KIND_SHORT:					return		"단거리 지역에서";
	//case SITUATION_KIND_CENTER:					return		"중앙 지역에서";
	//case SITUATION_KIND_NOMARK:					return		"노마크시";
	//case SITUATION_KIND_HOMEAREA:				return		"같은팀 진영에서";
	//case SITUATION_KIND_AWAYAREA:				return		"상대팀 진영에서";
	//case SITUATION_KIND_MYPOSITION:				return		"자기 포지션에서";
	//case SITUATION_KIND_START1MINUTE:			return		"시작후 1분 동안";
	//case SITUATION_KIND_END1MINUTE:				return		"종료전 1분 동안";
	//case SITUATION_KIND_COUNTERATTACK:			return		"역습시";
	//case SITUATION_KIND_RCOUNTERATTACK:			return		"역습 당할 때";
	//case SITUATION_KIND_2OVERMARK:				return		"2명 이상 마크시";
	//case SITUATION_KIND_POINT:					return		"득점할 때마다";
	//case SITUATION_KIND_RPOINT:					return		"실점할 때마다";
	//case SITUATION_KIND_SHOOTING:				return		"슈팅할 때마다";
	//case SITUATION_KIND_TACKLE:					return		"태클할때 마다";
	//case SITUATION_KIND_STEAL:					return		"스틸할 때마다";
	//case SITUATION_KIND_CUTTING:				return		"커팅할 때마다";
	//case SITUATION_KIND_ASSIST:					return		"어시스트할 때마다";
	//case SITUATION_KIND_WIN:					return		"승리시";
	//case SITUATION_KIND_LOSE:					return		"패배시";
	//case SITUATION_KIND_MVP:					return		"MVP 선정시";
	//case SITUATION_KIND_DRAW:					return		"무승부시";
	//case SITUATION_KIND_SOMETIME:				return		"가끔";
	//default:									return		"";
	//}
}						

char* UICommonMethod::OptionCodeToStr( int nFacultyCode )
{
	CResOptionTableFormat* pTable = dynamic_cast<CResOptionTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_OPTIONTABLE, nFacultyCode ,0));
	assert(pTable);

	return pTable->m_sTitle;

	/*switch(nFacultyCode)
	{
	case FACULTY_KIND_RUN:						return		"달리기";
	case FACULTY_KIND_DRIBBLE:					return		"드리블";
	case FACULTY_KIND_AGILITY:					return		"순발력";
	case FACULTY_KIND_STAMINA:					return		"지구력";
	case FACULTY_KIND_JUMP:						return		"탄력";
	case FACULTY_KIND_BODYCHECK:				return		"몸싸움";
	case FACULTY_KIND_CROSS:					return		"크로스";
	case FACULTY_KIND_SHORTPASS:				return		"숏패스";
	case FACULTY_KIND_LONGPASS:					return		"롱패스";
	case FACULTY_KIND_HEADING:					return		"헤딩슛";
	case FACULTY_KIND_SHORTSHOT:				return		"단거리슛";
	case FACULTY_KIND_LONGSHOT:					return		"중거리슛";
	case FACULTY_KIND_KEEPING:					return		"키핑";
	case FACULTY_KIND_STEAL:					return		"스틸";
	case FACULTY_KIND_TACKLE:					return		"태클";
	case FACULTY_KIND_CATCH:					return		"캐치";
	case FACULTY_KIND_PUNCH:					return		"펀치";
	case FACULTY_KIND_GUARD:					return		"가드";
	case FACULTY_KIND_POWER:					return		"공격력";
	case FACULTY_KIND_DEFENCE:					return		"수비력";
	case FACULTY_KIND_AIRSUPREMACY:				return		"제공권";
	case FACULTY_KIND_PASSING:					return		"패스능력";
	case FACULTY_KIND_MOVING:					return		"이동능력";
	case FACULTY_KIND_PHYSICAL:					return		"체력";
	case FACULTY_KIND_THROUGH:					return		"돌파력";
	case FACULTY_KIND_ALLABILITY:				return		"모든 능력치";
	case FACULTY_KIND_ALLCRITICAL:				return		"모든 치명타";
	case FACULTY_KIND_SKILLCTIMEDEC:			return		"스킬 쿨타임 감소";
	case FACULTY_KIND_SHORTSHOTCRI:				return		"치명적 단거리율";
	case FACULTY_KIND_LONGSHOTCRI:				return		"치명적 중거리율";
	case FACULTY_KIND_TACKLECRI:				return		"치명적 태클율";
	case FACULTY_KIND_STEALCRI:					return		"치명적 스틸율";
	case FACULTY_KIND_HEADINGCRI:				return		"치명적 헤딩율";
	case FACULTY_KIND_BODYCHECKCRI:				return		"치명적 몸싸움율";
	case FACULTY_KIND_FORTUNE:					return		"행운";
	case FACULTY_KIND_EXPUP:					return		"경험치";
	case FACULTY_KIND_POINTUP:					return		"포인트";
	default:									return		"";
	}*/
}

void UICommonMethod::FUNCTION_CardInfoDraw(CUIControl* pParent, int nPosition, int nRank, int nLv, int nCostume)
{
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "AICard_Position_%d", nPosition);
	UIFunction::ChangeChildTexture( pParent, "AICard_Position", sText );

	_snprintf(sText, STRLEN_64, "AICard_Rank%c", UICommonMethod::RankNtoRankChar(nRank));
	UIFunction::ChangeChildTexture( pParent, "AICard_Rank", sText );

	_snprintf(sText, STRLEN_64, "AICardNum_%d", nLv/LIST10_SIZE);
	UIFunction::ChangeChildTexture( pParent, "AICardNum_00", sText );

	_snprintf(sText, STRLEN_64, "AICardNum_%d", nLv%LIST10_SIZE);
	UIFunction::ChangeChildTexture( pParent, "AICardNum_0", sText );

	_snprintf(sText, STRLEN_64, "CardBg%d", nRank);
	UIFunction::ChangeChildTexture( pParent, "CardBg", sText );

	if( nCostume < 39 )
		_snprintf(sText, STRLEN_64, "Man%02d", nCostume%23);
	else
		_snprintf(sText, STRLEN_64, "Woman%02d", (nCostume-38)%23);

	UIFunction::ChangeChildTexture( pParent, "Character", sText );
}