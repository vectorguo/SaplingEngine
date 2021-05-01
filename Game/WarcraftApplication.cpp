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

	//����ComponentCreator
	ComponentFactory::SetComponentCreator(Warcraft::CreateComponent);
	
	//��ʼ����Ϸ����
	if (GameSetting::Initialize())
	{
		//��ʼ������
		if (GameApplication::Initialize(hInstance))
		{
			GameApplication::Run();
			GameApplication::Destroy();
		}
	}

    return 0;
}
