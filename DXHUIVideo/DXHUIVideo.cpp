// DXHUIVideo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DXHUIVideo.h"
#include "shader_pixel.h"
#include "../DXHUI/DXUT/DXUT.h"

#include "D3DVideoRenderInterface.h"
#include "Display.h"
#include "Display_YUV420.h"
#include "Display_NV12.h"
#include "display_rgb24.h"
#include "Display_RGB32.h"
#include "display_yuv422.h"
struct REGISTER
{
	ID3DVRInterface::COLOR_SPACE cs ;
	CRenderFactory::CreateCallback pCallback ;
} ;
REGISTER Entries [] = {	{ID3DVRInterface::CS_YV12,  &CDisplay_YUV420::Create},
{ID3DVRInterface::CS_I420,  &CDisplay_YUV420::Create},		
{ID3DVRInterface::CS_NV12,  &CDisplay_NV12::Create},
{ID3DVRInterface::CS_YUY2,  &CDisplay_YUV422::Create},
{ID3DVRInterface::CS_UYVY,  &CDisplay_YUV422::Create},
{ID3DVRInterface::CS_RGB24, &CDisplay_RGB24::Create},
{ID3DVRInterface::CS_BGR24, &CDisplay_RGB24::Create},
{ID3DVRInterface::CS_RGB32, &CDisplay_RGB32::Create},
{ID3DVRInterface::CS_RGB16, &CDisplay_RGB32::Create},
{ID3DVRInterface::CS_RGB15, &CDisplay_RGB32::Create}
} ;


CDXHUIVideo::CDXHUIVideo(void)
{
	m_pDev = NULL;
	m_pDXDev= NULL;
	for ( int i = 0; i < SHADER_COUNT; i++ )
	{
		m_pPS[i] = NULL;
	}
	m_pDisplay = NULL;
	m_pShader = NULL;
	m_pConstable = NULL;
}


CDXHUIVideo::~CDXHUIVideo(void)
{
	CloseVideo();
}

void	CDXHUIVideo::OnReset()
{
	if (m_pDisplay) m_pDisplay->RestoreDeviceObjects();
}
void	CDXHUIVideo::OnLost()
{
	if (m_pDisplay) m_pDisplay->InvalidateDeviceObjects();
}
bool	CDXHUIVideo::Create( IDXHUIDev* pDev )
{
	if (!pDev) return false;
	
	this->m_pDXDev	=	reinterpret_cast<IDirect3DDevice9*>(pDev->GetDXDevice());
	this->m_pDev	=	pDev;
	this->m_pShader	=	NULL;

	if ( !CheckIsPSOK() )
	{
		DXHUI_LogErr( TEXT("Display not support PixelShader!") );
		return false;
	}
	return true;
}

bool	CDXHUIVideo::CheckIsPSOK()
{
	if ( m_pDXDev )
	{
		static	bool bCheck = false;
		if (bCheck) return true;

		D3DCAPS9	cap;
		if (SUCCEEDED(m_pDXDev->GetDeviceCaps(&cap)))
		{
			if (cap.PixelShaderVersion < D3DPS_VERSION(2, 0))
				return false;

			bCheck = true;
			return true;
		}
	}
	return false;
}

bool	CDXHUIVideo::OpenVideo( int w, int h, E_CLR_MODE e)
{
	SAFE_DELETE(m_pDisplay);
	if (m_pDXDev)
	{
		tstring strPath;
		if (!m_pDev->FindResourcePath( TEXT("HLSL\\PS_TestOne.ps"), &strPath))
			return false;

		string ps = D3DXGetPixelShaderProfile( m_pDXDev );
		ps = "ps_3_0";
		LPD3DXBUFFER	ppShaderBuff = NULL;
		LPD3DXBUFFER	ppErr		 = NULL;
		LPD3DXEFFECTCOMPILER pEffectCompiler = NULL;
		if ( FAILED(D3DXCreateEffectCompilerFromFile( strPath.c_str(), NULL, NULL, 0, &pEffectCompiler, &ppErr)))
		{
 			const char* pMsg = (char*)ppErr->GetBufferPointer();
			DXHUI_LogErr( TEXT("Failed complite ps file!") );
			return false;
		}
		
		m_pConstable->SetDefaults(m_pDXDev);
		return true;
	}
	//for( int i=0; i<sizeof(Entries)/sizeof(Entries[0]); i++)
	//{
	//	if ( e == Entries[i].cs )
	//	{
	//		m_pDisplay = Entries[i].pCallback( this->m_pDXDev, (CPixelShader**)&m_pPS);
	//		if ( m_pDisplay )
	//		{
	//			RECT	r = {0,0,100,100};
	//			m_pDisplay->SetImageBuffer( w, h, (ID3DVRInterface::COLOR_SPACE)e, &r );
	//			return true;
	//		}
	//		break;
	//	}
	//}
	return false;
}

bool	CDXHUIVideo::CloseVideo()
{
	SAFE_DELETE(m_pDisplay);
	for ( int i=0; i<SHADER_COUNT; i++)
	{
		SAFE_DELETE(m_pPS[i]);
	}
	return true;
}

bool	CDXHUIVideo::Reader(float f)
{
	if ( m_pDisplay )
		m_pDisplay->DrawImage( 1.f, ID3DVRInterface::GS_NONE, 0,0);
	return false;
}

bool	CDXHUIVideo::Free()
{
	delete this;
	return true;
}

bool	CDXHUIVideo::InputImageBuffer(void*	pbBuff, size_t len)	 
{
	if ( m_pDisplay )
	{
		m_pDisplay->UpdateImage( (BYTE*)pbBuff );
	}
	return true;
}

EXPORT_DXHUI_CLASS( CDXHUIVideo, DXHUIVideo, IDXHUIVideo )