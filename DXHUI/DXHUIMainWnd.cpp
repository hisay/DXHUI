#pragma once
#include "stdafx.h"
#include "BaseControl.h"
#include <ShellAPI.h>
#include "MainWndRes.h"
#include "DXUT/DXUTGui.h"
#include "DXUT/DXUTmisc.h"

class	CDXDev : public IDXHUIDev
{
public:
	CDXDev();
	~CDXDev();
	IDXHUIDev*	GetDev()
	{
		return this;
	}
	virtual UINT GetSDKVer();
	virtual bool Create()	;
	virtual	bool Destroy()	;
	virtual bool Release()	;
	virtual void DecRef()
	{
		if (m_nRefCount) m_nRefCount--;
	}
	virtual void AddRef()	
	{
		m_nRefCount++;
	}
	virtual void SetWinInfo(int w,int h, HWND hw)
	{
		m_nWidth = w;
		m_nHeight = h;
		m_hWnd = hw;
	}
	virtual IDirect3DDevice9 * GetDXDev();
	virtual void*	GetDXDevice()
	{
		return GetDXDev();
	}

	void	ResetDevice( IDirect3DDevice9  *pd3dDevice)
	{
		m_pDev9 = pd3dDevice;
	}

	virtual	bool FindResourcePath(LPCTSTR szPath, tstring* pstr);
protected:
	UINT	m_nRefCount;
	//IDirect3D9 * m_pD39 ;
	int		m_nWidth;
	int		m_nHeight;
	HWND	m_hWnd;
	IDirect3DDevice9  * m_pDev9;
};

class	CDXFont	: public IDXHUI_CLASS<CDXFont>, public IDXHUIFont
{
public:
	CDXFont()
	{
		m_w = 15;
		m_h = 15;
		m_szName = TEXT("Arial");
		m_bBold = FW_BOLD;
		m_bI	= FALSE;
		m_pFont	= NULL;
	}

	virtual ~CDXFont()
	{
		SAFE_RELEASE(m_pFont);
	}
	virtual void SetSize(int w, int h)
	{
		m_w = w;
		m_h = h;
	}
	virtual	void SetName(LPCTSTR szName)
	{
		m_szName = szName;
	}
	virtual LPCTSTR GetName() const
	{
		return m_szName.c_str();
	}
	virtual	void SetBold( INT n)
	{
		m_bBold	= n;
	}
	virtual INT	 GetBold()
	{
		return m_bBold;
	}
	virtual bool Create(IDXHUIDev* p)
	{
		if ( p && !m_pFont)
		{
			IDirect3DDevice9 * pDev = (IDirect3DDevice9 * )p->GetDXDevice();
			HRESULT	hr = NULL;
			if (FAILED(D3DXCreateFont( (IDirect3DDevice9 *)p->GetDXDevice(), m_h, m_w, m_bBold ? m_bBold  : FW_NORMAL, 0, m_bI, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
				m_szName.c_str(), &m_pFont)))
			 
			{
				return false;
			}

			return true;
		}
		return false;
	}
	virtual void OnReset(IDXHUIDev* p)
	{
	}
	virtual void OnLost(IDXHUIDev* p)
	{
	}
	virtual void DeleteMe()
	{
		this->Release();
	}
protected:
	LPD3DXFONT	m_pFont;
	int			m_w,m_h;
	int			m_bBold;
	int			m_bI;
	tstring		m_szName;
};

CDXDev::CDXDev()
{
	m_nRefCount = 0;
	//m_pD39 	= NULL;
	m_nWidth	= 800;
	m_nHeight	= 600;
	m_hWnd		= NULL;
	m_pDev9		= NULL;
	AddRef();
}
CDXDev::~CDXDev()
{
	if ( m_nRefCount)
	{
		DXHUI_LogErr(TEXT("RefCount NOT IS 0 when delete CDXDev"));
	}
	this->Destroy();
}

bool CDXDev::FindResourcePath(LPCTSTR szPath, tstring* pstr)
{
	// Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strDesPath[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
		strLastSlash++;
        *strLastSlash = TEXT('\0');
	}

	wcscpy_s( strDesPath, MAX_PATH,  strExePath );
	wcscat_s( strDesPath, MAX_PATH,	 TEXT("Resource\\"));
	wcscat_s( strDesPath, MAX_PATH,	 szPath);

	if ( GetFileAttributes( strDesPath ) != 0xFFFFFFFF )
	{
		if ( pstr ) *pstr = strDesPath;
		return true;
	}
	return false;
}


IDirect3DDevice9 * CDXDev::GetDXDev()
{
	return m_pDev9;
}
UINT CDXDev::GetSDKVer()
{
	return D3D_SDK_VERSION;
}

bool CDXDev::Create()
{
	//only create once.
	static	bool	sBCreate = false;
	if (sBCreate)	return false;

	if ( m_pDev9) return true;

	HRESULT	hr;
	hr =	DXUTCreateDevice();
	if ( FAILED(hr) ) return false;
	m_pDev9 = DXUTGetD3D9Device();
	
	sBCreate = true;
	//if (!m_pD39  && !(m_pD39 = Direct3DCreate9 ( GetSDKVer())))
	//{
	//	DXHUI_LogErr(_T("Create D3D9 Failed!"));
	//	return false;
	//}

	//D3DCAPS9	caps;
	//if ( FAILED(m_pD39 ->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	//{
	//	return false;
	//}
	//
	//UINT vp	= (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? D3DCREATE_HARDWARE_VERTEXPROCESSING : 
	//	D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//D3DPRESENT_PARAMETERS	present;
	//memset(&present, 0, sizeof(present));

	//////present.AutoDepthStencilFormat	= D3DFMT_D24X8;
	////present.BackBufferCount			= 1;
	//////present.BackBufferFormat		= D3DFMT_X8R8G8B8;
	////present.BackBufferHeight		= m_nHeight;
	////present.BackBufferWidth			= m_nWidth;
	//present.hDeviceWindow			= m_hWnd;
	//////present.Flags					= 2;
	////present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	////present.MultiSampleQuality		= 0;
	////present.MultiSampleType			= D3DMULTISAMPLE_NONE;
	////present.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_DEFAULT;//D3DPRESENT_INTERVAL_IMMEDIATE;
	////

	////present.BackBufferHeight		=	1	;
	////present.BackBufferWidth			=	1	;
	//present.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	//present.Windowed				= TRUE;

	////D3DDISPLAYMODEEX		pm;
	////pm.Format						= D3DFMT_A8R8G8B8;
	////pm.Height						= m_nHeight;
	////pm.Width						= m_nWidth;
	////pm.ScanLineOrdering				= D3DSCANLINEORDERING_UNKNOWN;
	////pm.RefreshRate					= 0;
	////pm.Size							= 0;
	//if ( !m_pDev9 && FAILED(hr=this->m_pD39 ->CreateDevice  ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
	//	vp | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE ,
	//	&present,  &m_pDev9)))
	//{
	//	DXHUI_LogErr(_T("CreateDevice failed!")<<hr);
	//	return false;
	//}
	return true;
}
bool CDXDev::Destroy()
{
	//::DXUTDestroyState();
	m_pDev9 = NULL;
	return true;
}
bool CDXDev::Release()
{
	DecRef();
	if (m_nRefCount<=0)
		delete this;
	return true;
}
class	CMainWnd	: public virtual IDXHUI_CLASS<CMainWnd>, public virtual IDXHUIMainWnd,
	public IMainWndRes
{
public:
	CMainWnd();
	~CMainWnd();
	virtual	bool CreateWnd( LPCTSTR strTitle, int nWidth, int nHeight );
	virtual IDXHUIDev*	GetHUIDev()								;
	virtual void DoMainLoop	()											;
	virtual void SetEvtInterface( IMainWndEvt*)							 ;
	IMainWndEvt* GetEvtInterface()								 ;
	virtual void OnRender(double fT, float fE)					;
	virtual bool SetParameter( UINT v, DWORD dwV )				;
	virtual IDXHUIFont*	GetNewFont( LPCTSTR szName, int w, int h, INT bBold);
	virtual	 CDXUTDialogResourceManager*	GetDialogRM()	
	{
		return &this->m_dlgRM;
	}
	
	bool	ToggleFullScreen()									
	{
		return !FAILED(DXUTToggleFullScreen());
	}
	static	LRESULT	CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM)	 ;
	static  LRESULT CALLBACK DXWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
	static	LRESULT	CALLBACK DXKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
	{
		return ((CMainWnd*)pUserContext)->OnKeyProc( nChar, bKeyDown, bAltDown );
		return S_OK;
	}
	static	LRESULT	CALLBACK DXMouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
	{
		return ((CMainWnd*)pUserContext)->OnMouseProc( bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down, bSideButton2Down, nMouseWheelDelta, xPos, yPos );
		return S_OK;
	}
	static	void   CALLBACK DXTimer( UINT idEvent, void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnTimer(	idEvent );
	}
	static	bool	CALLBACK DXDevRemoved( void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnDeviceRemove();
		return true;
	}
	static	bool	CALLBACK DXDevSetting( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnDeviceSetting( pDeviceSettings);
		return true;
	}
	 
	static	void   CALLBACK DX9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnRender(  fTime, fElapsedTime );
	}
	static	LRESULT CALLBACK DX9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext  )
	{
		((CMainWnd*)pUserContext)->OnCreateDevice9( pd3dDevice ,pBackBufferSurfaceDesc);
		return S_OK;
	}
	static  HRESULT CALLBACK DX9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnResetDevice9( pd3dDevice ,pBackBufferSurfaceDesc);
		return S_OK;
	}
	static	void  CALLBACK DX9DeviceLost( void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnLostDevice9(   );
	}
	static	void  CALLBACK DX9DeviceDestroy( void* pUserContext )	//*LPDXUTCALLBACKD3D9DEVICEDESTROYED)( void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnDeviceDestroy();
	}
	static		void    CALLBACK DxFrameMove( double fTime, float fElapsedTime, void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnFrameMove( fTime, fElapsedTime );
	}
	static	void CALLBACK  DXGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl,
                                                 void* pUserContext )
	{
		((CMainWnd*)pUserContext)->OnGUIEvent( nEvent, nControlID, pControl );
	}
protected:
	bool			CreateD3DDev()								;
	void			OnCreateDevice9(IDirect3DDevice9* pd3dDevice,const D3DSURFACE_DESC* pDesc);
	void			OnResetDevice9(IDirect3DDevice9* pd3dDevice,const D3DSURFACE_DESC* pDesc);
	void			OnLostDevice9();
	void			OnDeviceDestroy();
	void			OnTimer(UINT);
	LRESULT			OnMouseProc(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos)
	{
		if ( this->m_pWndEvt )
			m_pWndEvt->OnMouseProc( bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down, bSideButton2Down, nMouseWheelDelta, xPos, yPos );
		return S_OK;
	}
	LRESULT			OnKeyProc( UINT nChar, bool bKeyDown, bool bAltDown )
	{
		if ( this->m_pWndEvt )
			m_pWndEvt->OnKeyProc( nChar, bKeyDown, bAltDown );
		return S_OK;
	}
	void			OnFrameMove(double fTime, float fElapsedTime)
	{
		if ( this->m_pWndEvt )
			m_pWndEvt->OnFrameMove(this, m_pDev);
	}
	void			OnDeviceRemove()
	{
		if (this->m_pWndEvt)
		{
			m_pWndEvt->OnDeviceRemoved(this, m_pDev);
		}
	}
	void			OnDeviceSetting( DXUTDeviceSettings* pDeviceSettings)
	{
	}
	void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl)
	{

	}
	IDXHUIFont*	CreateFont( LPCTSTR szName, int w, int h, bool bBold) 
	{
		if ( m_pDev )
		{
			HRESULT hr	= 0;
			LPD3DXFONT 	g_pFont = NULL;
			//((IDirect3DDevice9 *) m_pDev->GetDXDevice())->GetDirect3D;
			if (FAILED(D3DXCreateFont( (IDirect3DDevice9 *) m_pDev->GetDXDevice(), 15, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
				L"Arial", &g_pFont ) ))
			{
				return NULL;
			}
		}
		return NULL;
	}
protected:
	HWND			m_hWnd;
	Evt::Delegate<IDXHUIDev*, IDXHUIMainWnd*>	m_evtOnCreate,
	m_evtOnReset,m_evtOnLost, m_evtOnRender;
	HINSTANCE		m_hInstance;
	IMainWndEvt*	m_pWndEvt;
	IDXHUIDev*		m_pDev;
	DWORD			m_dwBkColor;

	CDXUTDialogResourceManager	m_dlgRM;
 
};

#define	MAINEVTCALL(ev,...)	if(m_pWndEvt) m_pWndEvt-> ev(__VA_ARGS__);

DXHUIDataMap<HWND, CMainWnd>	s_MapWnd;

IMainWndEvt* CMainWnd::GetEvtInterface()
{
	return m_pWndEvt;
}

void CMainWnd::SetEvtInterface( IMainWndEvt* p)	
{
	m_pWndEvt = p;
}

CMainWnd::CMainWnd()
{
	m_hInstance	= NULL;
	m_hWnd		= NULL;
	m_pWndEvt	= NULL;
	m_pDev		= NULL;
	m_dwBkColor	= 0;
}
CMainWnd::~CMainWnd()
{
	DXUTShutdown();
	if (m_pDev)
	{
		m_pDev->Release();
		m_pDev = NULL;
	}

	if (m_hWnd)
	{
		s_MapWnd.Del(m_hWnd);
		m_hWnd = NULL;
	}
	//::DXUTSetCallbackMsgProc( NULL , this );
	::DXUTSetCallbackD3D9FrameRender( NULL, this );
	::DXUTSetCallbackD3D9DeviceCreated( NULL, this );
	::DXUTSetCallbackD3D9DeviceDestroyed( NULL, this );
	::DXUTSetCallbackD3D9DeviceLost( NULL, this );
	::DXUTSetCallbackD3D9DeviceReset( NULL, this );
	::DXUTSetCallbackDeviceRemoved( NULL, this );
	::DXUTSetCallbackFrameMove( NULL, this );
	::DXUTSetCallbackDeviceChanging( NULL, this );
}

LRESULT	CALLBACK CMainWnd::WndProc(HWND h, UINT m,WPARAM w,LPARAM l)
{
	CMainWnd* ptr = s_MapWnd.Find(h);
	if(ptr && ptr->GetEvtInterface() && ptr->GetEvtInterface()->OnWndProc( h,m,w,l))
		return TRUE;
	switch(m)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(h,m,w,l);
}
LRESULT CALLBACK CMainWnd::DXWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	CMainWnd*	pMain = (CMainWnd*) pUserContext ;
	if (pMain && pMain->m_pWndEvt)
	{
		if (pMain->m_pWndEvt->OnWndProc( hWnd, uMsg, wParam, lParam))
		{
			*pbNoFurtherProcessing = true;
			return S_OK;
		}
	}
	return S_OK;
}
bool CMainWnd::CreateWnd( LPCTSTR strTitle, int nWidth, int nHeight )
{
	if (m_hWnd) return true;
	
	HRESULT	hr = 0;
	 
	::DXUTSetCallbackMsgProc( &CMainWnd::DXWndProc , this );
	::DXUTSetCallbackD3D9FrameRender( &CMainWnd::DX9FrameRender, this );
	::DXUTSetCallbackD3D9DeviceCreated( &CMainWnd::DX9DeviceCreated, this );
	::DXUTSetCallbackD3D9DeviceDestroyed( &CMainWnd::DX9DeviceDestroy, this );
	::DXUTSetCallbackD3D9DeviceLost( &CMainWnd::DX9DeviceLost, this );
	::DXUTSetCallbackD3D9DeviceReset( &CMainWnd::DX9DeviceReset, this );
	::DXUTSetCallbackDeviceRemoved( &CMainWnd::DXDevRemoved, this );
	::DXUTSetCallbackFrameMove( &CMainWnd::DxFrameMove, this );
	::DXUTSetCallbackDeviceChanging( &CMainWnd::DXDevSetting, this );

	if (FAILED((hr=DXUTInit(false, true, 0, true))))
	{
		DXTRACE_ERR( _T("Can't init DX!"),hr);
		return false;
	}
	

	//parameter setup
	
	DXUTSetHotkeyHandling( true, false, false );
	//DXUTSetShortcutKeySettings( false, false );

	hr =DXUTCreateWindow( strTitle ) ;
	if (FAILED( hr ))
	{
		DXTRACE_ERR( _T("Can'tCreateWindow!"), hr);
		return false;
	}
 
	if (!CreateD3DDev())
	{
		DXTRACE_ERR( _T("Can't Create DXDevice") , hr );
		return false;
	}
	m_hWnd = DXUTGetHWND();
 
	return true;
}
IDXHUIDev*	CMainWnd::GetHUIDev()	
{
	return m_pDev;
}

bool CMainWnd::CreateD3DDev()
{
	static	CDXDev*	pDev = NULL;
	if (pDev)
	{
		pDev->AddRef();
		m_pDev = pDev;
		return true;
	}else{
		pDev = new CDXDev();
		m_pDev = pDev;
		if( pDev->Create()){
			return true;
		}else{
			m_pDev = NULL;
			pDev->Release();
			pDev = NULL;
		}
	}
	return false;
}
void CMainWnd::DoMainLoop()
{
	 FAILED(DXUTMainLoop());
 
	 return;
 
}

void CMainWnd::OnRender(double fTime, float fEl)
{
	if (!m_pDev) return;
	if ( this->m_pWndEvt && !m_pWndEvt->OnBeforRender(this, m_pDev,fTime, fEl))
		return ;


	HRESULT hr;
	IDirect3DDevice9 * pDev = (IDirect3DDevice9 *)m_pDev->GetDXDevice();
	if(pDev)
	{
		pDev->Clear(  0, NULL, D3DCLEAR_TARGET	| D3DCLEAR_ZBUFFER, this->m_dwBkColor, 1.f,  0);
		V(pDev->BeginScene());

		MAINEVTCALL( OnRender, this, m_pDev, fTime, fEl );
			
		V(pDev->EndScene());

		//hr = pDev->Present (0,0,0,0  );

		//if ( hr == D3DERR_DEVICELOST ||	 hr == D3DERR_DRIVERINTERNALERROR)
		//{
		//	if ( m_pWndEvt ) m_pWndEvt->OnDeviceLost(this);
		//}
	}
	if ( this->m_pWndEvt && !m_pWndEvt->OnEndRender(this, m_pDev, fTime,fEl))
		return ;
}

void CMainWnd::OnCreateDevice9(IDirect3DDevice9* pd3dDevice,const D3DSURFACE_DESC* pDesc)
{
	if ( m_pDev )// !dynamic_cast<CDXDev*>(m_pDev)->GetDXDevice() )
	{
		dynamic_cast<CDXDev*>(m_pDev)->ResetDevice(pd3dDevice);
	}
	HRESULT	hr	= NULL;
	this->m_dlgRM.OnD3D9CreateDevice( pd3dDevice );
	MAINEVTCALL( OnDeviceCreate, this, m_pDev );
 
}

void CMainWnd::OnLostDevice9()
{
	//if ( m_pWndEvt ) m_pWndEvt->OnDeviceLost( this  );
	MAINEVTCALL( OnDeviceLost, this );
	this->m_dlgRM.OnD3D9LostDevice();
}

void CMainWnd::OnResetDevice9(IDirect3DDevice9* pd3dDevice,const D3DSURFACE_DESC* pDesc)
{
	this->m_dlgRM.OnD3D9ResetDevice();
	if ( m_pDev ) dynamic_cast<CDXDev*>(m_pDev)->ResetDevice( pd3dDevice );
	//if ( m_pWndEvt ) m_pWndEvt->OnDevieReset( this, m_pDev );
	MAINEVTCALL( OnDeviceReset, this, m_pDev );
}

void CMainWnd::OnDeviceDestroy()
{
	m_dlgRM.OnD3D9DestroyDevice();
	//if (m_pWndEvt) m_pWndEvt->OnDeviceDestroy(this);
	MAINEVTCALL( OnDeviceDestroy , this );
}

void	CMainWnd::OnTimer(UINT uID)
{
	if ( m_pWndEvt )
		m_pWndEvt->OnTimerCallBack( this, m_pDev, uID );
}

bool CMainWnd::SetParameter( UINT v, DWORD dwV )
{
	switch( v )
	{
	case IDXHUIMainWnd::MWP_BACKGROUND:
		this->m_dwBkColor = dwV;
		break;

	default:
		return false;
	}
	return true;
}
IDXHUIFont*	CMainWnd::GetNewFont(  LPCTSTR szName, int w, int h, INT bBold)
{
	CDXFont	* pFont = new CDXFont();
	if (!pFont)
	{
		DXHUI_LogErr( _T("Memory not enough!") );
		return NULL;
	}
	pFont->SetBold(bBold);
	pFont->SetSize(w,h);
	pFont->SetName(szName);
	if (!pFont->Create( this->m_pDev ))
	{
		delete pFont;
		DXHUI_LogErr( _T("Can't create font!"));
		return NULL;
	}
	
	return dynamic_cast<IDXHUIFont*>(pFont);
}

EXPORT_DXHUI_CLASS( CMainWnd, MainWnd, IDXHUIMainWnd)
EXPORT_DXHUI_CLASS( CDXFont, DXHUIFont, IDXHUIFont)