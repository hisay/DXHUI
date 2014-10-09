// DXHUILibCore.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../DXHUI/DXHUILib.h"



struct	AutoLock
{
	AutoLock(LPCRITICAL_SECTION lp)
	{
		if(m_lp = lp)EnterCriticalSection(lp);
	}
	~AutoLock()
	{
		if(m_lp) ::LeaveCriticalSection(m_lp);
	}
	LPCRITICAL_SECTION	m_lp;
};
class	CLibCore	: public virtual ILibCore
{
public:
	CLibCore()
	{
		::InitializeCriticalSection(&s_libCS);
	}
	~CLibCore()
	{
		::DeleteCriticalSection(&s_libCS);
	}
	virtual void	RegisterDXHUIClass(IDXHUI_CLASSDef* ptr) 
	{
		AutoLock	al(&s_libCS);
		for( vector<IDXHUI_CLASSDef*>::iterator i =  m_vDef.begin();
			i !=  m_vDef.end(); i++)
		{
			if ( ptr == (*i) ) return;
		}
		m_vDef.push_back(ptr);
	}
	virtual		IDXHUIBase*	CreateClassObject( LPCTSTR clsName )		
	{
		AutoLock	al(&s_libCS);
		if (!clsName) return NULL;
		for( vector<IDXHUI_CLASSDef*>::iterator i = m_vDef.begin();
			 i != m_vDef.end(); i++)
		{
			IDXHUI_CLASSDef*	pDef = (*i);
			if ( pDef && (pDef->GetClsName().compare(clsName) == 0) )
				return pDef->CreateObject();
		}
		return NULL;
	}
protected:
	vector<IDXHUI_CLASSDef*>	m_vDef;
	CRITICAL_SECTION	s_libCS;
};

CLibCore	s_LibCore;
ILibCore*	WINAPI	GetDXHUILibCore()
{
	return &s_LibCore;
}
