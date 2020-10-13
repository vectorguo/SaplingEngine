#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(165);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (GameSetting::Instance()->Initialize())
	{
		auto* pApp = new GameApplication();
		if (pApp->InitializeApplication(hInstance))
		{
			pApp->Run();
			pApp->Destroy();

			//���н���������GameApplication
			delete pApp;
		}
	}

    return 0;
}
