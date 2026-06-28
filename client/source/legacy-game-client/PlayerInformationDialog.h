//***********************************************************************************
//	File		:	PlayerInformationDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef __PLAYERINFORMATION_DIALOG_H__
#define __PLAYERINFORMATION_DIALOG_H__

#include "Singleton.h"
#include "ResCharacterFormat.h"
#include "Control.h"
#include "3DRenderedTexBoard.h"


class CPlayerInformationDialog : public CSingleton<CPlayerInformationDialog>
{
private:
    typedef std::vector< CSaleList > SaleInfoArray;
    SaleInfoArray   m_cSaleInfoArray;
	CFaculty        m_cFaculty;
	DWORD           nUsedFaculty;

    int             m_nCurrentTradePeriod;
    int             m_nCurrentTradePage;
    int             m_nCurrentTradeTotalPage;

    bool            m_bInitCtlPos;
    RECT            m_cAbilityWindowPos;
    RECT            m_cTrophyWindowPos;

public:

	// 분류 관련
	int								m_nBigFractionIndex;//대분류 번호
	int								m_nSmallFractionIndex;//소분류 번호
	int								m_nSmallFractionStart;

	int								m_nReceivePage;
	int								m_nMonthRankingPage;
	int								m_nTotalRankingPage;
	int								m_nAthleteTotalPage;
	int								m_nAthletePage;

	LPDIRECT3DDEVICE9				m_pd3dDevice;

	CObjPlayerInfo*					m_pSelectPlayerInfo;

    NiCameraPtr                     m_spCamera;
    NiNodePtr                       m_spRootNode;
    CResCharacterFormat*			m_pCharacter;

    C3DRenderedTexBoard             m_cCharacterTexBoard;
    
    bool                            m_bIsMeInfo;

	typedef vector<CObjItemInfo*>	CONSUMPTIONITEM_VECTOR;
	CONSUMPTIONITEM_VECTOR			m_vConsumptionItem;

	// 분류 벡트리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	FRACTION_VECTORLIST				m_vFractionList[4];
	
	int								m_nConsumPage;

	POINT							m_pPoint[LIST8_SIZE];


private:
	void Set_FACULTY();
	void Set_Remain_Faculty(DWORD nBonus);
    void InitCharacter();
    void ShowTrade( int nPeriod, int nIndex, CSaleList & cSaleInfo );
    std::string GetObjectName( int nObjectKind, int nCode);

public:
	CPlayerInformationDialog(void);
	~CPlayerInformationDialog(void);

    void                            Update( float fFrameTime, float fAccumTime );

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							FUNCTION_SmallFractionInit(char nBigFraction);
	void							FUNCTION_SmallFraction(char nSmallFraction);
	void							FUNCTION_BigFraction(char nBigFraction);
        
	void							FUNCTION_PlayerInformationInit();
	void							FUNCTION_PlayerInformationUpdate();
	void							FUNCTION_PlayerInformationTerminate();
	void							FUNCTION_PlayerBasicInformation();
	void							FUNCTION_PlayerAbilityInformation();
	void							FUNCTION_PlayerConsumptionInformation();
	void							FUNCTION_PlayerReceiveInformation();
    void                            FUNCTION_TradePageView( int nPage );
    void                            AddSaleList( int nPeriod, CSaleList & cSaleInfo );
    void                            SetTradePage( int nPeriod, int nTotalTradePage, int nCurrentPage );
	void							FUNCTION_PlayerMonthRankingInformation();
	void							FUNCTION_PlayerTotalRankingInformation();
	void							FUNCTION_AtheletListInformation(char nKind, char nPage);
	void							FUNCTION_ClubMemberList(char nPage);
    void                            FUNCTION_ChangeMent();
	void                            FUNCTION_Close();
	void							FUNCTION_InitLinePoint();
	void							DrawGraph();

public:
	void OnClick_AbillityUp(DWORD nIndex);
	void OnClick_AbillityDown(DWORD nIndex);
	void InfoDConfirm_Ability();

    void SetPlayer( CObjPlayerInfo * pPlayerInfo );
    void HideList( CUIControl * pPeriodTab );
};

#endif //__PLAYERINFORMATION_DIALOG_H__