#pragma once
#ifndef _INCDXHUI
#define _INCDXHUI
#include <iostream>
#include <sstream>
using namespace std;
//diy event mode.
#include "Delegate.h"

#ifndef tstring
#ifdef _UNICODE
#define	tstring wstring
#define tstringstream wstringstream
#else
#define tstring	string
#define tstringstream stringstream
#endif
#endif

#define	DXHUI_Log(tag,str)\
do{\
tstringstream	ss;\
ss<<TEXT("DXHUI - ")<<tag<<str<<std::endl;\
OutputDebugString(ss.str().c_str());\
}while(false);

#define	DXHUI_LogErr(str) //DXHUI_Log( TEXT("ERROR - "),str)
#define	DXHUI_LogInfo(str)// DXHUI_Log( TEXT("INFO - "),str)

#define		DXHUI_INTERFACE	class __declspec(novtable)

DXHUI_INTERFACE	IDXHUIBase
{
public:
	virtual	~IDXHUIBase()		{};
	virtual bool Create()		=	0;
	virtual	bool Destroy()		=	0;
	virtual bool Release()		=	0;
	virtual void DecRef()		=	0;
	virtual void AddRef()		=	0;
};

DXHUI_INTERFACE	IDXHUIDev : public IDXHUIBase
{
public:
	virtual ~IDXHUIDev(){}
	virtual	UINT	GetSDKVer ()	=	0;
	virtual	void*	GetDXDevice()	=	0;
};

IDXHUIBase* CreateHUI(LPCTSTR interfaceName);


#endif