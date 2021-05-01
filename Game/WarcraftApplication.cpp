#include "SaplingEngineHeader.h"
#include "GameObject/ComponentFactory.h"
#include "Utility/WarcraftComponentFactory.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(377);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//设置ComponentCreator
	ComponentFactory::SetComponentCreator(Warcraft::CreateComponent);
	
	//初始化游戏设置
	if (GameSetting::Initialize())
	{
		//初始化程序
		if (GameApplication::Initialize(hInstance))
		{
			GameApplication::Run();
			GameApplication::Destroy();
		}
	}

    return 0;
}
