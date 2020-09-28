#pragma once

#include <Windows.h>
#include <wrl.h>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <dxgi1_4.h>

#include "D3D12Exception.h"
#include "d3dx12.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;

constexpr void ThrowIfFailed(const HRESULT result)
{
	if (FAILED(result))
	{
		throw D3D12Exception("");
	}
}

constexpr void ThrowIfFailed(const HRESULT result, const std::string& message)
{
	if (FAILED(result))
	{
		throw D3D12Exception(message);
	}
}

constexpr void ThrowIfFailed(const HRESULT result, std::string&& message)
{
	if (FAILED(result))
	{
		throw D3D12Exception(std::move(message));
	}
}