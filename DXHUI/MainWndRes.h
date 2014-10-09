#pragma once
#ifndef _MAINWNDRES_HEADER
#define _MAINWNDRES_HEADER
#include "DXUT\DXUTgui.h"

struct	IMainWndRes
{
	virtual	 CDXUTDialogResourceManager*	GetDialogRM()	= 0;
};


#endif