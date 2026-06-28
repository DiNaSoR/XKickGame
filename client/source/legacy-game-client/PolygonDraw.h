#pragma once

class CPolygonDraw
{
private:
	LPDIRECT3DDEVICE9 m_pd3dDevice;

public:
	CPolygonDraw(void);
	CPolygonDraw(LPDIRECT3DDEVICE9 pd3dDevice);
	~CPolygonDraw(void);

	void DrawPolygon(char* sTargetControl, int nPolygonNum, float fBaseScale, D3DXCOLOR* color, D3DXVECTOR3 vSink, float fVertexScale[LIST32_SIZE]);

    struct Vertex
    { 
    public:
        Vertex()
        {
        }
		
        Vertex( float x, float y, float z, float w, D3DCOLOR color )
        {
            this->m_vP.z		= x;
            this->m_vP.y		= y;
			this->m_vP.z		= z;
			this->m_fW			= w;
            this->m_nColor		= color;
        }

        D3DXVECTOR3     m_vP;
		float			m_fW;
        D3DCOLOR        m_nColor;
    };
};
