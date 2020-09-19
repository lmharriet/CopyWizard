#pragma once

template <class T>
class singletonBase
{
protected:
	//ΩÃ±€≈Ê ∞¥√º º±æ
	static T* singleton;

	singletonBase() {}
	~singletonBase() {}
public:
	//ΩÃ±€≈Ê ∞¥√º ∞°¡Æø¿±‚
	static T* getSingleton();
	//ΩÃ±€≈Ê ∞¥√º «ÿ¡¶«œ±‚
	void releaseSingleton();
};

//ΩÃ±€≈Ê ∞¥√º √ ±‚»≠
template<class T>
T* singletonBase<T>::singleton = nullptr;

//ΩÃ±€≈Ê ∞¥√º ∞°¡Æø¿±‚
template<class T>
inline T * singletonBase<T>::getSingleton()
{
	//ΩÃ±€≈Ê¿Ã æ¯¿∏∏È ªı∑Œ ª˝º∫
	if (!singleton) singleton = new T;
	return singleton;
}

//ΩÃ±€≈Ê ∞¥√º «ÿ¡¶«œ±‚
template<class T>
inline void singletonBase<T>::releaseSingleton()
{
	//ΩÃ±€≈Ê¿Ã ¿÷¥Ÿ∏È ∏ﬁ∏∏Æø°º≠ «ÿ¡¶
	if (singleton)
	{
		delete singleton;
		singleton = nullptr;
	}
}