/*********************************************************************************************



*			ChoiceRoomProcess.h

*			방 선택 프로세스

*			설명 : 게임을 하기위한 방선택 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "3DRenderedTexBoard.h"

class CItemShopProcess : public CBaseProcess
{
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_END
	};

	enum STATE_INTERESTPAGE
	{
		NOT_INTEREST_PAGE=false,
		INTEREST_PAGE=true
	};

public:
	CObjPlayerInfo*					m_pObjPlayerInfo;
	POINT							m_pDNDPoint;

	// 분류 관련
	int								m_nSmallFractionStart;					// 소분류 시작위치
	int								m_nBigFractionIndex;					// 대분류 번호
	int								m_nSmallFractionIndex;					// 소분류 번호
	
	
	// 아이템 정보관련
	CObjItemInfo					m_ItemList[LIST512_SIZE];				// 아이템 샵 리스트
	CObjItemInfo					m_CurrentItemList[LIST6_SIZE];			// 아이템 샵 리스트
	CResAiCardTableFormat			m_cCardList[LIST512_SIZE];				// 카드 리스트
	int								m_nShopEquipWear[MAX_EQUIP];			//착용 의상	
	CResBaseFormat*					m_pEquipWearModelID[MAX_EQUIP];			//착용 의상 아이디
	int                             m_nSelectIndex;
	int								m_nSkin;
	int								m_nTableSell;
	int								m_ItemTotalCount;
	int								m_nCurrentTime;

	bool							m_bLookChange;
	bool							m_bCardAni;

	CCardInfo						m_cCardInfo;

	// 페이지 관련
	int								m_nCurrentPage;
	int								m_nTotalPage;

	// 임시
	char							m_nOption[6];
	int								m_nOptionCode[6];
	
	// 분류 벡트리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	typedef vector<int>				SMALLFRACTIONTYPE_VECTORLIST;

	FRACTION_VECTORLIST				m_vFractionList[5];
	SMALLFRACTIONTYPE_VECTORLIST	m_vSmallFractionTypeList[5];

public:
	CItemShopProcess();
	virtual ~CItemShopProcess(void);

	virtual void					InitProcess();
	CBaseProcess*					GetSameProcess();

	virtual void					RenderFrame();
	virtual bool					OnDefault(NiEventRef pEventRecord);
	virtual void					Update(float fFrameTime, float fAccumTime);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();

    virtual void					Terminate();
	
	void							LoadGround();
	void							LoadCharacter();
	void							UpdateCamera(int Index);

	void							ReloadCharacter();						//3D캐릭터 다시 로드
	void							CharacterEquipInit();					//본래 장비 다시 로드
	void							FUNCTION_3DRotation();
	void							FUNCTION_ExampleExchangeItem(int nCode, int nWear = -1, bool bAlways = false );
	
	void							FUNCTION_BigFractionInitShop(char nBigFraction);		// 대분류 별 초기화
	virtual void					FUNCTION_InitShop();					// 아이템 샾 들어갈때 초기화
	virtual void					FUNCTION_UpdateShop();					// 아이템 샾 업데이트
	void							FUNCTION_ItemRender( int idx );

	// 구매 관련
	void							FUNCTION_BuyItem(char idx);
	void							FUNCTION_ExchangeItem(char idx);

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							FUNCTION_BigFraction(int nBigFraction);
	void							FUNCTION_SmallFractionInit(int nBigFraction);
	void							FUNCTION_SmallFraction(int nSmallFraction);

	// 아이템 코드 관련
	void							InitItemList();
	void							InitItemCode(int nType);
	void							UpdateItemCode(int nPage);
	void							InitCardList();
	void							UpdateCardList();

	// 페이지 관련
	void							FUNCTION_GoPage(int nPage);
	short							FUNCTION_UpdatePage(short nPage);

	// 기타 UI 관련
	void							FUNCTION_Selection();
	void							FUNCTION_ColorNext(int idx);
	void							FUNCTION_ColorPrev(int idx);
	void							FUNCTION_DivestItem(int idx);

	void							FUNCTION_SetCardBooster(CCardInfo* pInfo);
	void							FUNCTION_CardBoosterUpdate();

	// 툴팁 관련
	void							FUNCTION_ItemShopToolTipViewUpdate();
	void							FUNCTION_ItemShopEquipWearToolTipViewUpdate();

};