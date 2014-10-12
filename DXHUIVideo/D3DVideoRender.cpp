/*
author: you not't need know!
*/


// D3DVideoRender.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
 
#include "RenderFactory.h"
 
#include "Display_YUV420.h"
#include "Display_YUV422.h"
#include "Display_NV12.h"
#include "Display_RGB24.h"
#include "Display_RGB32.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HMODULE g_hModule = NULL ;
 
CRenderFactory g_RenderFactory ;

 
  void Register() ;

 

#ifdef _MANAGED
#pragma managed(pop)
#endif


ID3DVRInterface* D3D_VIDEO_RENDER::D3D_Video_Render_Create ()
{
 
	return NULL;
}

void D3D_VIDEO_RENDER::D3D_Video_Render_Destroy ()
{
 
}

void Register()
{
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

	for ( int i = 0 ; i < sizeof(Entries) / sizeof(REGISTER) ; i++ )
	{
		g_RenderFactory.RegisterRender ( Entries[i].cs, Entries[i].pCallback ) ;
	}
}