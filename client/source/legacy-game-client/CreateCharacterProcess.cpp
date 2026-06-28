#include "stdafx.h"
#include "CreateCharacterProcess.h"
#include "ChoiceTrioProcess.h"
#include "InputManager.h"
#include "UIProgress.h"
#include "UIIMEEditBox.h"
#include "UIButton.h"
#include "PlayModeManager.h"
#include "UICommonMethod.h"
#include "PutPacket.h"
#include "UIFunction.h"
#include "ResTableStack.h"

CCreateCharacterProcess::CCreateCharacterProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_CREATECHARACTER;

	m_pCharacterInfo = new CObjPlayerInfo;

	m_nSkin			= 11;
	m_nGender		= 1;
	m_bDuplication	= false;
	m_nSelSkin		= 0;

	m_pCharacterInfo->m_cShape.m_nGender = m_nGender;
}

CCreateCharacterProcess::~CCreateCharacterProcess(void)
{

}

void CCreateCharacterProcess::CreateCharacter()
{
	CObjectModelManager::GetPtr()->DeleteModelObject(m_pCharacterInfo->m_pModel);
	SAFE_DELETE( m_pCharacterInfo );
	m_pCharacterInfo = new CObjPlayerInfo;

	m_pCharacterInfo->m_cShape.m_nGender = m_nGender;
}

void CCreateCharacterProcess::Terminate()
{
	for( int i=0;i<LIST6_SIZE;i++)
	{
		if( (int)m_vItemList[i].size() )
			m_vItemList[i].clear();

		m_nCurrentEquipIndex[i] = 0;
	}

	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();

	m_pCharacterInfo->Terminate();
	SAFE_DELETE( m_pCharacterInfo );
}

void CCreateCharacterProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	InitItemTable();

	CObjMemberInfo*	pMember	= CObjMemberManager::GetPtr()->GetMemberPointer();
	
	FUNCTION_InitCreateCharacter();
	
    FUNCTION_3DRender("Static_Render" , pObjGroundInfo->GetObjectCamera());	

	m_nState = STATE_START;
}

void CCreateCharacterProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	{	
		CGameProject::s_pGameProject->BeginFrame();

		CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
		assert(pObjGroundInfo);
		pObjGroundInfo->m_pModel->BeforeRender();

		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime, true);

		//m_cCharacterTexBoard.Render();

		CGameProject::s_pGameProject->EndFrame();

		CGameProject::s_pGameProject->DisplayFrame();
	}

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayGameVersion();
	UICommonMethod::DisplayMenuToolTip();
}

bool CCreateCharacterProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CCreateCharacterProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();
	}
}

void CCreateCharacterProcess::UpdateState(float fFrameTime, float fAccumTime)
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
			InputNewCharacterInfo();
			LoadCharacter();

			m_nState = STATE_UPDATE;			
		}
		break;
	case STATE_UPDATE:
		{
			m_bIsRender = true;

			m_nState=STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

bool CCreateCharacterProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CCreateCharacterProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

CBaseProcess* CCreateCharacterProcess::GetSameProcess()
{
	return new CCreateCharacterProcess;
}

void CCreateCharacterProcess::InitItemTable()
{
	CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "SkinBtn1" ));
	assert(pBtn);
	
	if( m_nGender == 2 )
		UIFunction::Set_UIVisibility( "SkinBtn1", N3UI_VISIBLE_FALSE );
	else
		UIFunction::Set_UIVisibility( "SkinBtn1", N3UI_VISIBLE_TRUE );

	for( int i=0;i<LIST6_SIZE;i++)
	{
		if( (int)m_vItemList[i].size() )
			m_vItemList[i].clear();

		m_nCurrentEquipIndex[i] = 0;
	}
	
	CResItemTableStack *temp = dynamic_cast<CResItemTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_ITEMTABLE));	
	assert(temp);

	for(int i = 0; i <  temp->GetResourceSize(); i++)
	{
		CResItemTableFormat *pItemTable = dynamic_cast<CResItemTableFormat*>(temp->GetSequenceField(i));
		assert(pItemTable);
		
		if( pItemTable->m_nOrder >= 100 && pItemTable->m_nType >= ITEM_TYPE_FACEPAINT )
			continue;

		CResItemTableFormat pItem;

		memcpy( &pItem, pItemTable, sizeof(CResItemTableFormat));

		int nCode = pItem.GetObjCode();

		if( pItemTable->m_nGender == m_nGender || pItemTable->m_nGender == 0 )
		{
			switch(pItemTable->m_nType)
			{
			case ITEM_TYPE_FACE:
				m_vItemList[0].push_back(pItem);
				break;
			case ITEM_TYPE_HAIR:
				m_vItemList[1].push_back(pItem);
				break;
			case ITEM_TYPE_SHIRTS:
				m_vItemList[2].push_back(pItem);
				break;
			case ITEM_TYPE_PANTS:
				m_vItemList[3].push_back(pItem);
				break;
			case ITEM_TYPE_GLOVE:
				m_vItemList[4].push_back(pItem);
				break;
			case ITEM_TYPE_SHOES:
				m_vItemList[5].push_back(pItem);
				break;
			}			
		}
	}
}

void CCreateCharacterProcess::InputNewCharacterInfo()
{
	ZeroMemory(m_pCharacterInfo->m_sName, PLAYER_NAME_SIZE);
	m_nSkin = m_vItemList[0][m_nCurrentEquipIndex[0]].GetObjCode()%100;
	m_pCharacterInfo->m_cShape.m_nSkin = m_nSkin;
	m_pCharacterInfo->m_nOrder = 0;

	int arrType[6] = {
		ITEM_EQUIP_FACE,
		ITEM_EQUIP_HAIR,
		ITEM_EQUIP_SHIRTS,
		ITEM_EQUIP_PANTS,
		ITEM_EQUIP_GLOVE,
		ITEM_EQUIP_SHOES,
	};

	char sText[STRLEN_32]="";

	for( int i = 0;i < LIST6_SIZE;i++ )
	{
		if( i == 0 || i == 4)
		{
			m_pCharacterInfo->m_nEquipWear[arrType[i]] = m_vItemList[i][m_nCurrentEquipIndex[i]].GetObjCode();
			continue;
		}
		else
		{
			m_pCharacterInfo->m_nEquipWear[arrType[i]] = m_vItemList[i][m_nCurrentEquipIndex[i]].GetObjCode();
			CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_pCharacterInfo->m_nEquipWear[arrType[i]], m_nSkin));
			assert(pTable);
			_snprintf( sText, STRLEN_32, "Static_Style%d", i );
			UIFunction::Set_ControlText( sText , pTable->m_sName );
		}
	}
}

void CCreateCharacterProcess::Change_Style(int index)
{
	if( 0 < index )
	{
		m_nCurrentEquipIndex[index]++;
		if( (int)m_vItemList[index].size() < m_nCurrentEquipIndex[index]+1 )
			m_nCurrentEquipIndex[index] = 0;
	}
	else
	{
		index = index*(-1);
		m_nCurrentEquipIndex[index]--;
		if( m_nCurrentEquipIndex[index] < 0 )
			m_nCurrentEquipIndex[index] = m_vItemList[index].size()-1;
	}

	int nCode = m_vItemList[index][m_nCurrentEquipIndex[index]].GetObjCode();

	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, nCode, m_nSkin));
	assert(pTable);

	m_pCharacterInfo->m_nEquipWear[pTable->m_nWear] = nCode;
	m_pCharacterInfo->m_pEquipWearModelID[pTable->m_nWear] = 
		CObjectModelManager::GetPtr()->ExchangeItem(m_pCharacterInfo->m_nEquipWear[pTable->m_nWear], m_nSkin,
		m_pCharacterInfo->m_pEquipWearModelID[pTable->m_nWear], m_pCharacterInfo->m_pModel);

	char sText[STRLEN_32]="";

	_snprintf( sText, STRLEN_32, "Static_Style%d", index);
	UIFunction::Set_ControlText( sText, pTable->m_sName );
}

void CCreateCharacterProcess::Change_Gender(int index)
{
	m_nGender = index;
	InitItemTable();
	CreateCharacter();
	InputNewCharacterInfo();
	LoadCharacter();
}

void CCreateCharacterProcess::Change_Skin(int index)
{
	if( m_nGender == 2 && index == 2 )
		index = 1;

	m_nCurrentEquipIndex[0] = index;

	CreateCharacter();
	InputNewCharacterInfo();
	LoadCharacter();
}

void CCreateCharacterProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	pObjGroundInfo->SetObjectPosition(NiPoint3(0,0,0));

	NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CCreateCharacterProcess::LoadCharacter()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pCharacterInfo, 0);

	m_pCharacterInfo->SetScale();

	m_pCharacterInfo->m_cCurrentAction.m_fStartAngle = -90.f;
	CProcessManager::GetPtr()->AttachItem(m_pCharacterInfo); 
}

void CCreateCharacterProcess::Create_Character()
{
	if(!m_bDuplication)
	{
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200218, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return;
	}

	CCSCreateCharacter* cNewCharacterInfo = new CCSCreateCharacter;

	_snprintf(cNewCharacterInfo->m_sName, PLAYER_NAME_SIZE, m_pCharacterInfo->m_sName);

	cNewCharacterInfo->m_nEquip[0]=m_pCharacterInfo->m_nEquipWear[0];
	cNewCharacterInfo->m_nEquip[1]=m_pCharacterInfo->m_nEquipWear[1];
	cNewCharacterInfo->m_nEquip[2]=m_pCharacterInfo->m_nEquipWear[3];
	cNewCharacterInfo->m_nEquip[3]=m_pCharacterInfo->m_nEquipWear[4];
	cNewCharacterInfo->m_nEquip[4]=m_pCharacterInfo->m_nEquipWear[6];
	cNewCharacterInfo->m_cShape=m_pCharacterInfo->m_cShape;

	PutCreateCharacter(cNewCharacterInfo);
	delete cNewCharacterInfo;
}

bool CCreateCharacterProcess::InputCharacterName()
{
	char sText[MAX_PATH]="";
	CUIIMEEditBox* pName=dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CharacterName"));
	assert(pName);
	if(!strcmp(m_sCharacterName, pName->GetTextEx().c_str()))
	{
		_snprintf(m_pCharacterInfo->m_sName, PLAYER_NAME_SIZE, m_sCharacterName);
		pName->ClearFocus();
		return true;
	}
	else
	{
		m_bDuplication = false;
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200218, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return false;
	}
}

void CCreateCharacterProcess::FUNCTION_CreateTutoInit()
{
	UIFunction::Set_UIVisibility( "Statistics_CC", N3UI_VISIBLE_TRUE );			// 캐릭터 생성 설명 창
	UIFunction::Set_UIVisibility( "Choice_Taxt04", N3UI_VISIBLE_TRUE );				// 캐릭터 생성 설명 1
	UIFunction::Set_UIVisibility( "Create_Continue", N3UI_VISIBLE_TRUE );			// 계속 버튼
}

void CCreateCharacterProcess::FUNCTION_InitCreateCharacter()
{
	char sText[STRLEN_64]="";

	CUIIMEEditBox* pIme = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CharacterName"));
	assert(pIme);

	pIme->ClearText();
}

void CCreateCharacterProcess::FUNCTION_ShowFacultTuto()
{
	char sText[MAX_PATH]="";

	UIFunction::Set_UIVisibility( "Choice_Taxt05", N3UI_VISIBLE_FALSE );			// 캐릭터 생성 설명 2
	UIFunction::Set_UIVisibility( "Statistics_CC", N3UI_VISIBLE_TRUE );			// 캐릭터 생성 설명 창
	UIFunction::Set_UIVisibility( "Choice_Taxt06", N3UI_VISIBLE_TRUE );				// 캐릭터 생성 설명 3
	UIFunction::Set_UIVisibility( "Create_Ok", N3UI_VISIBLE_TRUE );					// 확인
	CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl("CharacterName");	// 캐릭터 명(EditBox)
	assert(pName);

	CUIControl* pText = CClientUIManager::GetPtr()->FindEventControl("Choice_Taxt06");	// 캐릭터 명(Static)
	assert(pText);

	_snprintf( sText, MAX_PATH, pText->GetTextEx().c_str(), pName->GetTextEx().c_str() );
	pText->SetText(sText);
}

char CCreateCharacterProcess::NameCheck()
{
	char nAllowCode[4] = {0, 1, 2, 6};

	CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl("CharacterName");	// 캐릭터 명
	assert(pName);
	
	if(pName->GetTextEx().length() == 0)
		return 3;									// 캐릭터 이름을 입력해 주세요.

	if(4>pName->GetTextEx().length() || 14<pName->GetTextEx().length())
		return 4;									// 캐릭터 이름은 한글 2~7자, 영문 4~14자만 가능합니다.

	CResUnicodeTableFormat * pUnicodeTable = NULL;
	for(int i=0;i<::lstrlenW(pName->GetText());i++)
	{
		for(int p=0;p<4;p++)
		{
			pUnicodeTable = dynamic_cast<CResUnicodeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_UNICODETABLE, nAllowCode[p], 0));
			assert(pUnicodeTable);

			if(i==0 && p==0)						// 첫 글자이며 숫자일 경우
			{
				if(pUnicodeTable->m_nStart<=pName->GetText()[i] && pUnicodeTable->m_nEnd>=pName->GetText()[i])
					return 1;						// 캐릭터 이름의 첫 글자로 숫자는 올수 없습니다.
			}

			if(pUnicodeTable->m_nStart<=pName->GetText()[i] && pUnicodeTable->m_nEnd>=pName->GetText()[i])
				break;
			else if(pName->GetText()[i] == 91 || pName->GetText()[i] == 93)	// 중괄호[]일 경우 허용
				break;
			else
			{
				if(p==3)
					return 2;						// 캐릭터 이름은 한글, 영문대소문자, 숫자, 중괄호[] 조합만 가능합니다.
			}
		}
	}
	return 0;
}

void CCreateCharacterProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		break;
	}
}

void CCreateCharacterProcess::DuplicationCheck()
{
	CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl("CharacterName");	// 캐릭터 명
	assert(pName);

	switch(NameCheck())
	{
	case 0:						// 사용 가능
		if(!m_bDuplication)
			PutCharacterSearch(pName->GetTextEx().c_str());
		break;
	case 1:						// 캐릭터 이름의 첫 글자로 숫자는 올수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200215, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case 2:						// 캐릭터 이름은 한글, 영문대소문자, 숫자, 중괄호[] 조합만 가능합니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200216, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case 3:						// 캐릭터 이름을 입력해 주세요.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200214, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case 4:						// 캐릭터 이름은 한글 2~7자, 영문 4~14자만 가능합니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200217, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}