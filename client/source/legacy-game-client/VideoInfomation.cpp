#include "GameProject.h"
#include "VideoInfomation.h"

VideoInfomation::VideoInfomation()
{
    m_nAnti = eMultiSampleQuality::eNoneMultiSampleQuality;
}
VideoInfomation::~VideoInfomation(){
}
void VideoInfomation::SetCharacterLight( char nEnable )
{
    m_nCharacterLight = nEnable;
}
void VideoInfomation::SetLightMap( char nEnable ){    m_nLightMap = nEnable;}
void VideoInfomation::SetWeather( char nWeather ){    m_nWeather = nWeather;}
void VideoInfomation::SetGroundEffect( char nGroundEffect ){    m_nGround = nGroundEffect;}
void VideoInfomation::SetGameEffect( char nGameEffect ){    m_nEffect = nGameEffect;}
void VideoInfomation::SetShadow( char nShadow ){    m_nShadow = nShadow;}
void VideoInfomation::SetAntiAliasing( eMultiSampleQuality eQuality ){    if( m_nAnti != -1 ) return;    if( eQuality >= eMultiSampleQuality::eMiddleMultiSampleQuality )  eQuality = eMultiSampleQuality::eMiddleMultiSampleQuality;    m_nAnti = eQuality;}
void VideoInfomation::SetSpecular( char nSpecular ){    m_nSpecular = nSpecular;}
char VideoInfomation::GetCharacterLight(){    return m_nCharacterLight;}
char VideoInfomation::GetLightMap(){    return m_nLightMap;}
char VideoInfomation::GetWeather(){    return m_nWeather;}
char VideoInfomation::GetGroundEffect(){    return m_nGround;}
char VideoInfomation::GetGameEffect(){    return m_nEffect;}
char VideoInfomation::GetShadow(){    return m_nShadow;}
VideoInfomation::eMultiSampleQuality VideoInfomation::GetAntiAliasing(){    return m_nAnti;}
char VideoInfomation::GetSpecular(){    return m_nSpecular;}
void LoadVideoInfomation(){    TiXmlElement * pVideoOption = CGameProject::s_pGameProject->GetTiXml("VideoOption");
    TiXmlNode* pChild = pVideoOption->FirstChild( "Performance" );
    TiXmlElement* pElement = (TiXmlElement*)pChild;
    char nPerformance = (char) atoi( pElement->Attribute( "Value" ) );

    pChild = pChild->NextSibling( "CharacterLight" );
    pElement = (TiXmlElement*)pChild;
    char nCharacterLight = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "LightMap" );
    pElement = (TiXmlElement*)pChild;
    char nLightMap = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Weather" );
    pElement = (TiXmlElement*)pChild;
    char nWeather = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Ground" );
    pElement = (TiXmlElement*)pChild;
    char nGround = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Effect" );
    pElement = (TiXmlElement*)pChild;
    char nEffect = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Shadow" );
    pElement = (TiXmlElement*)pChild;
    char nShadow = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Anti" );
    pElement = (TiXmlElement*)pChild;
    char nAnti = (char) atoi( pElement->Attribute( "Value" ) );
    pChild = pChild->NextSibling( "Specular" );
    pElement = (TiXmlElement*)pChild;
    char nSpecular = (char) atoi( pElement->Attribute( "Value" ) );
    VideoInfomation * pVideoInfomation = VideoInfomation::GetPtr();
    pVideoInfomation->SetCharacterLight( nCharacterLight );
    pVideoInfomation->SetGameEffect( nEffect );
    pVideoInfomation->SetGroundEffect( nGround );
    pVideoInfomation->SetLightMap( nLightMap );
    pVideoInfomation->SetShadow( nShadow );
    pVideoInfomation->SetSpecular( nSpecular );
    pVideoInfomation->SetWeather( nWeather );
    pVideoInfomation->SetAntiAliasing( (VideoInfomation::eMultiSampleQuality) nAnti );}