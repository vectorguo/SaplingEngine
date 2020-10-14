#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(165);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//初始化游戏设置
	if (GameSetting::Instance()->Initialize())
	{
		//初始化程序
		if (GameApplication::Instance()->Initialize(hInstance))
		{
			GameApplication::Instance()->Run();
			GameApplication::Instance()->Destroy();
		}
	}

    return 0;
}
