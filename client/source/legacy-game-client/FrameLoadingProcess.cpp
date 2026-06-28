#include "StdAfx.h"
#include "FrameLoadingProcess.h"
#include "ResTableFormat.h"
#include "ChoiceTrioProcess.h"
#include "GameLoginProcess.h"
#include "PacketManager.h"
#include "PutPacket.h"
#include "PlayModeManager.h"
#include "UICommonMethod.h"

#include "LoadPackFile.h"
#include "Utility.h"

LoadingObserver::LoadingObserver(CFrameLoadingProcess * pFrameLoadingProcess )
{
    assert( m_pFrameLoadingProcess != 0 );
    m_pFrameLoadingProcess = pFrameLoadingProcess;
}

LoadingObserver::~LoadingObserver()
{
}

void LoadingObserver::OnUpdate( int nValue )
{
	//m_pFrameLoadingProcess->m_pProgress->SetSize( nValue, m_pFrameLoadingProcess->m_pProgress->GetHeight(), true );

	m_pFrameLoadingProcess->m_pProgress->SetValue( nValue );

    bool bBeginFrame = false;
    NiRenderer * pkRenderer = NiRenderer::GetRenderer();
    if( !pkRenderer->GetInsideFrameState() ) 
    {
        pkRenderer->BeginFrame();
        bBeginFrame = true;
    }

    IProcessObserver::OnUpdate( nValue );

    m_pFrameLoadingProcess->RenderFrame();

    if(bBeginFrame == true ) 
    {
        pkRenderer->EndFrame();
        pkRenderer->DisplayFrame();
    }
}


CFrameLoadingProcess::CFrameLoadingProcess() : CBaseProcess()
{
	m_nProcessCmd       = PROCESS_FRAMELOADING;
	m_bIsRender         = true;
    m_pLoadingObserver  = new LoadingObserver( this );
    CResourceManager::GetPtr()->SetProcessObserver( m_pLoadingObserver );
}

CFrameLoadingProcess::~CFrameLoadingProcess(void)
{
    CResourceManager::GetPtr()->SetProcessObserver( 0 );
    SAFE_DELETE( m_pLoadingObserver );
}

void CFrameLoadingProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CFrameLoadingProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	//UI Language Table 로딩
	UiStaticLanguageLoading();

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Loading_ProgressBar"));
	assert(m_pProgress);

	UICommonMethod::DisplayGameVersion();

	m_nState = STATE_START;
}

void CFrameLoadingProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
}

void CFrameLoadingProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{				
	case STATE_START:
		{
			m_nState = STATE_TABLE_OGG_LOAD;
			m_pLoadingObserver->OnUpdate(1); //m_pProgress->SetValue(10);
		}
		break;
	case STATE_TABLE_OGG_LOAD:
		{
			m_nState = STATE_TABLE_ANIMATION_LOAD;
			m_pLoadingObserver->OnUpdate(3); //m_pProgress->SetValue(15);
		}
		break;
	case STATE_TABLE_ANIMATION_LOAD:
		{
			m_nState = STATE_TABLE_WAV_LOAD;

			CResourceManager::GetPtr()->LoadResource(RESOURCE_FILETABLE);				//파일리스트 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_WAVTABLE);				//효과음 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_OGGTABLE);				//배경음 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_MSGTABLE);				//메세지 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_ITEMTABLE);				//아이템 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_OPTIONTABLE);				//옵션 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_SKILLTABLE);				//스킬 테이블파일

			m_pLoadingObserver->OnUpdate(10); //m_pProgress->SetValue(18);  
			CResourceManager::GetPtr()->LoadResource(RESOURCE_TRAININGTABLE);			//훈련 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_CEREMONYTABLE);			//세레모니 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_MAPTABLE);				//맵 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_LOCATIONTABLE);			//위치 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_PATTERNTABLE);			//패턴 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_ANIMATIONTABLE);			//에니메이션 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_INPUTTABLE);				//인풋 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_BALLTABLE);				//볼 테이블
			CResourceManager::GetPtr()->LoadResource(RESOURCE_OBJECTTABLE);				//오브젝트 테이블
			CResourceManager::GetPtr()->LoadResource(RESOURCE_SKELETONTABLE);			//뼈대 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_CAMERATABLE);				//카메라 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_LEVELTABLE);				//레벨 테이블파일

			m_pLoadingObserver->OnUpdate(30); //m_pProgress->SetValue(18);  
			CResourceManager::GetPtr()->LoadResource(RESOURCE_BALANCETABLE);			//벨런스 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_PANNELTABLE);				//판넬 테이블파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_MORPHTABLE);				//모프 시퀀스테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_TEMPABILITYTABLE);		//
			CResourceManager::GetPtr()->LoadResource(RESOURCE_TEMPCOSTUMETABLE);		//
			CResourceManager::GetPtr()->LoadResource(RESOURCE_TEMPSKILLTABLE);			//
			CResourceManager::GetPtr()->LoadResource(RESOURCE_TUTORIALTABLE);			//튜토리얼 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_QUESTTABLE);				//퀘스트 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_BRANDTABLE);				//브랜드 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_GROUNDTABLE);				//그라운드 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_POSITIONTABLE);			//포지션 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_QUESTCOMPLETETABLE);		//퀘스트완료조건 테이블 파일

			m_pLoadingObserver->OnUpdate(40); //m_pProgress->SetValue(18);  
			CResourceManager::GetPtr()->LoadResource(RESOURCE_UNICODETABLE);			//유니코드 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_SOUNDTABLE);				//사운드 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_HEADWORDTABLE);			//이펙트 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_PARTICLEEFFECTSTABLE);	//파티클 이펙트 테이블 파일
            CResourceManager::GetPtr()->LoadResource(RESOURCE_MAPPARTICLEEFFECTTABLE);
			CResourceManager::GetPtr()->LoadResource(RESOURCE_GAMETIPTABLE);			//게임팁 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_WAYPOINTTABLE);			//게임팁 테이블 파일
            CResourceManager::GetPtr()->LoadResource(RESOURCE_GOALPOSTNETTABLE);		//그물 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_FRACTIONTABLE);			//아이템 분류 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_SITUATIONTABLE);			//상황 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_OPTIONTABLE);			//옵션 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_ENCHANTTABLE);			//인첸트 테이블 파일
			CResourceManager::GetPtr()->LoadResource(RESOURCE_AICARDTABLE);				//AI CARD 테이블 파일

			m_pLoadingObserver->OnUpdate(50); //m_pProgress->SetValue(18);            
		}
		break;
	case STATE_TABLE_WAV_LOAD:
		{
			m_nState = STATE_TABLE_GROUND_LOAD;
			m_pLoadingObserver->OnUpdate(51); //m_pProgress->SetValue(20);
		}
		break;
	case STATE_TABLE_GROUND_LOAD:
		{
			m_nState = STATE_TABLE_CHARACTER_LOAD;
			m_pLoadingObserver->OnUpdate(52); //m_pProgress->SetValue(25);
		}
		break;
	case STATE_TABLE_CHARACTER_LOAD:
		{
			m_nState = STATE_GROUND_LOAD;

			if(CGameProject::s_pGameProject->GetWideScreen())
			{
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_ReadyRoom.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_MessageBox.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_Option.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_PlayerInformation.ui", false);
				m_pLoadingObserver->OnUpdate(100); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_CreateRoom.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_PasswordBox.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_GameResult_open.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_Choice.ui", false);
				m_pLoadingObserver->OnUpdate(150); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_GamePlay.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_Effect.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_ReplayControl.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_ItemDialog.ui", false);
				m_pLoadingObserver->OnUpdate(200); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_ItemShop.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_SkillDialog.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_W_SkillShop.ui", false);
				m_pLoadingObserver->OnUpdate(250); //m_pProgress->SetValue(25);
			}
			else if(!CGameProject::s_pGameProject->GetWideScreen())
			{
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_ReadyRoom_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_MessageBox_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_Option_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_PlayerInformation_New.ui", false);
				m_pLoadingObserver->OnUpdate(100); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_CreateRoom_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_PasswordBox.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_Choice_New.ui", false);
				m_pLoadingObserver->OnUpdate(150); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_GamePlay_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_Effect.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_ReplayControl_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_ItemDialog.ui", false);
				m_pLoadingObserver->OnUpdate(200); //m_pProgress->SetValue(25);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_ItemShop_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_SkillDialog_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_SkillShop_New.ui", false);
				
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_ItemBag_New.ui", false);
				CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_N_SkillBag_New.ui", false);
			}
			CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_CommonObject_New.ui", false);
			CClientUIManager::GetPtr()->LoadFile(".\\UI\\Process_TextEffect_Source.ui", false);
			m_pLoadingObserver->OnUpdate(300); //m_pProgress->SetValue(30);
		}
		break;
	case STATE_GROUND_LOAD:
		{
			m_nState = STATE_WEB_LOGIN;
			LoadCharacter();
			m_pLoadingObserver->OnUpdate(900);//m_pProgress->SetValue(80);
		}
		break;

	case STATE_WEB_LOGIN:
		{
			if(strlen(CGameProject::s_pGameProject->m_strName) && strlen(CGameProject::s_pGameProject->m_strPass))
			{
				m_nState = STATE_CERTIFY_LOGIN_WAIT;
				PutCertifyLogin(CGameProject::s_pGameProject->m_strName, CGameProject::s_pGameProject->m_strPass);
			}
			else
			{
				m_nState = STATE_END1;
			}
			m_pLoadingObserver->OnUpdate(1000); //m_pProgress->SetValue(90);
		}
		break;
	case STATE_CERTIFY_LOGIN_WAIT:
		{
			if(CPacketManager::GetPtr()->m_bIsCertifyLoginDone)//서버에서 패킷을 다받았으면
			{
				m_nState = STATE_MEMBER_INFO_WAIT;
				CPacketManager::GetPtr()->m_bIsCertifyLoginDone = false;
				PutMemberInfo();
			}
			m_pLoadingObserver->OnUpdate(1000);//m_pProgress->SetValue(100);
		}
		break;
	case STATE_MEMBER_INFO_WAIT:
		{
			if(CPacketManager::GetPtr()->m_bIsMemberInfoDone)
			{
				m_nState = STATE_END2;
				CPacketManager::GetPtr()->m_bIsMemberInfoDone = false;
			}
		}
		break;
	case STATE_END1:
		{
			CProcessManager::GetPtr()->SetChangeProcess(new CGameLoginProcess);
		}
		break;
	case STATE_END2:
		{
			CProcessManager::GetPtr()->SetChangeProcess(new CChoiceTrioProcess);
		}
		break;
	}
}

void CFrameLoadingProcess::LoadCharacter()
{
	CResBaseFormat* temp = CResourceManager::GetPtr()->FindResource(RESOURCE_CHARATERMODEL, 1, false);//남자(에니로딩을 위해)
	SAFE_DELETE(temp);
	CResBaseFormat* temp2 = CResourceManager::GetPtr()->FindResource(RESOURCE_CHARATERMODEL, 2, false);//여자(에니로딩을 위해)
	SAFE_DELETE(temp2);
}

void CFrameLoadingProcess::UiStaticLanguageLoading()
{
	const char seperator='	';
	const char quote='"';

	// 언어 종류 식별
	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(CGameProject::s_pGameProject->m_cTiXmlDocument.FirstChild( "Language" ));
	assert(pElement);

	std::string sLangKind = pElement->Attribute("Kind");

	CResourceManager::GetPtr()->m_sLang = sLangKind;

	if(!strcmp(sLangKind.c_str(), "Kor"))		CClientUIManager::GetPtr()->m_nLangage = LANGUAGE_KOR;
	else if(!strcmp(sLangKind.c_str(), "Eng"))	CClientUIManager::GetPtr()->m_nLangage = LANGUAGE_ENG;
	else if(!strcmp(sLangKind.c_str(), "Arg"))	CClientUIManager::GetPtr()->m_nLangage = LANGUAGE_ARG;

	int nLine = 0;

#ifdef _PACKING

	WCHAR buf[1024+1] = {0,};

	LPVOID pp = CLoadPackFile::GetPtr()->PackFileLoadVoid( "Table.pak", "Table_Language.txt" );
	int nSize = CLoadPackFile::GetPtr()->GetCurrentFileSize();
	WCHAR* tmp  = new WCHAR[nSize];
	memset(tmp, 0, nSize);

	_snwprintf( tmp, nSize, L"%s", (WCHAR*)pp );
    int pos = 0;

	nSize = wcslen(tmp);	
	
	bool bFirst = true;

    while( pos < nSize )
	{
		pos = StrGetLineW(buf, pos, tmp);

        WCHAR* p = buf;
		if(bFirst)
		{
			p++;
			bFirst=false;
		}

		WCHAR sText[MAX_PATH] = L"";
		int strCnt = 0;
		int nColNum = 0;
		int nCode = 0;
		
		while(1)
		{
			if( p[0] == 13 && p[1] == 10 )
			{
				sText[strCnt] = '\0';

				char* szRet = NULL;
				int nLen = WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, 0, NULL, NULL);
				szRet = new char[nLen];
				ZeroMemory(szRet, nLen);
				WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, nLen, NULL, NULL);
				std::string strRet(szRet);

				if( nLine == 0 )
					CResourceManager::GetPtr()->m_vColumn.push_back(strRet);
				else
				{
					if(!CResourceManager::GetPtr()->m_vColumn.at(nColNum).compare(sLangKind))
						CResourceManager::GetPtr()->m_pStaticMap[ nCode ] = strRet;
				}					
				
				delete [] szRet;
				break;
			}

			if( p[0] == seperator )
			{
				sText[strCnt] = '\0';

				char* szRet = NULL;
				int nLen = WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, 0, NULL, NULL);
				szRet = new char[nLen];
				ZeroMemory(szRet, nLen);
				WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, nLen, NULL, NULL);
				std::string strRet(szRet);

				if( nLine == 0 )
					CResourceManager::GetPtr()->m_vColumn.push_back(strRet);
				else
				{
					if(nColNum==0)
						nCode = atoi(strRet.c_str());
					if(!CResourceManager::GetPtr()->m_vColumn.at(nColNum).compare(sLangKind))
					{
						CResourceManager::GetPtr()->m_pStaticMap[ nCode ] = strRet;					
						delete [] szRet;
						break;
					}
				}				
				
				delete [] szRet;

				sText[0]='\0';
				strCnt=0;

				nColNum++;
			}
			else
               sText[strCnt++] = p[0];

			p++;
		}
		nLine++;
	}
	delete[] tmp;

#else

	WCHAR buf[1024+1] = {0,};

	FILE* fp = fopen(".\\Table\\Table_Language.txt", "rb");
	if (fp == NULL) return;
	
	while (fgetws(buf,1024, fp) != NULL )
	{
        WCHAR* p = buf;
		WCHAR sText[MAX_PATH] = L"";
		int strCnt = 0;
		int nColNum = 0;
		int nCode = 0;
		
		while(1)
		{
			if( p[0] == 13 && p[1] == 10 )
			{
				sText[strCnt] = '\0';

				char* szRet = NULL;
				int nLen = WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, 0, NULL, NULL);
				szRet = new char[nLen];
				ZeroMemory(szRet, nLen);
				WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, nLen, NULL, NULL);
				std::string strRet(szRet);

				if( nLine == 0 )
					CResourceManager::GetPtr()->m_vColumn.push_back(strRet);
				else
				{
					if(!CResourceManager::GetPtr()->m_vColumn.at(nColNum).compare(sLangKind))
						CResourceManager::GetPtr()->m_pStaticMap[ nCode ] = strRet;
				}
				delete [] szRet;
				break;
			}

			if( p[0] == seperator )
			{
				sText[strCnt] = '\0';

				char* szRet = NULL;
				int nLen = WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, 0, NULL, NULL);
				szRet = new char[nLen];
				ZeroMemory(szRet, nLen);
				WideCharToMultiByte(CP_ACP, 0, sText, -1, szRet, nLen, NULL, NULL);
				std::string strRet(szRet);

				if( nLine == 0 )
					CResourceManager::GetPtr()->m_vColumn.push_back(strRet);
				else
				{
					if(nColNum==0)
						nCode = atoi(strRet.c_str());

					if(!CResourceManager::GetPtr()->m_vColumn.at(nColNum).compare(sLangKind))
					{
						CResourceManager::GetPtr()->m_pStaticMap[ nCode ] = strRet;
						delete [] szRet;
						break;
					}
				}

				delete [] szRet;

				sText[0]='\0';
				strCnt=0;

				nColNum++;
			}
			else
               sText[strCnt++] = p[0];

			p++;
		}
		nLine++;
	}
#endif

}