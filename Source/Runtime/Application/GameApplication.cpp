#include "Application/GameApplication.h"

namespace SaplingEngine
{
	GameApplication* GameApplication::s_Instance = nullptr;

	GameApplication::GameApplication()
	{
		s_Instance = this;
	}

	GameApplication::~GameApplication()
	{
	}

	/**
	 * \brief 运行
	 */
	int GameApplication::Run()
	{
		Time::Initialize();
		
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{//处理消息
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Time::Tick();

				Update();
				Render();
			}
		}

		return static_cast<int>(msg.wParam);
	}
}
