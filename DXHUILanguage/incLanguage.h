#pragma once

struct	ISubLang
{
	virtual	LPCTSTR	GetLang( LPCTSTR szTag, LPCTSTR	szName, LPCTSTR	szDefVal )	=	0;
};
struct	ILanguage
{
	virtual	bool	LoadLanguage( LPCTSTR szCHN )	=	0;
	virtual	bool	FreeLanguage()					=	0;
	virtual	ISubLang*	GetCurLang()				=	0;
};