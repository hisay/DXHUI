#include "StdAfx.h"
#include "Control.h"
#include "MainWndRes.h"

CTextBox::CTextBox(CDXUTEditBox* p):m_pEditBox(p),CDXHUIControl(p)
{
}
CTextBox::~CTextBox()
{
}
void CTextBox::OnRender( IDXHUIDev* p , float fEl)
{
	if(m_pEditBox)
		m_pEditBox->Render(fEl);
}

 

VOID  CALLBACK CDialog::fnCALLBACKDXUTGUIEVENT ( UINT nEvent, int nControlID, CDXUTControl* pControl,
                                                 void* pUserContext )
{
	CDialog*	pDlg = ((CDialog*)pUserContext);
	if(pDlg && pDlg->m_pGUIEvt )
	{
		IDXHUIControl* pHC = NULL;
		for( vControlI i = pDlg->m_vControl.begin(); i != pDlg->m_vControl.end(); i++)
		{
			if ( (*i)->GetDXControl() == (void*)pControl )
			{
				pHC	=	(*i);
				break;
			}
		}
		
		pDlg->m_pGUIEvt->OnGUIEvent( nEvent, nControlID, pHC , pDlg->m_pGUIEvtD);
	}
}

CDialog::CDialog()
{
	this->m_pGUIEvt	= NULL;
	this->m_pGUIEvtD= NULL;
}
CDialog::~CDialog()
{

}

 

bool CDialog::InitDialog( IDXHUIMainWnd* pMainWnd, IGUIEvent* cb, void* pUD)
{
	IMainWndRes*	pMain = dynamic_cast<IMainWndRes*>(pMainWnd);
	if ( pMain )
	{
		this->m_pGUIEvt	=	cb;
		this->m_pGUIEvtD		=	pUD;

		m_dlg.Init( pMain->GetDialogRM()  );//, true, TEXT("e:/video/ListTitle.bmp"));// TEXT("G:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Samples/Media/Textures/four_NM_height.DDS"));
		//m_dlg.AddButton( 11, TEXT("BUTTON"), 10,10, 100,30);
		//m_dlg.AddCheckBox( 12, TEXT("CHECKBOX"), 10, 40, 100,30);
		//CDXUTComboBox*	pCombo = NULL;
		//m_dlg.AddComboBox( 13, 10, 70, 100,30 , 0,false, &pCombo);
		//pCombo->AddItem(TEXT("one"),0);
		//pCombo->AddItem(TEXT("one"),0);
		//pCombo->AddItem(TEXT("one"),0);
		//pCombo->AddItem(TEXT("one"),0);
		//pCombo->AddItem(TEXT("one"),0);
		//m_dlg.AddEditBox( 14, TEXT("EDITBOX"), 10, 110, 100,30);
		//CDXUTListBox*	pListBox = 0;
		//m_dlg.AddListBox( 15, 10,150,100,130, 0, &pListBox );
		//m_dlg.AddRadioButton( 16,1, TEXT("HEHE"), 10,290,100,30);
		//m_dlg.AddRadioButton( 17,1, TEXT("xx"), 10,320,100,30);
		//pListBox->AddItem( TEXT("one"), 0 );
		//pListBox->AddItem( TEXT("two"), 0 );
		//pListBox->AddItem( TEXT("tree"), 0 );
		//m_dlg.AddSlider( 18, 10, 360, 100, 30, 0, 1000, 20);
		//m_dlg.AddStatic( 19, TEXT("STATIC"), 10, 390, 100,30 );
		m_dlg.SetCallback( fnCALLBACKDXUTGUIEVENT, this );
	}
	return true;
}
void	CDialog::SetSize( int w,int h )
{
	m_dlg.SetSize(w,h);
}

void	CDialog::SetLocation( int x, int y )
{
	m_dlg.SetLocation(x,y);
}

void	CDialog::GetLocation( int &x,int & y)
{
	POINT pt;
	m_dlg.GetLocation(pt);
	x = pt.x;
	y = pt.y;
}

void	CDialog::GetSize(int &w, int& h )
{
	w = m_dlg.GetWidth();
	h = m_dlg.GetHeight();
}
void	CDialog::SetBkGround( DWORD dw )
{
	m_dlg.SetBackgroundColors(dw);
}
void	CDialog::SetBkGround( DWORD lt, DWORD rt, DWORD lb, DWORD rb )
{
	m_dlg.SetBackgroundColors(lt,rt,lb,rb);
}
void	CDialog::OnRender(IDXHUIDev* pDev,float fEl)	
{
	m_dlg.OnRender(fEl);
}
bool	CDialog::AddControl(IDXHUIControl* pControl				)
{
	if ( pControl && pControl->GetDXControl() )
	{
		CDXUTControl*	pCtl = reinterpret_cast<CDXUTControl*>(pControl->GetDXControl());
		if ( SUCCEEDED(m_dlg.AddControl(pCtl)))
		{
			m_vControl.push_back( pControl );
			return true;
		}
	}
	return false;
}

bool	CDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	return m_dlg.MsgProc( hWnd, uMsg, wParam, lParam);
}

IDXHUIControl* CDialog::GetControl(UINT uID )
{
	void* pCtrl = m_dlg.GetControl(uID);
	if (!pCtrl) return NULL;

	for( vControlI i = m_vControl.begin(); i != m_vControl.end(); i++)
	{
		if ( (*i)->GetDXControl() == pCtrl )
		{
			return (*i);
		}
	}
	return false;
}
IDXHUIControl* CDialog::GetControlByPos(int x, int y )
{
	POINT pt;pt.x = x; pt.y = y;
	void* pCtrl = m_dlg.GetControlAtPoint( pt );
	if (!pCtrl) return NULL;

	for( vControlI i = m_vControl.begin(); i != m_vControl.end(); i++)
	{
		if ( (*i)->GetDXControl() == pCtrl )
		{
			return (*i);
		}
	}
	return false;
}
IDXHUIStaticText*	CDialog::AddStatic(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTStatic*	pStatic = 0;
	if (SUCCEEDED(m_dlg.AddStatic( nID, szText, w, y , w, h , false, &pStatic)))
	{
		CStatic*	pS = new CStatic(pStatic);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
 
			this->m_vControl.push_back(pS ->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
IDXHUITextBox*		CDialog::AddTextBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTEditBox*	pEdit = 0;
	if (SUCCEEDED(m_dlg.AddEditBox( nID, szText, w, y , w, h , false, &pEdit)))
	{
		CTextBox*	pS = new CTextBox(pEdit);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
IDXHUICheckBox*		CDialog::AddCheckBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTCheckBox*	pChkBox = 0;
	if (SUCCEEDED(m_dlg.AddCheckBox( nID, szText, w, y , w, h , false,0, false, &pChkBox)))
	{
		CCheckBox*	pS = new CCheckBox(pChkBox);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
IDXHUISlider*		CDialog::AddSlider(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTSlider*	pSlider = 0;
	if (SUCCEEDED(m_dlg.AddSlider( nID, w, y , w, h , 0, 100, 50, false, &pSlider)))
	{
		CSlider*	pS = new CSlider(pSlider);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
IDXHUIListBox*		CDialog::AddListBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTListBox*	pLB = 0;
	if (SUCCEEDED(m_dlg.AddListBox( nID, w, y , w, h , 0, &pLB)))
	{
		CListBox*	pS = new CListBox(pLB);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;

}
IDXHUIButton*		CDialog::AddButton(UINT nID, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTButton*	pBtn = 0;
	if (SUCCEEDED(m_dlg.AddButton( nID, szText, w, y , w, h , 0, false, &pBtn)))
	{
		CButton*	pS = new CButton(pBtn);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;

}

void	CDialog::SetFont( UINT idx,	LPCTSTR szName, int height, int weight )
{
	m_dlg.SetFont( idx, szName, height, weight );
}

IDXHUIRadioButton*		CDialog::AddRadioButton(UINT nID, UINT nGroup, LPCTSTR szText, int x, int y ,int w, int h)
{
	CDXUTRadioButton*	pChkBtn = 0;
	if (SUCCEEDED(m_dlg.AddRadioButton( nID, nGroup, szText, w, y , w, h , false, 0, false, &pChkBtn)))
	{
		CRadioButton*	pS = new CRadioButton(pChkBtn);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
IDXHUIComboBox*		CDialog::AddComboBox(UINT nID,  int x, int y ,int w, int h)
{
	CDXUTComboBox*	pCombo = 0;
	if (SUCCEEDED(m_dlg.AddComboBox( nID, w, y , w, h , 0, false, &pCombo)))
	{
		CComboBox*	pS = new CComboBox(pCombo);
		if (!pS) m_dlg.RemoveControl(nID);
		else
		{
			this->m_vControl.push_back(pS->GetBaseControl());
			return pS;
		}
	}
	return NULL;
}
EXPORT_DXHUI_CLASS( CDialog, DXHUIDialog, IDXHUIDialog )