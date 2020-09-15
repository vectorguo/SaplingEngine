#pragma once

#include "GameApplication.h"

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
		 * ¸üÐÂ
		 */
		void Update() override;

		/*
		 * »æÖÆ
		 */
		void Render() override;
	};
}