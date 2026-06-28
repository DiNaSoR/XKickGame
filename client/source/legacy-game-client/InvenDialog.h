//***********************************************************************************
//	File		:	InvenDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#pragma once
#include "Singleton.h"

#include "Control.h"

#include <vector>

using namespace std;

class CInvenDialog : public CSingleton<CInvenDialog>
{
private:
	POINT							m_pDNDPoint;

	// 정보 저장 변수
	int								m_nItemCode[12];
	int								m_nItemSeq[12];
	int								m_nSkillCode[24];
	char							m_nEntry;

	int								m_nInvenEquipWear[MAX_EQUIP];

	// 페이지 관련
	int								m_nCurrentPage;
	int								m_nTotalPage;

	// 분류 관련
	int								m_nSmallFractionStart;			// 소분류 시작위치
	int								m_nBigFractionIndex;			// 대분류 번호
	int								m_nSmallFractionIndex;			// 소분류 번호

	CObjItemInfo					m_arrItem[MAX_INVEN];
	CObjSkillInfo					m_arrSkill[MAX_SKILL];

	// 분류 벡트리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	typedef vector<int>				SMALLFRACTIONTYPE_VECTORLIST;
	typedef vector<int>				ED_VECTORLIST;	

	FRACTION_VECTORLIST				m_vFractionList[5];
	SMALLFRACTIONTYPE_VECTORLIST	m_vSmallFractionTypeList[5];
	ED_VECTORLIST					m_vEDList;

public:
	CInvenDialog(void);
	~CInvenDialog(void);

	// 초기화 관련

	void							SetEntry(char nEntry);
	void							InitInven();

	void							FUNCTION_InvenConfirm();

	void							FUNCTION_InitItemInven();
	void							FUNCTION_InitEntryInven(char nEntry);
	void							FUNCTION_InitSkillInven();

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							SetSmallFractionStart(int nStart);
	void							FUNCTION_BigFraction(int nBigFraction);
	void							FUNCTION_SmallFractionInit(int nBigFraction);
	void							FUNCTION_SmallFraction(int nSmallFractionIndex);	

	void							FUNCTION_GetItemBagList();
	void							FUNCTION_GetEntryList();
	void							FUNCTION_GetSkillBagList();
	void							FUNCTION_EquipNDivest(int idx);

	void							FUNCTION_UpdateSecond(char nCount);

	void							UpdateInven();
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void							FUNCTION_ItemBagToolTipViewUpdate();
	void							FUNCTION_EquipWearToolTipViewUpdate();
	void							FUNCTION_GoPage(int nPage);
	void							FUNCTION_UpdatePage();
	void							FUNCTION_ImageRender(int ItemIndex, int nCode, int nEquip, char *sIconFile = NULL, char nSkillLevel = 0 );

};
