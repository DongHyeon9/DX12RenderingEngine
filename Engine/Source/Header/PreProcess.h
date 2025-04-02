#pragma once
#pragma region 개발편의

#define DECLARE_SINGLETON(type)				\
private:									\
	type() { }								\
public:										\
	static type* GetInstance()				\
	{										\
		static type singleinst;				\
		return &singleinst;					\
	}

#define TYPE(container) decltype(*container.data())
#define KEY_DOWN(key) InputManager::GetInstance()->GetButtonDown(key)
#define KEY_UP(key) InputManager::GetInstance()->GetButtonUp(key)
#define KEY_PRESS(key) InputManager::GetInstance()->GetButtonPress(key)

#pragma warning(disable : 4302)

#pragma endregion 개발편의

#pragma region 디버깅용

#ifdef _DEBUG
#ifdef UNICODE

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define LOG_LINE std::wcout << __FUNCTION__ << "(" << __LINE__ << ")";
#define LOG(str, ...) LOG_LINE std::wcout <<"\t";wprintf_s(TEXT(str), ##__VA_ARGS__);std::wcout <<std::endl;

#else

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#define LOG_LINE std::cout << __FUNCTION__ << "(" << __LINE__ << ")";
#define LOG(str, ...) LOG_LINE std::cout <<"\t";printf_s(str, ##__VA_ARGS__);std::cout <<std::endl;

#endif	//UNICODE

#else
#define LOG_LINE ((void)0)
#define LOG(str,...) ((void)0)
#endif	//_DEBUG

#define WARNING(exp,str) if(!(exp)){LOG(str);}
#define CHECK(exp,str,ret)		\
if(!(exp))	{					\
	LOG(str);					\
	MessageBox(NULL,			\
		TEXT(str),				\
		TEXT("Error"),			\
		MB_OK | MB_ICONERROR);	\
	return ret;					\
}

#pragma endregion 디버깅용

#pragma region 유니코드

#ifdef UNICODE
#define MAIN wWinMain
#else
#define MAIN WinMain
#endif

#pragma endregion 유니코드