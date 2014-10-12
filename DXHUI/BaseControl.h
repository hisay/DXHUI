#pragma once
#ifndef _DXHUIBASECONTROL_H
#define _DXHUIBASECONTROL_H

#include "incDXHUI.h"

 
class	IDXHUIMainWnd;
class	IDXHUIDev;
class	IDXHUIFont;

struct	IMainWndEvt
{
	virtual ~IMainWndEvt(){}
	virtual	void	OnDeviceCreate(IDXHUIMainWnd* ,IDXHUIDev*	)			= 0;
	virtual void	OnDeviceLost(IDXHUIMainWnd* )							= 0;
	virtual void	OnDeviceReset(IDXHUIMainWnd* ,IDXHUIDev*	)			= 0;
	virtual	void	OnDeviceDestroy(IDXHUIMainWnd*)							= 0;
	virtual	void	OnDeviceChange(IDXHUIMainWnd*, IDXHUIDev* )				= 0;
	virtual void	OnDeviceRemoved(IDXHUIMainWnd*, IDXHUIDev* )			= 0;
	
	virtual void	OnFrameMove(IDXHUIMainWnd* ,IDXHUIDev*			)		= 0;
	
	virtual	bool	OnWndProc( HWND hHwnd, UINT nMsg, WPARAM, LPARAM)		= 0;
	virtual bool	OnKeyProc(  UINT nChar, bool bKeyDown, bool bAltDown  )	= 0;									
	virtual bool	OnMouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, 
		bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos )			= 0;
	virtual	void	OnTimerCallBack(IDXHUIMainWnd* , IDXHUIDev*, UINT uID)	= 0;

	virtual	void	OnRender( IDXHUIMainWnd* , IDXHUIDev*,double fTime,float)= 0;
	virtual bool	OnBeforRender(IDXHUIMainWnd* , IDXHUIDev*,double fTime,float)	= 0;
	virtual bool	OnEndRender(IDXHUIMainWnd* , IDXHUIDev*,double fTime,float)	= 0;
};
class	IDXHUIControl;
struct	IGUIEvent
{
	virtual bool OnGUIEvent ( UINT nEvnt, UINT nID, IDXHUIControl* pControl, void* )	= 0;
};
class	IDXHUIMainWnd
{
public:
	enum E_ATTR
	{
		MWP_BACKGROUND	=	0
	};
	virtual	~IDXHUIMainWnd()	
	{
	}
	virtual	bool CreateWnd( LPCTSTR strTitle, int nWidth, int nHeight ) = 0;
	virtual IDXHUIDev*	GetHUIDev()										= 0;
	virtual void DoMainLoop	()											= 0;
	virtual	void SetEvtInterface( IMainWndEvt*)							= 0;
	virtual IMainWndEvt* GetEvtInterface()								= 0;
	virtual void OnRender(double fT, float fE)							= 0;
	virtual bool	SetParameter( UINT	,	DWORD dwValue )				= 0;
	virtual bool	ToggleFullScreen()									= 0;
	virtual IDXHUIFont*	GetNewFont(	LPCTSTR szName, int w, int h, INT bBold) = 0;
};

DXHUI_INTERFACE	IDXHUIFont
{
public:
	virtual	~IDXHUIFont(){}
	virtual void	SetSize(int width, int height)						= 0;
	virtual void	SetName(LPCTSTR szFontName)							= 0;
	virtual	LPCTSTR	GetName() const										= 0;
	virtual void	SetBold(INT b )										= 0;
	virtual INT		GetBold( )											= 0;
	virtual	bool	Create( IDXHUIDev*	)								= 0;
	virtual	void	OnReset( IDXHUIDev* )								= 0;
	virtual void	OnLost( IDXHUIDev* )								= 0;
	virtual	void	DeleteMe()											= 0;
};
DXHUI_INTERFACE	IDXHUIControl
{
public:
	virtual ~IDXHUIControl(){}
	virtual	void	SetSize( int w, int h)								= 0;
	virtual void	SetLocaltion(int x,int y)							= 0;
	virtual	void	SetForeGround( DWORD	)							= 0;
	virtual DWORD	GetForeGround( )									= 0;
	virtual DWORD	GetBackGround( )									= 0;
	virtual	void	SetBackGround( DWORD )								= 0;
	virtual IDXHUIFont* GetFont()										= 0;
	virtual void 	SetFont(IDXHUIFont *)								= 0;
	virtual	void SetText( LPCTSTR	szText )							= 0;
	virtual	LPCTSTR GetText ( )											= 0;	
	virtual bool 	IsEnable()											= 0;
	virtual void 	Enable(bool)										= 0;
	virtual	void	OnRender(IDXHUIDev*,float fEl)						= 0;
	virtual	void*	GetDXControl()										= 0;
	
};

DXHUI_INTERFACE	IDXHUIControlBase
{
public:
	virtual	IDXHUIControl*	GetBaseControl()							= 0;
};
class	IDXHUIStaticText : public  IDXHUIControlBase
{
public:
	virtual	~IDXHUIStaticText(){}

	
};

class 	IDXHUITextBox : public   IDXHUIControlBase
{
public:
	virtual ~IDXHUITextBox(){}
};

class 	IDXHUIButton	: public    IDXHUIControlBase
{
public:
	virtual 	~IDXHUIButton(){}
};

class	IDXHUIComboBox	: public    IDXHUIControlBase
{
public:
	virtual		~IDXHUIComboBox(){}
	virtual		bool	AddItem(LPCTSTR szText, void* pData)	= 0; 
	virtual		void	RemoveItem(UINT iNdex)					= 0;
	virtual		UINT	GetCount()								= 0;
};

class	IDXHUIListBox	: public  IDXHUIControlBase
{
public:
	virtual		~IDXHUIListBox(){}
	virtual		bool	AddItem(LPCTSTR szText, void* pData)	= 0; 
	virtual		void	RemoveItem(UINT iNdex)					= 0;
	virtual		UINT	GetCount()								= 0;
};
class	IDXHUIRadioButton : public IDXHUIControlBase
{
public:
	virtual	~IDXHUIRadioButton(){}
};

class	IDXHUICheckBox	: public  IDXHUIControlBase
{
public:
	virtual		~IDXHUICheckBox(){}
};

class	IDXHUISlider : public IDXHUIControlBase
{
public:
	virtual	~IDXHUISlider(){}
};
DXHUI_INTERFACE	IDXHUIDialog
{
public:
//	typedef	bool (*fnGUIEvent)( UINT nEvnt, UINT nID, IDXHUIControl* pControl, void* pUserData);
	virtual	~IDXHUIDialog(){}
	virtual	bool	InitDialog( IDXHUIMainWnd*, IGUIEvent* pEvt, void* pUserD)		= 0;
	virtual bool	AddControl(IDXHUIControl* pControl				)	= 0;
	virtual IDXHUIControl* GetControl(UINT uID )						= 0;
	virtual IDXHUIControl* GetControlByPos(int x, int y )				= 0;
	virtual void	SetCaption(LPCTSTR )								= 0;
	virtual LPCTSTR	GetCaption()										= 0;
	virtual bool 	IsShowCaption()										= 0;
	virtual void	ShowCaption(bool )									= 0;
	virtual bool 	IsVisible()											= 0;
	virtual void 	SetVisible(bool )									= 0;
	virtual	void	OnRender(IDXHUIDev*,float fEl)						= 0;
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;
	virtual	void	SetLocation( int x, int y )							= 0;
	virtual	void	GetLocation( int&x, int&y )							= 0;
	virtual	void	SetSize( int w, int h )								= 0;
	virtual void	GetSize( int&w, int&h )								= 0;
	virtual	void	SetBkGround( DWORD dw )								= 0;
	virtual	void	SetBkGround( DWORD lt, DWORD rt, DWORD lb, DWORD rb ) = 0;
	virtual	void	SetFont( UINT idx,	LPCTSTR szName, int height, int weight ) = 0;

	virtual	IDXHUIStaticText*	AddStatic(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual	IDXHUITextBox*		AddTextBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual IDXHUICheckBox*		AddCheckBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual IDXHUIRadioButton*	AddRadioButton(UINT nID,UINT nGroup, LPCTSTR szText, int x, int y ,int w, int h) = 0;	
	virtual	IDXHUIButton*		AddButton(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual	IDXHUISlider*		AddSlider(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual	IDXHUIListBox*		AddListBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h) = 0;
	virtual	IDXHUIComboBox*		AddComboBox(UINT nID,   int x, int y ,int w, int h) = 0;
};

#endif