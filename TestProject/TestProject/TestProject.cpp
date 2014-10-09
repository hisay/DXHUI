// TestProject.cpp : ����Ӧ�ó����as��ڵ㡣
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
		IDXHUIFont*	pFont = pMain->GetNewFont( _T("�����п�"), 40,120, FW_BOLD);
		IDXHUIDialog* pDlg = dynamic_cast<IDXHUIDialog*> ( CreateHUI(TEXT("DXHUIDialog")) );
		if ( pDlg )
		{
			m_pMainDlg = pDlg;
			pDlg->InitDialog( pMain,  this, pDlg );
			pDlg->SetFont( 0, TEXT("΢���ź�"), 13, FW_EXTRABOLD   );
			pDlg->SetSize(500,400);
			pDlg->SetLocation(0,0);
			pDlg->SetBkGround( 0x00000000 );
			int	y = 0;int nH = 30;
			pDlg->AddStatic( 100, TEXT("ADDSTATIC"), 0, 0, 80, nH);y += nH;
			pDlg->AddButton( 101, TEXT("BUTTON"), 0, y, 80, nH);y += nH;
			pDlg->AddCheckBox( 102, TEXT("CHECKBOX"), 0, y, 80,nH);y += nH;
			IDXHUIListBox* plbox =	pDlg->AddListBox( 103, TEXT("LISTBOX"), 0, y, 120,nH*4);y += nH*4;
			plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("��������"), 0);
			plbox->AddItem( TEXT("�������"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("��������"), 0);
			plbox->AddItem( TEXT("�������"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("��������"), 0);
			plbox->AddItem( TEXT("�������"), 0);plbox->AddItem( TEXT("one"), 0);
			plbox->AddItem( TEXT("��������"), 0);
			plbox->AddItem( TEXT("�������"), 0);
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
//// ȫ�ֱ���:
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
//
//// �˴���ģ���а����ĺ�����ǰ������:
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
// 	// TODO: �ڴ˷��ô��롣
//	MSG msg;
//	HACCEL hAccelTable;
//
//	// ��ʼ��ȫ���ַ���
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_TESTPROJECT, szWindowClass, MAX_LOADSTRING);
//	MyRegisterClass(hInstance);
//
//	// ִ��Ӧ�ó����ʼ��:
//	if (!InitInstance (hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPROJECT));
//
//	// ����Ϣѭ��:
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
////  ����: MyRegisterClass()
////
////  Ŀ��: ע�ᴰ���ࡣ
////
////  ע��:
////
////    ����ϣ��
////    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
////    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
////    ����Ӧ�ó���Ϳ��Ի�ù�����
////    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
////   ����: InitInstance(HINSTANCE, int)
////
////   Ŀ��: ����ʵ�����������������
////
////   ע��:
////
////        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
////        ��������ʾ�����򴰿ڡ�
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
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
////  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  Ŀ��: ���������ڵ���Ϣ��
////
////  WM_COMMAND	- ����Ӧ�ó���˵�
////  WM_PAINT	- ����������
////  WM_DESTROY	- �����˳���Ϣ������
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
//		// �����˵�ѡ��:
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
//		// TODO: �ڴ���������ͼ����...
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
//// �����ڡ������Ϣ�������
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
