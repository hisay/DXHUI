/*
author: you not't need know!
*/

#pragma once

 

template<class T> void Init_Array( T *t, int count, T value )
{
	for ( int i = 0 ; i < count ; i++ )
	{
		t[i] = value ;
	}
}

extern "C" void __cdecl memcpyMMX(void *Dest, void *Src, size_t nBytes) ;
