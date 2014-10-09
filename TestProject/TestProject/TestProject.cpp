// TestProject.cpp : 定义应用程序的as入口点。
//

#include "stdafx.h"
#include "TestProject.h"
#include "../DXHUI/BaseControl.h"
#include "../DXHUI/DXHUILib.h"

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
		IDXHUIDialog* pDlg = dynamic_cast<IDXHUIDialog*> ( CreateHUI(TEXT("DXHUIDialog")) );
		if ( pDlg )
		{
			m_pMainDlg = pDlg;
			pDlg->InitDialog( pMain,  this, pDlg );
			pDlg->SetFont( 0, TEXT("微软雅黑"), 13, FW_EXTRABOLD   );
			pDlg->SetSize(500,400);
			pDlg->SetLocation(0,0);
			pDlg->SetBkGround( 0x00000000 );
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
	virtual void	OnDeviceReset(IDXHUIMainWnd* ,IDXHUIDev*	)		    	{}
 
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
//
//#define MAX_LOADSTRING 100
//
//// 全局变量:
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
//
//// 此代码模块中包含的函数的前向声明:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY _ttWinMain(HINSTANCE hInstance,
//                     HINSTANCE hPrevInstance,
//                     LPTSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
// 	// TODO: 在此放置代码。
//	MSG msg;
//	HACCEL hAccelTable;
//
//	// 初始化全局字符串
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_TESTPROJECT, szWindowClass, MAX_LOADSTRING);
//	MyRegisterClass(hInstance);
//
//	// 执行应用程序初始化:
//	if (!InitInstance (hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPROJECT));
//
//	// 主消息循环:
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	return (int) msg.wParam;
//}
//
//
//
////
////  函数: MyRegisterClass()
////
////  目的: 注册窗口类。
////
////  注释:
////
////    仅当希望
////    此代码与添加到 Windows 95 中的“RegisterClassEx”
////    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
////    这样应用程序就可以获得关联的
////    “格式正确的”小图标。
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTPROJECT));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTPROJECT);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}
//
////
////   函数: InitInstance(HINSTANCE, int)
////
////   目的: 保存实例句柄并创建主窗口
////
////   注释:
////
////        在此函数中，我们在全局变量中保存实例句柄并
////        创建和显示主程序窗口。
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  目的: 处理主窗口的消息。
////
////  WM_COMMAND	- 处理应用程序菜单
////  WM_PAINT	- 绘制主窗口
////  WM_DESTROY	- 发送退出消息并返回
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// 分析菜单选择:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: 在此添加任意绘图代码...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}
