// TestProject.cpp : 定义应用程序的as入口点。
//

#include "stdafx.h"
#include "TestProject.h"
#include "../DXHUI/BaseControl.h"
#include "../DXHUI/DXHUILib.h"
#include "../DXHUILanguage/incLanguage.h"

#include <g_dxHUIInc.h>
#ifdef _DEBUG
#pragma comment(lib, "DXHUId.lib")
#else
#pragma comment(lib, "DXHUI.lib")
#endif

class	CMainWndEvt : public IMainWndEvt, public IGUIEvent
{
public:
	 
	CMainWndEvt( IDXHUIMainWnd* p)
	{
		m_pMainWnd	= p;
		m_pMainDlg  = NULL;
	}
	virtual bool OnGUIEvent ( UINT nEvnt, UINT nID, IDXHUIControl* pControl, void* pDLG )
	{
		return false;
	}
	virtual	void	OnRender( IDXHUIMainWnd* pWnd, IDXHUIDev*	pDev, double fTime, float fEl) 
	{
		if ( m_pMainDlg && m_pMainDlg->IsVisible())
		{
			m_pMainDlg->OnRender(pDev, fEl );
		}
	}
	virtual	void	OnDeviceCreate(IDXHUIMainWnd* pMain,IDXHUIDev*	pDev) 
	{
		IDXHUIFont*	pFont = pMain->GetNewFont( _T("华文行楷"), 40,120, FW_BOLD);
		//pFont->DeleteMe();
		IDXHUIDialog* pDlg = dynamic_cast<IDXHUIDialog*> ( CreateHUI(TEXT("DXHUIDialog")) );
		if ( pDlg )
		{
			m_pMainDlg = pDlg;
			pDlg->InitDialog( pMain,  this, pDlg );
			pDlg->SetFont( 0, TEXT("微软雅黑"), 13, FW_NORMAL   );
			pDlg->SetFont( 1, TEXT("微软雅黑"), 14, FW_BOLD   );
			pDlg->SetSize(500,400);
			pDlg->SetLocation(0,0);
			pDlg->SetBkGround( 0xff00BB00 );
			int	y = 0;int nH = 30;
			pDlg->AddStatic( 100, TEXT("ADDSTATIC"), 0, 0, 80, nH);y += nH;
			pDlg->AddButton( 101, TEXT("BUTTON"), 0, y, 80, nH);y += nH;
			pDlg->AddCheckBox( 102, TEXT("CHECKBOX"), 0, y, 80,nH);y += nH;
			IDXHUIListBox* plbox =	pDlg->AddListBox( 103, TEXT("LISTBOX"), 0, y, 120,nH*4);y += nH*4;
			plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("测试数据"), 0);
			plbox->AddItem( TEXT("大起大落"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("测试数据"), 0);
			plbox->AddItem( TEXT("大起大落"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("测试数据"), 0);
			plbox->AddItem( TEXT("大起大落"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("测试数据"), 0);
			plbox->AddItem( TEXT("大起大落"), 0);
			plbox->GetBaseControl()->SetText( TEXT("twe") );

			pDlg->AddRadioButton( 104, 1, TEXT("RADIO1"), 0, y, 100,nH);y += nH;
			pDlg->AddRadioButton( 105, 1, TEXT("RADIO2"), 0, y, 100,nH);y += nH;
			pDlg->AddSlider( 106, TEXT("SLIDER"), 0, y, 100, nH);y += nH;
			pDlg->AddTextBox( 107, TEXT("EDIT"), 0, y, 100, nH);y += nH;
			IDXHUIComboBox*	pCombo = pDlg->AddComboBox( 108, 0, y, 100, nH);y += nH;
			pCombo->AddItem( TEXT("A"), 0);
			pCombo->AddItem( TEXT("AB"), 0);
			pCombo->AddItem( TEXT("ABC"), 0);
			pCombo->GetBaseControl()->SetText( TEXT("AB"));

			
		}
	}
	virtual	void	OnDeviceDestroy(IDXHUIMainWnd* )
	{
	}
	virtual	void	OnDeviceChange(IDXHUIMainWnd*,IDXHUIDev*	pDev)
	{
	}
	virtual	void	OnDeviceRemoved(IDXHUIMainWnd*,IDXHUIDev*	pDev)
	{
	}
	virtual	void	OnFrameMove(IDXHUIMainWnd *,IDXHUIDev *)
	{
	}
	virtual	bool	OnWndProc( HWND hHwnd, UINT nMsg, WPARAM w, LPARAM l) 
	{
		if ( this->m_pMainDlg && m_pMainDlg->IsVisible() && m_pMainDlg->MsgProc( hHwnd, nMsg, w, l ))
			return true;
		switch( nMsg )
		{
		case WM_KEYDOWN:
			if ( LOBYTE(w) == VK_RETURN && m_pMainWnd)
			{
				m_pMainWnd->ToggleFullScreen();
				return false;
			}
			break;
		}
		return false;
	}
	virtual bool	OnKeyProc(UINT nChar, bool bKeyDown, bool bAltDown )
	{
		return false;
	}
	virtual bool	OnMouseProc(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, 
		bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos)
	{
		return false;
	}
	virtual void	OnTimerCallBack(IDXHUIMainWnd* , IDXHUIDev*,UINT)
	{
	}
	virtual void	OnDeviceLost(IDXHUIMainWnd* )	{
	}
	virtual void	OnDeviceReset(IDXHUIMainWnd* ,IDXHUIDev*	)		    	{
		if ( m_pMainDlg )
			m_pMainDlg->SetLocation( 10,10);
	}
 
	virtual bool	OnBeforRender(IDXHUIMainWnd* , IDXHUIDev*,double fTime, float)
	{
		return true;
	}
	virtual bool	OnEndRender(IDXHUIMainWnd* , IDXHUIDev*,double fTime, float)	
	{
		return true;
	}
	
protected:
	IDXHUIMainWnd*	m_pMainWnd;
	IDXHUIDialog*	m_pMainDlg;
};
 
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	IDXHUILib::GetDXHUILib()->InitLibrary();

	ILanguage*			pLang	 = dynamic_cast<ILanguage*>	(CreateHUI(TEXT("Language")));
	if ( pLang )
	{
		if ( pLang->LoadLanguage(TEXT("CN")) )
		{
			tstring str = pLang->GetCurLang()->GetLang( TEXT("MAIN"), TEXT("ID"), TEXT("UNKNOW"));
			str = pLang->GetCurLang()->GetLang( TEXT("MAIN"), TEXT("IDC"), TEXT("UNKNOW"));
			str = pLang->GetCurLang()->GetLang( TEXT("MAIN"), TEXT("IDE"), TEXT("UNKNOW"));
			str = pLang->GetCurLang()->GetLang( TEXT("MAIN"), TEXT("IDX"), TEXT("UNKNOW"));
		}
	}
	IDXHUIMainWnd*		pMainWnd = dynamic_cast<IDXHUIMainWnd*>(CreateHUI(_T("MainWnd") ));
	if (pMainWnd)
	{
		CMainWndEvt evt(pMainWnd);
		pMainWnd->SetEvtInterface(&evt);
		pMainWnd->CreateWnd( _T("TestProject"), 800, 600);
		pMainWnd->SetParameter( IDXHUIMainWnd::MWP_BACKGROUND, 0xffAA3333 );
		pMainWnd->DoMainLoop();
		delete pMainWnd;
	}

	return TRUE;
}
 