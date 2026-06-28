#ifndef __VIDEO_INFOMATION_H__
#define __VIDEO_INFOMATION_H__
#include "Singleton.h"

void LoadVideoInfomation();

class VideoInfomation : public CSingleton<VideoInfomation>
{
public:
    VideoInfomation();
    virtual ~VideoInfomation();

public:
    enum eMultiSampleQuality
    {
        eNoneMultiSampleQuality   = -1,
        eLowestMultiSampleQuality =  0,
        eMiddleMultiSampleQuality =  1,
        eHighestMultiSampleQuality=  2,
    };

public:
    void                SetCharacterLight( char nEnable );
    void                SetLightMap( char nEnable );
    void                SetWeather( char nWeather );
    void                SetGroundEffect( char nGroundEffect );
    void                SetGameEffect( char nGameEffect );
    void                SetShadow( char nShadow );
    void                SetAntiAliasing( eMultiSampleQuality eQuality );
    void                SetSpecular( char nSpecular );

public:
    char                GetCharacterLight();
    char                GetLightMap();
    char                GetWeather();
    char                GetGroundEffect();
    char                GetGameEffect();
    char                GetShadow();
    eMultiSampleQuality GetAntiAliasing();
    char                GetSpecular();
    
protected:
    char				m_nCharacterLight;
    char				m_nLightMap;        // π›ªÁ∏ 
    char				m_nWeather;         // ≥Øææ ¿Ã∆Â∆Æ
    char				m_nGround;          // πË∞Ê ¿Ã∆Â∆Æ
    char				m_nEffect;          // ∞‘¿” ¿Ã∆Â∆Æ
    char				m_nShadow;
    eMultiSampleQuality m_nAnti;
    char				m_nSpecular;

};

#endif

