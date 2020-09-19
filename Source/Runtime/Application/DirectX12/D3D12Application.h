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
		 * ��ʼ��
		 */
		bool InitializeConfig() override;
		bool InitializeApplication(HINSTANCE hInstance) override;
		
		/*
		 * ����
		 */
		void Update() override;

		/*
		 * ����
		 */
		void Render() override;

	private:
		/*
		 * ��Ϣ����ص�
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		/*
		 * ��ʼ������
		 */
		bool InitializeWindow();

		/*
		 * ��ʼ��DirectX12
		 */
		bool InitializeDirectX12();

		/*
		 * ��Ϣ����
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * ���ڱ仯�ص�
		 */
		void OnResize();
	};
}