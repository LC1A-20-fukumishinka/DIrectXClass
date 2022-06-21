#pragma once

template<class T>
inline void SafeDelete(T *&obj)
{
	if (obj)
	{
		delete obj;
	}
	obj = nullptr;
}