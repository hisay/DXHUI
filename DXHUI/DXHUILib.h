#pragma once
#include "incDXHUI.h"
#include <vector>
#include <tchar.h>
#include <string>
#include <string.h>
#include <xstring>
#include <map>
using namespace std;

#define	EXPORT_DXHUI_CLASS(className, strClassName, interfaceName)\
	template<> IDXHUI_CLASSImp<className> className::s_Object(TEXT(#strClassName), TEXT(#interfaceName));

class	NullDXHUIBase : public virtual IDXHUIBase
{
public:
	NullDXHUIBase(){ m_nRefCount = 0;}
	virtual ~NullDXHUIBase(){}
	virtual bool Create()	{ return false;}
	virtual	bool Destroy()	{ return false;}
	virtual bool Release()	{ if((m_nRefCount--) <= 0){ delete this;} return true;}
	virtual void DecRef()	{ if(m_nRefCount) m_nRefCount--;}
	virtual void AddRef()	{ m_nRefCount++;}
protected:
	UINT		m_nRefCount;
};

class IDXHUI_CLASSDef
{
public:
	virtual	~IDXHUI_CLASSDef(){}
	virtual IDXHUIBase*	CreateObject() = 0;
 
	virtual tstring&	GetClsName()	= 0;
 
	virtual tstring&	GetInterfaceName() = 0;
 
};
struct	ILibCore
{
	virtual void	RegisterDXHUIClass(IDXHUI_CLASSDef* ptr) = 0;
	virtual		IDXHUIBase*	CreateClassObject( LPCTSTR)		 = 0;
};
struct	IDXHUILib
{
	static	ILibCore*	GetDXHUILib()
	{
		static ILibCore*	pLib = NULL;
		if(pLib) return pLib;

		HINSTANCE	hDll = LoadLibrary(TEXT("DXHUILibCore.dll"));
		if(hDll)
		{
			typedef ILibCore* (WINAPI *fnGetCore)();
			fnGetCore fgc = (fnGetCore)GetProcAddress(hDll, "GetDXHUILibCore");
			if ( fgc )
				pLib = (ILibCore*)fgc();
		}
		return pLib;
	}
	static void	RegisterDXHUIClass(IDXHUI_CLASSDef* ptr)
	{
		ILibCore*	pLib = GetDXHUILib();
		if(pLib)
			return pLib->RegisterDXHUIClass(ptr);
	}
	static IDXHUIBase*	CreateClassObject(LPCTSTR clsName)
	{
			ILibCore*	pLib = GetDXHUILib();
		if(pLib)
			return pLib->CreateClassObject(clsName);
		return NULL;
	}
};

template<typename subName>
class IDXHUI_CLASSImp : public virtual IDXHUI_CLASSDef
{
public:
	IDXHUI_CLASSImp( LPCTSTR clsName, LPCTSTR infName)
	{
		m_clsName = clsName;
		m_infName = infName;
		IDXHUILib::RegisterDXHUIClass(this);
	}
	virtual ~IDXHUI_CLASSImp()
	{
	}
	virtual IDXHUIBase*	CreateObject()
	{
		return dynamic_cast<IDXHUIBase*>( new subName() );
	}
	virtual tstring&		GetClsName()
	{
		return	m_clsName ;
	}
	virtual tstring&		GetInterfaceName()
	{
		return  m_infName ;
	}
protected:
		tstring	m_clsName,m_infName;
};

class	IDXHUI_CLASS_BASE : public virtual NullDXHUIBase
{
public:
	virtual ~IDXHUI_CLASS_BASE(){}
	virtual IDXHUIBase*	CreateObject() = 0;
	virtual	IDXHUI_CLASSDef*	GetClassDef() = 0;
};


template<typename subName>
class IDXHUI_CLASS : public virtual IDXHUI_CLASS_BASE
{
public:
	virtual ~IDXHUI_CLASS(){}
	IDXHUI_CLASS( )
	{
		
	};
	IDXHUI_CLASSDef*	GetClassDef() 
	{
		return &s_Object;
	}
	IDXHUIBase*	CreateObject() 
	{
		return s_Object.CreateObject();
	}
protected:
	static	IDXHUI_CLASSImp<subName>	s_Object;

};

template <typename Type,typename Ptr>
class	DXHUIDataMap
{
public:
	typedef typename map<Type,Ptr*>	MTP;
	typedef typename MTP::iterator	IMPT;
	virtual ~DXHUIDataMap()
	{
		m_vPtr.clear();
	}
	Ptr*	Find(Type t)
	{
		for( IMPT i = m_vPtr.begin(); i != m_vPtr.end(); i++)
		{
			if ( i->first == t ) return i->second;
		}
		return NULL;
	}
	void	Add(Type t, Ptr* p)
	{
		m_vPtr[t] = p;
	}
	void	Del(Type t)
	{
		for( IMPT i = m_vPtr.begin(); i != m_vPtr.end(); i++)
		{
			if ( i->first == t ) { m_vPtr.erase(i); return;}
		}
	}
protected:
	MTP	m_vPtr;
};