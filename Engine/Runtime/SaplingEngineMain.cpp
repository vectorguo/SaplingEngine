#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(165);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//��ʼ����Ϸ����
	if (GameSetting::Instance()->Initialize())
	{
		//��ʼ������
		if (GameApplication::Instance()->Initialize(hInstance))
		{
			GameApplication::Instance()->Run();
			GameApplication::Instance()->Destroy();
		}
	}

    return 0;
}
