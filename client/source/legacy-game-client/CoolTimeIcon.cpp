#include "CoolTimeIcon.h"
#include "d3dx9math.h"
#include "UtlMath.h"
#include <cassert>

CoolTimeIcon::CoolTimeIcon()
{
    this->Initialize();
}

CoolTimeIcon::CoolTimeIcon( LPDIRECT3DDEVICE9 pDevice )
{
    this->Initialize();

    assert( pDevice != 0 );
    m_pDevice = pDevice;
}

CoolTimeIcon::~CoolTimeIcon()
{
    this->DestroyLinkedPoints( m_pkPoints );
    m_pkPoints = 0;
}

void CoolTimeIcon::SetDevice( LPDIRECT3DDEVICE9 pDevice )
{
    m_pDevice = pDevice;
}

void CoolTimeIcon::Initialize()
{
    m_pDevice   = 0;

    m_fRatio    = 0.0f;

    m_nWidth    = 0;
    m_nHeight   = 0;

    m_fStartValue   = 0.0f;
    m_fEndValue     = 0.0f;
    m_fCurrentValue = 0.0f;

    m_nColor = D3DCOLOR_ARGB( 200, 0, 0, 0 );

    m_pkPoints = 0;

    m_pkControl = 0;
    m_eRotate   = ROTATE::eRight;

    m_bVisible  = false;
}

void CoolTimeIcon::DestroyLinkedPoints( CoolTimeIcon::LinkedPoint * pkPoints )
{
    if( pkPoints == 0 ) return;

    LinkedPoint * pkEndPoint = pkPoints->m_pkLeft;
    for( LinkedPoint * pkCurPoint = pkPoints; pkCurPoint != pkEndPoint; )
    {
        LinkedPoint * pkNextPoint = pkCurPoint->m_pkRight;
        delete pkCurPoint;

        pkCurPoint = pkNextPoint;
    }
}

void CoolTimeIcon::SetVisible( bool bVisible )
{
    m_bVisible = bVisible;
}

void CoolTimeIcon::SetStart( float fStartValue )
{
    m_fStartValue = fStartValue;
}

void CoolTimeIcon::SetEnd( float fEndValue )
{
    m_fEndValue = fEndValue;
}

void CoolTimeIcon::SetUIControl( CUIControl * pkControl )
{
    m_pkControl = pkControl;
    
    RECT sRect = pkControl->GetWindowBox();

    this->SetStartPoint( NiPoint2( (float) sRect.left, (float) sRect.top ) );
    this->SetSize( sRect.right - sRect.left, sRect.bottom - sRect.top );
}

void CoolTimeIcon::SetRotate( CoolTimeIcon::ROTATE eRotate )
{
    m_eRotate = eRotate;
}

void CoolTimeIcon::SetSize( int nWidth, int nHeight )
{
    this->DestroyLinkedPoints( m_pkPoints );
    m_pkPoints = 0;

    m_nWidth  = nWidth;
    m_nHeight = nHeight;

    m_cEndPt = m_cStartPt + NiPoint2( float(m_nWidth), float(m_nHeight) );

    LinkedPoint * pkLeftTop = new LinkedPoint();
    pkLeftTop->m_cPoint = m_cStartPt;

    LinkedPoint * pkRightTop = new LinkedPoint();
    pkRightTop->m_cPoint = NiPoint2( m_cEndPt.x, m_cStartPt.y );

    LinkedPoint * pkRightBottom = new LinkedPoint();
    pkRightBottom->m_cPoint = m_cEndPt;

    LinkedPoint * pkLeftBottom = new LinkedPoint();
    pkLeftBottom->m_cPoint = NiPoint2( m_cStartPt.x, m_cEndPt.y );

    pkLeftTop->m_pkRight     = pkRightTop;
    pkRightTop->m_pkRight    = pkRightBottom;
    pkRightBottom->m_pkRight = pkLeftBottom;
    pkLeftBottom->m_pkRight  = pkLeftTop;

    pkLeftTop->m_pkLeft      = pkLeftBottom;
    pkLeftBottom->m_pkLeft   = pkRightBottom;
    pkRightBottom->m_pkLeft  = pkRightTop;
    pkRightTop->m_pkLeft     = pkLeftTop;

    m_pkPoints = pkLeftTop;
}

CoolTimeIcon::LinkedPoint * CoolTimeIcon::FindVertex( LinkedPoint * pkPoints,  CLine2D & cLine, ROTATE eRotate, NiPoint2 & cIntersectPt )
{
    LinkedPoint * pkStartPoint = pkPoints;
    LinkedPoint * pkCurPoint   = pkPoints;
    LinkedPoint * pkNextPoint  = 0;

    int nLineInterCnt = 0;
    do
    {
        if( eRotate == eRight )
            pkNextPoint = pkCurPoint->m_pkRight;
        else
            pkNextPoint = pkCurPoint->m_pkLeft;

        CLine2D cTestLine( pkCurPoint->m_cPoint, pkNextPoint->m_cPoint );

        NiPoint2 cInterPt;
        bool bResult = IntersectLine( cTestLine, cLine, cIntersectPt );
        if( bResult == true )
        {
            return pkNextPoint;
        }

        pkCurPoint = pkNextPoint;

    } while( pkStartPoint != pkCurPoint );

    return 0;
}

int CoolTimeIcon::CalculateVertex( LinkedPoint * pkPoints, CLine2D & cLine1, CLine2D & cLine2, ROTATE eRotate, NiPoint2 * pReturnPts )
{
    NiPoint2 cLine1IntersectPt;
    NiPoint2 cLine2IntersectPt;

    LinkedPoint * pkLine1LinkPt = FindVertex( pkPoints, cLine1,  eRotate, cLine1IntersectPt );
    LinkedPoint * pkLine2LinkPt = FindVertex( pkPoints, cLine2,  eRotate, cLine2IntersectPt );

    bool bCycle = false;

    if( pkLine1LinkPt == pkLine2LinkPt )
    {
        NiPoint2 cTestLine1 = pkLine1LinkPt->m_cPoint - cLine1IntersectPt;
        NiPoint2 cTestLine2 = pkLine1LinkPt->m_cPoint - cLine2IntersectPt;
        
        float fLen1 = cTestLine1.Length();
        float fLen2 = cTestLine2.Length();

        if( eRotate == eRight && fLen1 <= fLen2 )
        {
            bCycle = true;
        }

        if( eRotate == eLeft && fLen1 <= fLen2 )
        {
            bCycle = true;
        }
    }

    int nCnt = 0;

    pReturnPts[nCnt++] = cLine1IntersectPt;

    for( LinkedPoint * pkCurPt = pkLine1LinkPt; pkCurPt != pkLine2LinkPt || bCycle;  )
    {        
        pReturnPts[ nCnt++ ] = pkCurPt->m_cPoint;

        if( eRotate == eRight )
            pkCurPt = pkCurPt->m_pkRight;
        else
            pkCurPt = pkCurPt->m_pkLeft;

        bCycle = false;
    }

    pReturnPts[ nCnt++ ] = cLine2IntersectPt;

    return nCnt;
}

void CoolTimeIcon::SetColor( D3DCOLOR nColor )
{
    m_nColor = nColor;
}

void CoolTimeIcon::Start( float fStartValue, float fEndValue )
{
    m_fStartValue = fStartValue;
    m_fEndValue   = fEndValue;
}

void CoolTimeIcon::SetStartPoint( NiPoint2 & kStartPoint )
{
    m_cStartPt = kStartPoint;
}

void CoolTimeIcon::Update( float fValue )
{
    m_fCurrentValue = fValue;

    if( m_fCurrentValue > m_fEndValue   ) m_fCurrentValue = m_fEndValue;
    if( m_fCurrentValue < m_fStartValue ) m_fCurrentValue = m_fStartValue;

    float fAmount = m_fEndValue - m_fStartValue;
    if( fAmount == 0.0f ) return;

    m_fRatio = ( m_fCurrentValue - m_fStartValue ) / fAmount;
}

float CoolTimeIcon::GetValue()
{
    return m_fCurrentValue;
}

void CoolTimeIcon::Draw()
{
    if( m_bVisible == false ) return;

    struct Vertex
    { 
    public:
        Vertex()
        {
        }

        Vertex( float x, float y, float z, float w, D3DCOLOR color )
        {
            this->m_fX        = x;
            this->m_fY        = y;
            this->m_fZ        = z;
            this->m_fW        = w;
            this->m_nColor    = color;
        }

        float           m_fX, m_fY, m_fZ, m_fW;
        D3DCOLOR        m_nColor;
    };

    int nLeft   = 0;
    int nRight  = m_nWidth;
    int nTop    = 0;
    int nBottom = m_nHeight;

    NiPoint2 kCenter = (m_cStartPt + m_cEndPt ) * 0.5f;

    int nDiagonalLenSqr = m_nHeight * m_nHeight + m_nWidth * m_nWidth;
    NiPoint2 kStartVector = NiPoint2( 0.0f, float(-nDiagonalLenSqr) );

    Vertex vertices[ 30 ];
    vertices[0] = Vertex( kCenter.x, kCenter.y, 0.0f, 1.0f, m_nColor );

    float fTargetAngleDegree = 360.0f * m_fRatio;
    float fTargetAngleRad    = float(fTargetAngleDegree) / 180.0f * 3.141592f;        

    NiPoint2 kRotateVector  = kStartVector;

    kRotateVector.x         = cosf(fTargetAngleRad) * kStartVector.x - sinf(fTargetAngleRad) * kStartVector.y;
    kRotateVector.y         = cosf(fTargetAngleRad) * kStartVector.y + sinf(fTargetAngleRad) * kStartVector.x;

    CLine2D cLine1( kCenter, kCenter + kStartVector  );
    CLine2D cLine2( kCenter, kCenter + kRotateVector );

    NiPoint2 pPoints[100];

    int nCnt = this->CalculateVertex( m_pkPoints, cLine1, cLine2, m_eRotate, pPoints ); //렌더링할 점들을 계산.
    for( int i=0; i<nCnt; i++ )
    {
        vertices[ i+1 ].m_fX = pPoints[i].x - 0.5f;
        vertices[ i+1 ].m_fY = pPoints[i].y - 0.5f;
        vertices[ i+1 ].m_fZ = 0.0f;
        vertices[ i+1 ].m_fW = 1.0f;
        vertices[ i+1 ].m_nColor = m_nColor;
    }
    nCnt++;

    m_pDevice->SetVertexShader( 0 );
    m_pDevice->SetPixelShader( 0 );
    
    m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

    m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
    m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

    if( nCnt - 2 > 0 )
    {
        m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nCnt - 2, vertices, sizeof(Vertex) );
    }

    m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

