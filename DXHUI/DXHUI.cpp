// DXHUI.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "DXHUILib.h"

IDXHUIBase* CreateHUI(LPCTSTR interfaceName)
{
	return IDXHUILib::CreateClassObject(interfaceName);
	return NULL;
}