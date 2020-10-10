#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(165);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameApplication* pApp = new D3D12Application();
	if (pApp->InitializeApplicationConfig() && pApp->InitializeApplication(hInstance))
	{
		pApp->Run();
		pApp->Destroy();
		
		//运行结束，销毁GameApplication
		delete pApp;
	}
	
    return 0;
}
