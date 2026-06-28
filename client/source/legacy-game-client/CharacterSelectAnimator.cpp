#include "stdafx.h"
#include "CharacterSelectAnimator.h"

CharacterSelectAnimator::CharacterSelectAnimator()
{
    m_nSelectIndex = -1;
}

CharacterSelectAnimator::~CharacterSelectAnimator()
{
}

void CharacterSelectAnimator::SetAnimationIdlePosition( CharacterInfo & cCharacterInfo )
{
    CResCharacterFormat * pChar = cCharacterInfo.m_pResCharacter;

    NiNode * pkFootStep         = (NiNode*) pChar->m_spNode->GetObjectByName( "Bip01 Footsteps" );
    NiPoint3 kFootStepWorldPos  = pkFootStep->GetWorldTranslate();

    NiPoint3 kTargetPos         = kFootStepWorldPos + cCharacterInfo.m_cTestPos;

    NiTransform kWorldTransform = pChar->m_spNode->GetParent()->GetWorldTransform();

    NiPoint3 kTrn = ( kTargetPos - kWorldTransform.m_Translate ) * kWorldTransform.m_Rotate;

    pChar->m_spNode->SetTranslate( kTrn - cCharacterInfo.m_cBiasPos );
}

void CharacterSelectAnimator::Update( float fTime )
{
    m_fCurrentTime = fTime;

    for( int i=0; i<m_snMaxSelectableCharacter; i++ )
    {
        CharacterInfo * pCharInfo = &m_cCharacterInfo[i];
        if( pCharInfo->m_pResCharacter == 0 ) continue;

        if( m_fCurrentTime < pCharInfo->m_fCurrentAnimationEndTime ) continue;

        switch( pCharInfo->m_eCurrentState )
        {
        case CharacterInfo::STATE_BEHINDING:
            {
                NiMatrix3 cRotate;
                cRotate.MakeZRotation( pCharInfo->m_fAngle * 3.141592f / 180.0f );

                pCharInfo->m_pResCharacter->m_spNode->SetRotate( cRotate );

                this->SetAnimationIdlePosition( *pCharInfo );

                pCharInfo->m_pResCharacter->SetTargetAnimation( pCharInfo->m_nIdleAnimation );
                pCharInfo->m_eCurrentState = CharacterInfo::STATE_BEHIND_IDLE;
            }
        break;

        case CharacterInfo::STATE_FRONTING:
            {
                NiMatrix3 cRotate;
                cRotate.MakeZRotation( pCharInfo->m_fAngle * 3.141592f / 180.0f );

                pCharInfo->m_pResCharacter->m_spNode->SetRotate( cRotate );

                this->SetAnimationIdlePosition( *pCharInfo );

                pCharInfo->m_pResCharacter->SetTargetAnimation( pCharInfo->m_nIdleAnimation );
                pCharInfo->m_eCurrentState = CharacterInfo::STATE_FRONT_IDLE;
            }
        break;

        case CharacterInfo::STATE_FRONT_IDLE:
        case CharacterInfo::STATE_BEHIND_IDLE:

        break;

        default:
            assert( !"CharacterInfo State Error!" );
        break;
        }
    }
}

void CharacterSelectAnimator::SetCharacter( CResCharacterFormat * pResCharacter, int nIndex, int nFrontAnimation, int nBehindAnimation, int nIdleAnimation, float fAngle )
{
    assert( nIndex < m_snMaxSelectableCharacter );
    if( nIndex >= m_snMaxSelectableCharacter ) return;

    CharacterInfo * pCharInfo = &m_cCharacterInfo[nIndex];
    if( pResCharacter == 0 )
    {
        pCharInfo->Init();
        return;
    }

    NiMatrix3 cRotate;
    cRotate.MakeIdentity();

    pResCharacter->m_spNode->SetTranslate( NiPoint3(0.0f,0.0f,0.0f) );
    pResCharacter->m_spNode->SetRotate( cRotate );
    pResCharacter->m_spNode->Update( 0.0f );

    pCharInfo->m_pResCharacter          = pResCharacter;
    pCharInfo->m_eCurrentState          = CharacterInfo::STATE_BEHIND_IDLE;

    pCharInfo->m_nBehindAnimation       = nBehindAnimation;
    pCharInfo->m_nFrontAnimation        = nFrontAnimation;
    pCharInfo->m_nIdleAnimation         = nIdleAnimation;

    NiControllerManager * pkControllerManager = pResCharacter->m_pkControllerManager;

    char buf[512];
    sprintf( buf, "%d", nFrontAnimation );

    NiControllerSequence * pkSeq = pkControllerManager->GetSequenceByName( buf );
    pCharInfo->m_fFrontAnimationEndTime = pkSeq->GetLength(); //    = 2.0f;

    sprintf( buf, "%d", nBehindAnimation );
    pkSeq = pkControllerManager->GetSequenceByName( buf );

    pCharInfo->m_fBackAnimationEndTime  = pkSeq->GetLength();

    pCharInfo->m_fAngle = fAngle;

    pResCharacter->SetTargetAnimation( nFrontAnimation );
    pResCharacter->m_spNode->Update( 0.0f );

    NiNode * pkNode = (NiNode*) pResCharacter->m_spNode->GetObjectByName( "Bip01 Footsteps" );
	_Assert(pkNode, "Bip01 Footsteps is ¾øÀ½", 0, "", 0);

    pCharInfo->m_cAniStartPos = pkNode->GetWorldTranslate();

    pResCharacter->SetTargetAnimation( nIdleAnimation );

    pkNode = (NiNode*) pResCharacter->m_spNode->GetObjectByName( "Bip01 Footsteps" );

    pCharInfo->m_cOrgFootStepPos = pkNode->GetWorldTranslate();

    pResCharacter->SetUpdateStateMachine( false );

    pResCharacter->m_spNode->Update( 0.0f );
}

void CharacterSelectAnimator::SelectCharacter( int nIndex, bool bEnd )
{
    assert( nIndex < m_snMaxSelectableCharacter );
    if( nIndex >= m_snMaxSelectableCharacter ) return;
    
    if( m_nSelectIndex == nIndex ) return;
    if( m_cCharacterInfo[nIndex].m_pResCharacter == 0 ) return;

    if( this->IsAllCharacterDone() == false ) return;

    this->AllCharacterBehind( bEnd );

    CharacterInfo * pCharInfo = &m_cCharacterInfo[nIndex];
    if( pCharInfo->m_pResCharacter == 0 ) return;

    this->CharacterFront( *pCharInfo, bEnd );

    m_nSelectIndex = nIndex;
}

void CharacterSelectAnimator::SelectCharacterBySeq( int nSeq )
{
    for( int i=0; i<m_snMaxSelectableCharacter; i++ )
    {
        CResCharacterFormat * pChar = m_cCharacterInfo[i].m_pResCharacter;
        if( pChar == 0 ) continue;

        if( pChar->GetObjSeq() == nSeq ) 
        {
            this->SelectCharacter( i );
            return;
        }
    }
}

void CharacterSelectAnimator::Reset()
{
    for( int i=0; i<m_snMaxSelectableCharacter; i++ )
    {
        m_cCharacterInfo[i].Init();
    }
    m_nSelectIndex = -1;
}

int CharacterSelectAnimator::GetSelectIndex()
{
    return m_nSelectIndex;
}

int CharacterSelectAnimator::GetSelectSequence()
{
    if( m_nSelectIndex < 0 ) return -1;

    assert( m_nSelectIndex >= 0 && m_nSelectIndex < m_snMaxSelectableCharacter );
    if( m_nSelectIndex < 0 || m_nSelectIndex >= m_snMaxSelectableCharacter ) return -1;

    CResCharacterFormat * pSelectChar = m_cCharacterInfo[m_nSelectIndex].m_pResCharacter;
    assert( pSelectChar != 0 );
    if( pSelectChar == 0 ) return -1;

    return pSelectChar->GetObjSeq();
}

bool CharacterSelectAnimator::IsAllCharacterDone()
{
    for( int i=0; i<m_snMaxSelectableCharacter; i++ )
    {
        CharacterInfo * pCharInfo = &m_cCharacterInfo[i];
        if( 
            pCharInfo->m_eCurrentState == CharacterInfo::STATE_BEHINDING ||
            pCharInfo->m_eCurrentState == CharacterInfo::STATE_FRONTING 
          ) 
        {
            return false;
        }
    }
    return true;
}

void CharacterSelectAnimator::AllCharacterBehind( bool bEnd )
{
    for( int i=0; i<m_snMaxSelectableCharacter; i++ )
    {
        CharacterInfo * pCharInfo = &m_cCharacterInfo[i];
        this->CharacterBehind( *pCharInfo, bEnd );
    }
}

void CharacterSelectAnimator::CharacterBehind( CharacterSelectAnimator::CharacterInfo & cCharacterInfo, bool bEnd )
{
    if( cCharacterInfo.m_eCurrentState != CharacterInfo::STATE_FRONT_IDLE ) return;

    float fOffsetTime = 0.0f;
    if( bEnd == true ) 
        fOffsetTime = -cCharacterInfo.m_fBackAnimationEndTime;
    else
        fOffsetTime = -NI_INFINITY;

    NiMatrix3 cIdentity;
    cIdentity.MakeIdentity();

    cCharacterInfo.m_pResCharacter->m_spNode->SetRotate( cIdentity );
    cCharacterInfo.m_eCurrentState = CharacterInfo::STATE_BEHINDING;

    CResCharacterFormat * pResChar = cCharacterInfo.m_pResCharacter;

    NiPoint3 kAnimationStartPos = NiPoint3( 0.0f, 0.0f, 0.0f );

    pResChar->m_spNode->SetTranslate( kAnimationStartPos + cCharacterInfo.m_cBiasPos );
    pResChar->m_spNode->Update( m_fCurrentTime );

    cCharacterInfo.m_cTestPos = pResChar->m_spNode->GetWorldTranslate() - cCharacterInfo.m_cOrgFootStepPos;

    pResChar->SetTargetAnimation( cCharacterInfo.m_nBehindAnimation, 1.0f , fOffsetTime );

    if( fOffsetTime == -NI_INFINITY ) fOffsetTime = 0.0f;
    cCharacterInfo.m_fCurrentAnimationEndTime = m_fCurrentTime + cCharacterInfo.m_fBackAnimationEndTime + fOffsetTime;
}

void CharacterSelectAnimator::CharacterFront( CharacterSelectAnimator::CharacterInfo & cCharacterInfo, bool bEnd )
{
    if( cCharacterInfo.m_eCurrentState != CharacterInfo::STATE_BEHIND_IDLE ) return;

    float fOffsetTime = 0.0f;
    if( bEnd == true ) 
        fOffsetTime = -cCharacterInfo.m_fBackAnimationEndTime;
    else
        fOffsetTime = -NI_INFINITY;


    NiMatrix3 cIdentity;
    cIdentity.MakeIdentity();

    cCharacterInfo.m_pResCharacter->m_spNode->SetRotate( cIdentity );

    cCharacterInfo.m_eCurrentState = CharacterInfo::STATE_FRONTING;

    NiTransform kWorldTransform = cCharacterInfo.m_pResCharacter->m_spNode->GetParent()->GetWorldTransform();

    cCharacterInfo.m_cBiasPos = ( ( cCharacterInfo.m_cOrgFootStepPos - cCharacterInfo.m_cAniStartPos ) ) * kWorldTransform.m_Rotate;

    CResCharacterFormat * pResChar = cCharacterInfo.m_pResCharacter;

    NiPoint3 kAnimationStartPos = NiPoint3( 0.0f, 0.0f, 0.0f );

    pResChar->m_spNode->SetTranslate( kAnimationStartPos + cCharacterInfo.m_cBiasPos );
    pResChar->m_spNode->Update( m_fCurrentTime );

    cCharacterInfo.m_cTestPos = pResChar->m_spNode->GetWorldTranslate() - cCharacterInfo.m_cOrgFootStepPos;

    pResChar->Update( 0.0f, m_fCurrentTime );
    pResChar->SetTargetAnimation( cCharacterInfo.m_nFrontAnimation, 1.0f, fOffsetTime );

    if( fOffsetTime == -NI_INFINITY ) fOffsetTime = 0.0f;
    cCharacterInfo.m_fCurrentAnimationEndTime = m_fCurrentTime + cCharacterInfo.m_fFrontAnimationEndTime + fOffsetTime;
}
