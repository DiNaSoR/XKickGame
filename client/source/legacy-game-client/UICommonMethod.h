#pragma once

#include "UIButton.h"
#include "PacketForm.h"

class CObjPlayerInfo;
namespace UICommonMethod
{
	enum
	{
		AWAY_TEAM,
		HOME_TEAM
	};

	void OnClickTab(const char* sIndexstr, const char* sTabCommonName, const char* sDetailTabCommonName);
	void OnClickTab(const char* sIndexstr, const char* sDetailTabCommonName);
	void OnClickTabInvisible(const char* sIndexstr, const char* sDetailTabCommonName);
	
	void DrawPositionIcon( char* sTarget, DWORD nPositionCode, bool bHomeTeam );
	void DrawPositionIcon2( char* sTarget, DWORD nPositionCode, bool bHomeTeam );

	void PositionIconDraw( char* sTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw( CUIControl* pTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw( char* sParent, char* sTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw( CUIControl* pParent, char* sTarget, char nPositionCode, bool bHomeTeam );

	void PositionIconDraw2( char* sTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw2( CUIControl* pTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw2( char* sParent, char* sTarget, char nPositionCode, bool bHomeTeam );
	void PositionIconDraw2( CUIControl* pParent, char* sTarget, char nPositionCode, bool bHomeTeam );

	void TurnOffAllChild(char* sTarget);

	void DisplayGameVersion();
	void DisplayMoney();
	void DisplayBasicInfo(CObjPlayerInfo* pPlayer);
	void DisplayAll(CObjPlayerInfo* pPlayer);
	void DisPlayNotice();
	void DisplayMenuToolTip();
	void DisplayMenuToolTip(char idx);

	bool IsRectInPoint(LPRECT rt, LPPOINT pt);

	void BGEffectAnimationInit(char* sWindow1);
	void BGEffectAnimationInit(char* sWindow1, char* sWindow2);
	void BGEffectAnimationInit(char* sWindow1, char* sWindow2, char* sWindow3);
	void BGEffectAnimationPlay(char* sWindow1);
	void BGEffectAnimationPlay(char* sWindow1, char* sWindow2);
	void BGEffectAnimationPlay(char* sWindow1, char* sWindow2, char* sWindow3);

	void BasicInfoStop();

	// 规积己 包访
	void GameChange(int nCode);
	void WeatherChange(int nCode);
	void TimeChange(int nCode);
	void AiChange(int nCode);
	void PointChange(int nCode);
	void ManCountChange(int nCode);

	// 墨靛包访
	std::string						PositionNtoPositionStr(char nPosition);
	char							RankNtoRankChar(char nRank);
	D3DCOLOR						RankNtoRankColor(char nRank);
	void							FUNCTION_CardInfoDraw(CUIControl* pParent, int nPosition, int nRank, int nLv, int nCostume);

	char*							SituationCodeToStr( int nSituationCode );
	char*							OptionCodeToStr( int nFacultyCode );
};

class CUIPopUp
{
private:
	vector<CUIControl*> m_vControlList;
	CUIControl* m_cTargetControl;
public:
	CUIPopUp(CUIControl* cTargetControl);
	CUIPopUp(const char* sTargetControl);
	~CUIPopUp();
	void Add_Control(CUIControl* cUIControl);
	void Add_Control(const char* sControlName);
	bool WhetherHoverControls();
};