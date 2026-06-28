#ifndef __COOL_TIME_ICON_H__
#define __COOL_TIME_ICON_H__

#include "d3d9.h"
#include "ComStructure.h"
#include "UIControl.h"
#include <vector>

// Skill Icon 의 CoolTime 을 Skill Icon 위에서 시계 방향으로 진행하면서
// 검은색으로 블렌딩한다.
class CoolTimeIcon
{
public:
    enum ROTATE
    {
        eRight = 0,
        eLeft  = 1,
    };

public:
    CoolTimeIcon( LPDIRECT3DDEVICE9 pDevice );
    CoolTimeIcon();

    virtual ~CoolTimeIcon();

public:
    void    SetUIControl    ( CUIControl * pkControl ); //덮을 UI Control 설정 : pkControl 사이즈만큼 Gage 를 만든다.
    void    SetStart        ( float fStartValue );      //Gage 시작값 설정
    void    SetEnd          ( float fEndValue );        //Gage 끝값설정
    void    SetValue        ( float fCurrentValue );    //현재 Gage값
    void    SetColor        ( D3DCOLOR nColor );        //블렌딩할 색
    void    SetSize         ( int nWidth, int nHeight ); // Gage의 사이즈설정
    void    SetStartPoint   ( NiPoint2 & kStartPoint ); // 게이지의 위치
    void    SetRotate       ( ROTATE eRotate );           // 왼쪽으로 돌지 오른쪽으로 돌지 설정
    void    SetDevice       ( LPDIRECT3DDEVICE9 pDevice ); // D3D9 Device 설정

public:
    float   GetValue(); // 현재 Gage 값 설정

protected:
    void Initialize(); // 초기화

public:
    void Start       ( float fStartValue, float fEndValue ); // 게이지 시작값과 끝값을 설정한다.
    void Update      ( float fValue ); //현재 게이지 값을 설정한다.
    void SetVisible  ( bool bVisible ); // 게이지를 표시할지 안할지 설정

    void Draw(); // 렌더링

protected:
    struct LinkedPoint
    {
    public:
        NiPoint2 m_cPoint;

        LinkedPoint * m_pkRight;
        LinkedPoint * m_pkLeft;
    };

protected:
    // pkPoints 에 점들을 연결한 4개의 선분과 cLine1, cLine2 사이에 교점들, 그리고 cLine1, cLine2 사이에 있는 pkPoints에 정점들을 모든 pkReturnPts에 돌려준다.
    int           CalculateVertex( LinkedPoint * pkPoints, CLine2D & cLine1, CLine2D & cLine2, ROTATE eRotate, NiPoint2 * pReturnPts );

    //pkPoints 에 점들을 연결한 선분과 cLine 과의 교점을 cIntersectPt 에 돌려주고 교점에 eRotate 방향( 왼쪽이나 오른쪽 ) 에 pkPoints 점을 돌려준다.
    LinkedPoint * FindVertex( LinkedPoint * pkPoints,  CLine2D & cLine, ROTATE eRotate, NiPoint2 & cIntersectPt );

protected:
    void DestroyLinkedPoints( LinkedPoint * pkPoints );

protected:
    LPDIRECT3DDEVICE9   m_pDevice;

    float               m_fRatio;

    int                 m_nWidth;
    int                 m_nHeight;

    float               m_fStartValue;
    float               m_fEndValue;
    float               m_fCurrentValue;

    D3DCOLOR            m_nColor;

    NiPoint2            m_cStartPt;
    NiPoint2            m_cEndPt;

    LinkedPoint *       m_pkPoints;

    CUIControl *        m_pkControl;
    ROTATE              m_eRotate;

    bool                m_bVisible;
};

struct CoolTimeInfo
{
public:
	CoolTimeInfo()
	{
		m_nSkillCode = -1;
		m_bEnable = false;

		m_fCurrentCoolTime = 0.0f;
		m_fCoolTime        = 0.0f;

		m_pControl         = 0;
	}

public:
	bool            m_bEnable;

	float           m_fCurrentCoolTime;
	float           m_fCoolTime;

	CoolTimeIcon    m_cCoolTimeIcon;
	CUIControl   *  m_pControl;

	int             m_nSkillCode;
};

typedef std::vector< CoolTimeIcon * > CoolTimeIconArray;

#endif
