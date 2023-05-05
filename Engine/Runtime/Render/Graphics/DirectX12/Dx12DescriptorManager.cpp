#include "Runtime/Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Runtime/Render/Graphics/DirectX12/GraphicsManager.h"

namespace SaplingEngine
{
	uint32_t									Dx12DescriptorManager::rtvDescriptorSize = 0;
	uint32_t									Dx12DescriptorManager::dsvDescriptorSize = 0;
	uint32_t									Dx12DescriptorManager::cbvSrvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap>				Dx12DescriptorManager::defaultRtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>				Dx12DescriptorManager::defaultDsvDescriptorHeap;

	std::vector<ComPtr<ID3D12DescriptorHeap>>	Dx12DescriptorManager::objectCbvDescriptorHeaps;
	std::vector<Dx12UploadBuffer*>				Dx12DescriptorManager::objectCbUploadBuffers;
	ComPtr<ID3D12DescriptorHeap>				Dx12DescriptorManager::passCbvDescriptorHeap;
	Dx12UploadBuffer*							Dx12DescriptorManager::passCbUploadBuffer;

	ComPtr<ID3D12DescriptorHeap>				Dx12DescriptorManager::defaultSrvDescriptorHeap;
	std::vector<uint32_t>						Dx12DescriptorManager::availableSrvIndices;

	void Dx12DescriptorManager::Initialize()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		dsvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		cbvSrvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		const auto swapBufferCount = GraphicsManager::swapChainBufferCount;

		//创建默认渲染缓冲区和深度/模板缓冲区描述符堆
		CreateDescriptorHeap(pDevice, defaultRtvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapBufferCount);
		CreateDescriptorHeap(pDevice, defaultDsvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);

		auto* pSwapChainBuffer = GraphicsManager::swapChainBuffer;
		auto* pDepthStencilBuffer = GraphicsManager::depthStencilBuffer.Get();

		//创建渲染目标View
		for (auto i = 0; i < swapBufferCount; ++i)
		{
			CreateRenderTargetView(pDevice, defaultRtvDescriptorHeap.Get(), pSwapChainBuffer[i].Get(), i);
		}

		//创建深度模板View
		CreateDepthStencilView(pDevice, GraphicsManager::depthStencilViewFormat, defaultDsvDescriptorHeap.Get(), pDepthStencilBuffer, 0);

		//创建Pass常量缓冲区描述符堆与上传缓冲区
		CreatePassCbvDescriptorHeap();

		//创建Srv缓冲区描述符堆
		CreateSrvDescriptorHeap();
	}

	void Dx12DescriptorManager::Destroy()
	{
		defaultRtvDescriptorHeap.Reset();
		defaultDsvDescriptorHeap.Reset();

		for (auto iter = objectCbvDescriptorHeaps.begin(); iter != objectCbvDescriptorHeaps.end(); ++iter)
		{
			iter->Reset();
		}
		objectCbvDescriptorHeaps.clear();

		for (auto iter = objectCbUploadBuffers.begin(); iter != objectCbUploadBuffers.end(); ++iter)
		{
			delete* iter;
		}
		objectCbUploadBuffers.clear();

		passCbvDescriptorHeap.Reset();
		delete passCbUploadBuffer;

		defaultSrvDescriptorHeap.Reset();
		availableSrvIndices.clear();
	}

	void Dx12DescriptorManager::CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		auto* pDevice = GraphicsManager::GetDx12Device();
		CreateDescriptorHeap(pDevice, descriptorHeap, type, descriptorCount, flags);
	}

	void Dx12DescriptorManager::CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = type;
		heapDesc.NumDescriptors = descriptorCount;
		heapDesc.Flags = flags;
		heapDesc.NodeMask = 0;

		ThrowIfFailed(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf())));
	}
	
	void Dx12DescriptorManager::CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		CreateRenderTargetView(GraphicsManager::GetDx12Device(), pHeap, pResource, offset);
	}

	void Dx12DescriptorManager::CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		auto descriptorHandle = GetCPUHandleFromDescriptorHeap(pHeap, offset, rtvDescriptorSize);
		pDevice->CreateRenderTargetView(pResource, nullptr, descriptorHandle);
	}
	
	void Dx12DescriptorManager::CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		CreateDepthStencilView(GraphicsManager::GetDx12Device(), format, pHeap, pResource, offset);
	}
	
	void Dx12DescriptorManager::CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = format;
		dsvDesc.Texture2D.MipSlice = 0;

		auto descriptorHandle = GetCPUHandleFromDescriptorHeap(pHeap, offset, dsvDescriptorSize);
		pDevice->CreateDepthStencilView(pResource, &dsvDesc, descriptorHandle);
	}

	void Dx12DescriptorManager::CreateShaderResourceView(ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension)
	{
		CreateShaderResourceView(GraphicsManager::GetDx12Device(), pResource, index, shaderMapping, format, viewDimension);
	}

	void Dx12DescriptorManager::CreateShaderResourceView(ID3D12Device* pDevice, ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = shaderMapping;
		srvDesc.Format = format;
		srvDesc.ViewDimension = viewDimension;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		GraphicsManager::GetDx12Device()->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandleFromDescriptorHeap(defaultSrvDescriptorHeap.Get(), index, cbvSrvDescriptorSize));
	}
	
	D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorManager::GetRenderTargetView()
	{
		return GetCPUHandleFromDescriptorHeap(defaultRtvDescriptorHeap.Get(), GraphicsManager::currentSwapChainIndex, rtvDescriptorSize);
	}
	
	D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorManager::GetDepthStencilView()
	{
		return defaultDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}
	
	/**
	 * \brief	获取对象常量缓冲区描述符堆
	 */
	void Dx12DescriptorManager::GetObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer*& pUploadBuffer)
	{
		if (objectCbvDescriptorHeaps.empty())
		{
			auto* pDevice = GraphicsManager::GetDx12Device();

			//创建描述符堆
			CreateDescriptorHeap(pDevice, descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, ObjectCbvDescriptorCount * 2, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

			//创建上传缓冲区
			pUploadBuffer = new Dx12UploadBuffer(ObjectCbvDescriptorCount * (ObjectCommonCbSize + ObjectSpecialCbSize));

			//创建描述符
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			auto bufferLocation = pUploadBuffer->GetGpuVirtualAddress();
			for (uint32_t index = 0; index < ObjectCbvDescriptorCount; ++index)
			{
				cbvDesc.BufferLocation = bufferLocation;
				cbvDesc.SizeInBytes = ObjectCommonCbSize;
				pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap.Get(), index, cbvSrvDescriptorSize));

				bufferLocation += ObjectCommonCbSize;
			}

			//创建特殊数据的常量缓冲区描述符
			for (uint32_t index = 0; index < ObjectCbvDescriptorCount; ++index)
			{
				cbvDesc.BufferLocation = bufferLocation;
				cbvDesc.SizeInBytes = ObjectSpecialCbSize;
				pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(descriptorHeap.Get(), ObjectCbvDescriptorCount + index, cbvSrvDescriptorSize));

				bufferLocation += ObjectSpecialCbSize;
			}
		}
		else
		{
			descriptorHeap = *objectCbvDescriptorHeaps.rbegin();
			pUploadBuffer = *objectCbUploadBuffers.rbegin();
			objectCbvDescriptorHeaps.pop_back();
			objectCbUploadBuffers.pop_back();
		}
	}
	
	/**
	 * \brief	归还对象常量缓冲区描述符堆
	 */
	void Dx12DescriptorManager::ReturnObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer* pUploadBuffer)
	{
		objectCbvDescriptorHeaps.push_back(descriptorHeap);
		objectCbUploadBuffers.push_back(pUploadBuffer);
	}
	
	/**
	 * \brief	创建Pass常量缓冲区描述符堆
	 */
	void Dx12DescriptorManager::CreatePassCbvDescriptorHeap()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();

		CreateDescriptorHeap(pDevice, passCbvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, PassCbvDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		//创建上传缓冲区
		passCbUploadBuffer = new Dx12UploadBuffer(PassCbvDescriptorCount * PassCommonCbSize);

		//创建通用数据的常量缓冲区描述符
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		auto bufferLocation = passCbUploadBuffer->GetGpuVirtualAddress();
		for (uint32_t index = 0; index < PassCbvDescriptorCount; ++index)
		{
			cbvDesc.BufferLocation = bufferLocation;
			cbvDesc.SizeInBytes = PassCommonCbSize;
			pDevice->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(passCbvDescriptorHeap.Get(), index, cbvSrvDescriptorSize));

			bufferLocation += PassCommonCbSize;
		}
	}
	
	/**
	 * \brief	创建Srv缓冲区描述符堆
	 */
	void Dx12DescriptorManager::CreateSrvDescriptorHeap()
	{
		//创建SRV描述符堆
		CreateDescriptorHeap(defaultSrvDescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, SrvCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

		//填充可用描述符索引列表
		availableSrvIndices.reserve(SrvCount);
		for (auto i = 0; i < SrvCount; ++i)
		{
			availableSrvIndices.push_back(i);
		}
	}
}