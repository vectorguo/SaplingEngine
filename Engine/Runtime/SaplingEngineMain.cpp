#include "SaplingEngineHeader.h"

using namespace SaplingEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetBreakAlloc(377);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//��ʼ����Ϸ����
	if (Setting::Initialize())
	{
		//��ʼ������
		if (Application::Initialize(hInstance))
		{
			Application::Run();
			Application::Destroy();
		}
	}

	return 0;
}