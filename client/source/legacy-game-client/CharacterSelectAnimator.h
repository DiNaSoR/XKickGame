#ifndef __CHARACTER_SELECT_ANIMATOR_H__
#define __CHARACTER_SELECT_ANIMATOR_H__

#include<vector>
#include "NiNode.h"
#include "NiControllerManager.h"

// 캐릭터 선택창에 뛰어나오는 캐릭터의 애니메이션을 셋팅하고 뛰어나온뒤 IDLE Animation 으로 전환할때 위치를 보정한다.
class CharacterSelectAnimator
{
public:
    CharacterSelectAnimator();
    virtual ~CharacterSelectAnimator();

protected:
    struct CharacterInfo
    {
    public:
        enum CHARACTER_STATE
        {
            STATE_BEHINDING, //뒤로 뛰어가는중 상태
            STATE_FRONTING, // 앞으로 뛰어나오는중 상태
            STATE_BEHIND_IDLE, //뒤에서 IDLE Animation
            STATE_FRONT_IDLE, // 앞에서 IDLE Animation
        };

    public:
        CharacterInfo()
        {
            this->Init();
        }

        void Init()
        {
            m_bSelected                 = false;
            m_eCurrentState             = STATE_BEHIND_IDLE;

            m_fFrontAnimationEndTime    = FLT_MAX;       
            m_fBackAnimationEndTime     = FLT_MAX;       
            m_fCurrentAnimationEndTime  = FLT_MAX;

            m_pResCharacter             = 0;

            m_nBehindAnimation          = -1; // 뒤로 뛰어들어가는 애니메이션 번호
            m_nFrontAnimation           = -1; // 앞으로 뛰어 나오는 애니메이션 번호
            m_nIdleAnimation            = -1; // IDLE 애니메니션 번호

            m_fAngle                    = 0.0f; // 초기 회전각도.
        }

    public:
        bool                    m_bSelected;

        int                     m_nFrontAnimation;
        int                     m_nBehindAnimation;
        int                     m_nIdleAnimation;

        float                   m_fFrontAnimationEndTime;
        float                   m_fBackAnimationEndTime;
        
        float                   m_fCurrentAnimationEndTime;

        CHARACTER_STATE         m_eCurrentState;
        CResCharacterFormat *   m_pResCharacter;

        NiPoint3                m_cOrgFootStepPos;
        NiPoint3                m_cTestPos; //애니메니션 시작할때 m_spNode - Bip01 Footsteps 값
        NiPoint3                m_cBiasPos; //애니메이션 시작할때 IDLE Animation 과의 Footstep Node 보정값
        NiPoint3                m_cAniStartPos; // 애니메이션 시작할때 IDLE Animation 의 Footstep Node World Translate 값

        float                   m_fAngle;
    };

public:
    int  GetSelectIndex(); //현재 선택된 캐릭터의 인덱스 번호를 돌려준다
    int  GetSelectSequence(); // 현재 선택된 캐릭터의 Sequence 번호를 돌려준다.

    // 캐릭터 초기화
    // pResCharacterFormat : 초기화할 캐릭터
    // nIndex : 슬롯번호
    // nFrontAnimation : 앞으로 뛰어나오는 애니메이션 번호
    // nBehindAnimation : 뛰로 뛰어 들어가는 애니메이션 번호
    // fAngle : 초기 회전각도.
    void SetCharacter( CResCharacterFormat * pResCharacter, int nIndex, int nFrontAnimation, int m_nBehindAnimation, int nIdleAnimation, float fAngle );

    //초기화
    void Reset();

    //캐릭터 선택 : 뒤에 있는 캐릭터는 앞으로 뛰어나오게 하고, 앞에 있는 캐릭터는 뒤로 뛰어들어가게 한다.
    // nIndex : 선택할 캐릭터의 슬롯번호
    // bEnd : true 이면 중간 애니메이션 없이 앞으로 나오거나 뒤로 들어간다.
    void SelectCharacter( int nIndex, bool bEnd = false );

    //캐릭터 선택 : Sequence 번호에 의해 캐릭터를 선택하게 한다.
    void SelectCharacterBySeq( int nSeq );
    
    //캐릭터 삭제 
    // nSeq : 삭제할 캐릭터의 Sequence 번호
    void RemoveCharacterBySeq( int nSeq );

    void Update( float fTime );

protected:
    //모든 캐릭터를 뒤어 뛰어들어가게 한다.
    void AllCharacterBehind( bool bEnd = false );

    // 캐릭터를 뛰어 나오게 한다.
    // cCharacterInfo : 뛰어나오게 할 캐릭터에 관한 정보
    // bEnd : true 이면 중간 애니메이션 없이 앞으로 나오거나 뒤로 들어간다.
    void CharacterFront ( CharacterInfo & cCharacterInfo, bool bEnd = false );

    // 캐릭터를 뛰어 들어가게 한다.
    // cCharacterInfo : 뛰어들어가게 할 캐릭터정보
    // bEnd : true 이면 중간 애니메이션 없이 앞으로 나오거나 뒤로 들어간다.
    void CharacterBehind( CharacterInfo & cCharacterInfo, bool bEnd = false );

    // 해당캐릭터에 IDLE Animation 을 셋팅한다.
    // cCharacterInfo : IDLE Animation 을 할 캐릭터 정보
    void SetAnimationIdlePosition( CharacterInfo & cCharacterInfo );

    // 모든 캐릭터가 앞으로 나오거나 뒤로 뛰어 들어가는 애니메이션이 끝났는지 돌려준다.
    bool IsAllCharacterDone();

protected:
    static const int        m_snMaxSelectableCharacter = 3;

    float                   m_fCurrentTime;
    int                     m_nSelectIndex;

protected:
    CharacterInfo   m_cCharacterInfo [ m_snMaxSelectableCharacter];
    int             m_nAnimationIndex[m_snMaxSelectableCharacter][2];
};

#endif
