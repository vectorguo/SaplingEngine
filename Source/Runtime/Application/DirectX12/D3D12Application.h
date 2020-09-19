#pragma once

#include "Application/GameApplication.h"

namespace SaplingEngine
{
	class D3D12Application : public GameApplication
	{
	public:
		D3D12Application();
		~D3D12Application() override;

		D3D12Application(const D3D12Application&) = delete;
		D3D12Application(D3D12Application&&) = delete;
		D3D12Application& operator=(const D3D12Application&) = delete;
		D3D12Application& operator=(D3D12Application&&) = delete;
		
		/*
		 * 初始化
		 */
		bool InitializeConfig() override;
		bool InitializeApplication(HINSTANCE hInstance) override;
		
		/*
		 * 更新
		 */
		void Update() override;

		/*
		 * 绘制
		 */
		void Render() override;

	private:
		/*
		 * 消息处理回调
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		/*
		 * 初始化窗口
		 */
		bool InitializeWindow();

		/*
		 * 初始化DirectX12
		 */
		bool InitializeDirectX12();

		/*
		 * 消息处理
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * 窗口变化回调
		 */
		void OnResize();
	};
}