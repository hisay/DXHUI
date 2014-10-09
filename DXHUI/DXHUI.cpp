// DXHUI.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DXHUILib.h"

IDXHUIBase* CreateHUI(LPCTSTR interfaceName)
{
	return IDXHUILib::CreateClassObject(interfaceName);
	return NULL;
}