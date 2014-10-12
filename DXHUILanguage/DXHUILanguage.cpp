// DXHUILanguage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../DXHUI/incDXHUI.h"
#include "../DXHUI/DXHUILib.h"
#include "incLanguage.h"
#include <map>
using namespace std;

class	CSubLang	: public ISubLang
{
public:
	LPCTSTR	GetLang( LPCTSTR szTag, LPCTSTR	szName, LPCTSTR	szDefVal )	;
	bool	LoadDir( LPCTSTR szLangTag );
	bool	LoadFile( LPCTSTR szFileName );
	bool	Free()
	{
		m_mMapTLang.clear();
		return true;
	}
	void	trimstr( tstring& s)
	{
		tstring	cur;
		const TCHAR*	p = s.c_str();
		while( *p && IsSpace(p[0]) )
		{
			p++;
		}
		cur = p;
		tstring tmp(cur);

		size_t t = tmp.size()-1;
		while( t )
		{
			if ( IsSpace(tmp[t]) )
				t--;
		}
		s = tmp.substr(0,t);
	}
	bool	IsSpace( TCHAR x )
	{
		if (x == TEXT('\r') ||
			x == TEXT('\n') ||
			x == TEXT('\t') ||
			x == TEXT(' '))
			return true;
		return false;
	}
	int		findSpace( TCHAR* p )
	{
		if ( p )
		{
			int i = 0;
			while( *p )
			{
				if ( IsSpace( *p ))
					return i;
				else{
					i++;
					p++;
				}
			}

			if ( *p == 0 ) return i;
		}
		return -1;
	}
protected:
	typedef map<tstring, tstring>	mMapLang;
	typedef mMapLang::iterator		mMapLangI;
	typedef map<tstring, mMapLang>	mMapTLang;
	typedef mMapTLang::iterator		mMapTLangI;

	mMapTLang			m_mMapTLang;
};

LPCTSTR	CSubLang::GetLang(LPCTSTR szTag, LPCTSTR	szName, LPCTSTR	szDefVal )	
{
	mMapTLangI i = m_mMapTLang.find( szTag );
	if ( i == m_mMapTLang.end() ) return szDefVal;

	mMapLangI x = i->second.find( szName );
	if ( x == i->second.end() )	return szDefVal;
	
	return x->second.c_str();
}

bool	CSubLang::LoadFile( LPCTSTR szFileName )
{
	FILE*	pFile = NULL;
	_tfopen_s( &pFile, szFileName, TEXT("r"));
	if ( pFile )
	{ 
		const TCHAR* pTagEnd = _tcsrchr( szFileName, TEXT('.'));
		tstring strTag(szFileName,pTagEnd - szFileName);
		size_t pos = strTag.rfind( TEXT('\\') );
		if ( -1 != pos )
		{
			strTag = strTag.substr( pos+1 );
		}
		if (this->m_mMapTLang.find( strTag ) == m_mMapTLang.end())
		{
			m_mMapTLang.insert( mMapTLang::value_type(strTag, mMapLang()));
		}
		mMapLang&	curMap = m_mMapTLang[strTag];
		
		size_t	buSize	=	0;
		fseek( pFile, 0, SEEK_END);
		buSize	=	ftell(pFile);
		fseek( pFile, 0, SEEK_SET);

		TCHAR*	pszBuff	=	new TCHAR[buSize+1];
		if ( !pszBuff )
		{
			DXHUI_LogErr( TEXT("MEMORY NO ENOUGHT!"));
			fclose(pFile);
			return false;
		}
		memset( pszBuff, 0, sizeof(TCHAR)*(1+buSize));
		if (!fread_s( pszBuff, sizeof(TCHAR)*buSize, 1,sizeof(TCHAR)*buSize, pFile))
		{
			DXHUI_LogErr( TEXT("Read language file failed!" ));
			fclose(pFile);
			return false;
		}
 
		TCHAR*	pChr	=	pszBuff;
		TCHAR*	pNULL	=	TEXT("\0\n");
		TCHAR*	pContent= NULL;
		TCHAR* pLine = _tcstok_s( pChr, pNULL, &pContent); 

		while( pLine != NULL )
		{
			OutputDebugString( pLine );
			const TCHAR*	pFG = _tcschr( pLine, TEXT('\t'));
			if ( pFG){
				tstring tname( pLine, pFG - pLine );
				tstring tval ( pFG+1 );
				trimstr( tname );
				trimstr( tval );
				if ( tname.size() && tval.size() )
				{
					curMap[tname]	=	tval;
				}
			}
			pLine = _tcstok_s( NULL, pNULL, &pContent);
		}
		fclose(pFile);
	}
	return false;
}
bool	CSubLang::LoadDir( LPCTSTR szTagDir )
{
	if (!szTagDir) return false;
	Free();

	tstring	tagstr(szTagDir);
	if ( tagstr[tagstr.size()-1] != TEXT('\\') )
		tagstr.append(TEXT("\\"));
	tstring curdir(tagstr);
	tagstr.append(TEXT("*.lang"));

	WIN32_FIND_DATA	findData;
	HANDLE	hFind = ::FindFirstFile( tagstr.c_str(), &findData); 
	if ( hFind == INVALID_HANDLE_VALUE || 0 == hFind ) return false;

	BOOL	bFind = TRUE;
	do
	{
		if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ))
		{
			if (!LoadFile( (curdir + findData.cFileName ).c_str() ))
			{
				DXHUI_LogInfo(  TEXT("Load language file failed.") );//<<(findData.cFileName));
			}
		}
	}while((bFind  = FindNextFile( hFind, &findData )));
	return m_mMapTLang.size() > 0;
}

class	CLanguage : public IDXHUI_CLASS<CLanguage>, public ILanguage
{
public:
	virtual	bool	LoadLanguage( LPCTSTR szCHN )	;
	virtual	bool	FreeLanguage()					;
	virtual	ISubLang*	GetCurLang()		
	{
		return &m_subLang;
	}

protected:
	CSubLang	m_subLang;
};

bool	CLanguage::LoadLanguage( LPCTSTR szCHN )
{
	TCHAR	szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	::GetModuleFileName( NULL, szPath, MAX_PATH);
	*(_tcsrchr( szPath, TEXT('\\')) + 1) = TEXT('\0');
	tstring	strPath(szPath);
	strPath.append(TEXT("Language\\"));
	strPath.append(szCHN);

	return m_subLang.LoadDir(strPath.c_str());
}

bool	CLanguage::FreeLanguage()
{
	return m_subLang.Free();
}
EXPORT_DXHUI_CLASS(	CLanguage,	Language, ILanguage );

