#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameApplication* pApp = new D3D12Application();
	if (pApp->Initialize(hInstance))
	{
		return pApp->Run();
	}
	
    return 0;
}
