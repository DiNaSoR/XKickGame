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

class CItemBagProcess : public CBaseProcess
{
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_END
	};

	enum BIGFRACTION_KIND
	{
		BF_CLOTH		= 0,
		BF_CARD,
		BF_CLUB,
		BF_SPECIAL,
		BF_POST,
	};

public:

	CObjPlayerInfo*					m_pObjPlayerInfo;
	POINT							m_pDNDPoint;

	// 정보 관련
	CObjItemInfo					m_ItemList[LIST32_SIZE];			// 코드
	int                             m_nSelectIndex;

	// 분류 관련
	int								m_nSmallFractionStart;			// 소분류 시작위치
	int								m_nBigFractionIndex;			// 대분류 번호
	int								m_nSmallFractionIndex;			// 소분류 번호
	bool							m_bListShape;					// 아이템 리스트 형태
	char							m_nEnchantIndex;
	char							m_nEnchantBuyKind;

	// 페이지 관련
	int								m_nCurrentPage;
	int								m_nTotalPage;
	
	// 카드 관련
	bool							m_bDnd;
	bool							m_bEntry;
	bool							m_bMix;
	int								m_nCard;
	char							m_nEntry;
	char							m_nCardTabIndex;
	int								m_nMix[3];
	char							m_nMixReservedRank;
	
	// 카드정보 벡터리스트
	typedef vector<CCardInfo*>		CardInfoVectorList;
	CardInfoVectorList				m_vCIVectorList;

	// 분류 벡트리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	typedef vector<int>				SMALLFRACTIONTYPE_VECTORLIST;

	FRACTION_VECTORLIST				m_vFractionList[5];
	SMALLFRACTIONTYPE_VECTORLIST	m_vSmallFractionTypeList[5];

public:
	CItemBagProcess();
	virtual ~CItemBagProcess(void);

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
	void							FUNCTION_3DRotation();
	void							UpdateCamera(int Index);

	// 샵 관련
	virtual void					FUNCTION_InitShop();		//아이템 샾 들어갈때 초기화
	virtual void					FUNCTION_UpdateShop();		//아이템 샾 업데이트
	virtual void					FUNCTION_TerminateShop();	//아이템 샾 종료할때
	virtual void					FUNCTION_ItemRender(int ItemIndex);
	
	// 페이지
	void							FUNCTION_GoPage(int nPage);
	void							FUNCTION_UpdatePage();

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							FUNCTION_BigFraction(int nBigFraction);
	void							FUNCTION_SmallFractionInit(int nBigFraction);
	void							FUNCTION_SmallFraction(int nSmallFractionIndex);

	// 카드 관련
	void							FUNCTION_InitCardShop();
	void							FUNCTION_CardUpdate();
	void							FUNCTION_InitEntry();
	bool							FUNCTION_IsEntryPostion( char nEntry, char nPosition );
	void							FUNCTION_GetCardInfo();
	void							FUNCTION_DNDDOWN();
	void							FUNCTION_DNDMOVE();
	void							FUNCTION_DNDUP();
	void							FUNCTION_CardTabChange(char idx);
	void							FUNCTION_CardMix();
	void							FUNCTION_CardMixResult(CCardInfo cCardInfo);
	void							FUNCTION_CardDetailView(char idx);
	
	// 기타 UI관련
	void							FUNCTION_DivestBagItem(char idx);
	void							FUNCTION_DivestBagItemS(char idx);
	void							FUNCTION_EquipBagItem(char idx);
	void							ExchangeItem();
	void							FUNCTION_ExchangeItem(int nCode, char nWear);
	void							FUNCTION_ItemListExchange(bool bItemListShape);
	void							FUNCTION_Selection();
	virtual void					FUNCTION_GetBagList();

	// 인챈트 관련
	void							FUNCTIOIN_Enchant(char idx);
	void							FUNCTION_EnchantChange(char idx);
	void							FUNCTION_EnchantOk(char idx);

	// 툴팁
	void							FUNCTION_ItemBagToolTipViewUpdate();
	void							FUNCTION_ItemBagEquipWearToolTipViewUpdate();
	void							FUNCTION_ItemBagCardToolTipViewUpdate();
	void							FUNCTION_ItemBagEntryToolTipViewUpdate();

};