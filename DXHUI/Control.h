#pragma once
#include "BaseControl.h"
#include "DXUT/DXUTgui.h"

class 	CDXHUIControl : public virtual IDXHUIControl
{
public:
	CDXHUIControl(CDXUTControl*	p)
	{
		m_pControl = p;
	}
	virtual ~CDXHUIControl(){}
	virtual void SetSize(int w, int h){ 
		if(this->m_pControl) m_pControl->SetSize(w,h);
	}
	virtual void SetText(LPCTSTR szText){
	}
	virtual LPCTSTR GetText()
	{
		return TEXT("");
	}
	virtual	DWORD GetBackGround()
	{
		return 0;
	}
	virtual	DWORD GetForeGround()
	{
		if (!m_pControl) return 0;
		return m_pControl->GetTextColor();
	}
	virtual void SetBackGround(DWORD dw)
	{
		
	}
	virtual void SetForeGround(DWORD dw)
	{
		if(m_pControl)m_pControl->SetTextColor(dw);
	}
	virtual void GetSize(int& w,int& h)
	{
		if(m_pControl)
		{
			w = m_pControl->m_width;
			h = m_pControl->m_height;
		}
	}
	virtual	void Enable(bool b)
	{
		if(m_pControl)
			m_pControl->SetEnabled(b);
	}
	virtual bool IsEnable()
	{
		if(m_pControl)
			return m_pControl->GetEnabled();
		return false;
	}
	virtual IDXHUIFont* GetFont()		
	{
		return NULL;
	}
	virtual void 	SetFont(IDXHUIFont * pFont){  ;	}
	virtual	void*	GetDXControl()										
	{
		return m_pControl;
	}
	virtual	void	SetLocaltion(int x, int y )
	{
		if (m_pControl)
			m_pControl->SetLocation(x,y);
	}
	virtual void  OnRender(IDXHUIDev *,float)
	{
	}
	

protected:
	CDXUTControl*	m_pControl;
};


class CDialog	: public IDXHUI_CLASS<CDialog>, public IDXHUIDialog
{
public:
	virtual	~CDialog();
	CDialog();
	virtual	bool	InitDialog( IDXHUIMainWnd*, IGUIEvent*, void* pUD) ;
 
	virtual bool	AddControl(IDXHUIControl* pControl				);
	virtual IDXHUIControl* GetControl(UINT uID );
	virtual IDXHUIControl* GetControlByPos(int x, int y );
	virtual	void	OnRender(IDXHUIDev*,float fEl)	;

	virtual void	SetCaption(LPCTSTR strc)
	{
		m_strCaption = strc;
	}
	virtual LPCTSTR	GetCaption()
	{
		return m_strCaption.c_str();
	}
	virtual bool 	IsShowCaption()
	{
		return m_strCaption.size() > 0;
	}
	virtual void	ShowCaption(bool b)
	{
		if ( b )
		{
			m_dlg.SetCaptionText( m_strCaption.c_str() );
		}else{
			m_dlg.SetCaptionText( TEXT("") );
		}
	}
	virtual bool 	IsVisible()	
	{
		return m_dlg.GetVisible();
	}
	virtual void 	SetVisible(bool b)
	{
		m_dlg.SetVisible(b);
	}
	virtual	void	SetLocation( int x, int y )							 ;
	virtual	void	GetLocation( int&x, int&y )							 ;
	virtual	void	SetSize( int w, int h )								 ;
	virtual void	GetSize( int&w, int&h )								 ;
	virtual	void	SetBkGround( DWORD dw )								 ;
	virtual	void	SetBkGround( DWORD lt, DWORD rt, DWORD lb, DWORD rb ) ;
	virtual	void	SetFont( UINT idx,	LPCTSTR szName, int height, int weight ) ;	

	virtual	IDXHUIStaticText*	AddStatic(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual	IDXHUITextBox*		AddTextBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual IDXHUICheckBox*		AddCheckBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual IDXHUIRadioButton*	AddRadioButton(UINT nID,UINT nGroup, LPCTSTR szText, int x, int y ,int w, int h);	
	virtual	IDXHUIButton*		AddButton(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual	IDXHUISlider*		AddSlider(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual	IDXHUIListBox*		AddListBox(UINT nID, LPCTSTR szText, int x, int y ,int w, int h);
	virtual	IDXHUIComboBox*		AddComboBox(UINT nID,  int x, int y ,int w, int h);

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
 
	static VOID  CALLBACK fnCALLBACKDXUTGUIEVENT ( UINT nEvent, int nControlID, CDXUTControl* pControl,
                                                 void* pUserContext );
protected:
	CDXUTDialog					m_dlg;
	vector<IDXHUIControl*>		m_vControl;
	tstring						m_strCaption;
	IGUIEvent*					m_pGUIEvt;
	void*						m_pGUIEvtD;
	typedef vector<IDXHUIControl*>::iterator	vControlI;
};
class CTextBox	: public virtual CDXHUIControl,
	public virtual IDXHUITextBox
{
public:
	CTextBox(CDXUTEditBox*);
	virtual ~CTextBox();
	virtual void OnRender( IDXHUIDev* p,float fEl);
	virtual void* GetDXControl()
	{
		return m_pEditBox;
	}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}

protected:
	CDXUTEditBox*	m_pEditBox;

};

class CStatic : public virtual IDXHUIStaticText,
	public virtual CDXHUIControl
{
public:
	CStatic(CDXUTStatic* p):CDXHUIControl(p){m_pStatic = p;}
	virtual ~CStatic(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
protected:
	CDXUTStatic*	m_pStatic;
};

class CComboBox : public virtual CDXHUIControl,
	public virtual IDXHUIComboBox
{
public:
	CComboBox(CDXUTComboBox* p) : CDXHUIControl(p){m_pCombox = p;}
	virtual ~CComboBox(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
	virtual		bool	AddItem(LPCTSTR szText, void* pData)	
	{
		if (m_pCombox)
		{
			if ( SUCCEEDED(m_pCombox->AddItem(szText, pData)))
			{
				return true;
			}
		}
		return false;
	};		//return index.
	virtual		void RemoveItem(UINT iNdex)					 
	{
		if ( m_pCombox )
			m_pCombox->RemoveItem(iNdex);
	}
	virtual		UINT	GetCount()		
	{
		if ( m_pCombox )
			return m_pCombox->GetNumItems();
		return 0;
	}
	virtual		void	SetText( LPCTSTR	szText )
	{
		if ( m_pCombox )
		{
			m_pCombox->SetSelectedByText( szText );
 
		}
	}
protected:
	CDXUTComboBox*	m_pCombox;
};


class CListBox : public virtual CDXHUIControl,
	public virtual IDXHUIListBox
{
public:
	CListBox(CDXUTListBox* p) : CDXHUIControl(p){m_pListbox = p;}
	virtual ~CListBox(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
	virtual		bool	AddItem(LPCTSTR szText, void* pData)
	{
		if ( m_pListbox )
		{
			if (SUCCEEDED(m_pListbox->AddItem( szText, pData)))
			{
				return true;
			}
		}
		return false;
	}
	virtual		void	RemoveItem(UINT iNdex)					
	{
		if ( m_pListbox )
		{
			m_pListbox->RemoveItem(iNdex);
		}
	}
	virtual		UINT	GetCount()							
	{
		if ( m_pListbox )
			return m_pListbox->GetSize();
		return 0;
	}
	virtual		void	SetText( LPCTSTR	szText )
	{
		if ( m_pListbox )
		{
			for( int i=0; i<m_pListbox->GetSize(); i++)
			{
				if ( 0 == _tcscmp ( m_pListbox->GetItem(i)->strText, szText ))
				{
					m_pListbox->SelectItem(i);
					break;
				}
			}
		}
	}
protected:
	CDXUTListBox*	m_pListbox;
};

class CCheckBox : public virtual CDXHUIControl,
	public virtual IDXHUICheckBox
{
public:
	CCheckBox(CDXUTCheckBox*p)  : CDXHUIControl(p){m_pCheckbox = p;}
	virtual ~CCheckBox(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
protected:
	CDXUTCheckBox*	m_pCheckbox;
};

class CSlider : public virtual CDXHUIControl,
	public virtual IDXHUISlider
{
public:
	CSlider ( CDXUTSlider* p)  : CDXHUIControl(p){m_pSlider = p;}
	virtual ~CSlider(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
protected:
	CDXUTSlider*	m_pSlider;
};

class CButton : public virtual CDXHUIControl
	,public virtual IDXHUIButton
{
public:
	CButton ( CDXUTButton* p)  : CDXHUIControl(p){m_pButton = p;}
	virtual ~CButton(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
protected:
	CDXUTButton*	m_pButton;
};

class CRadioButton : public virtual CDXHUIControl,
	public virtual IDXHUIRadioButton
{
public:
	CRadioButton( CDXUTRadioButton* p)  : CDXHUIControl(p){m_pRadioButton = p;}
	virtual ~CRadioButton(){;}
	virtual	IDXHUIControl*	GetBaseControl()
	{
		return this;
	}
protected:
	CDXUTRadioButton*	m_pRadioButton;
};