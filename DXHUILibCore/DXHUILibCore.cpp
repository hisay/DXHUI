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
	virtual	bool	InitLibrary();
protected:
	vector<IDXHUI_CLASSDef*>	m_vDef;
	CRITICAL_SECTION	s_libCS;
};

CLibCore	s_LibCore;
ILibCore*	WINAPI	GetDXHUILibCore()
{
	return &s_LibCore;
}

bool	CLibCore::InitLibrary()
{
	TCHAR	szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	::GetModuleFileName( NULL, szPath, MAX_PATH);
	*(_tcsrchr( szPath, TEXT('\\')) + 1) = TEXT('\0');
	tstring	strPath(szPath);
	tstring sstrPath(strPath);
	sstrPath.append( TEXT("*.huilib"));

	WIN32_FIND_DATA	findData;
	HANDLE	hFind = ::FindFirstFile( sstrPath.c_str(), &findData); 
	if ( hFind == INVALID_HANDLE_VALUE || 0 == hFind ) return false;

	BOOL	bFind = TRUE;
	do
	{
		if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
		{
			tstring libname(strPath + findData.cFileName);
			if (! LoadLibrary( (libname).c_str() ))
			{
				DXHUI_LogInfo(  TEXT("Load lib failed ")<< libname.c_str()<< TEXT("ERR = ")<<GetLastError() );//<<(findData.cFileName));
			}
		}
	}while((bFind  = FindNextFile( hFind, &findData )));
	return true;
}