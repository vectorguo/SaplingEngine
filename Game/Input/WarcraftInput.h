#pragma once

#include "SaplingEngineHeader.h"
#include "WarcraftEnum.h"

namespace Warcraft
{
	class WarcraftInput final : public SaplingEngine::Component
	{
	public:
		WarcraftInput() = default;
		~WarcraftInput() override = default;

		/*
		 * 禁止拷贝和移动
		 */
		WarcraftInput(const WarcraftInput&) = delete;
		WarcraftInput(WarcraftInput&&) = delete;
		WarcraftInput& operator= (const WarcraftInput&) = delete;
		WarcraftInput& operator= (WarcraftInput&&) = delete;
		
		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_WarcraftInput;
		}
		
		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

	protected:
		/**
		 * \brief Start
		 */
		void Start() override;

		/**
		 * \brief 更新
		 */
		void Update() override;
		
		/**
		 * \brief 更新相机
		 */
		void UpdateCamera();

	private:
		/**
		 * \brief 相机
		 */
		SaplingEngine::CameraSptr m_MainCamera;

		/**
		 * \brief 鼠标位置
		 */
		Vector2 m_MousePosition;

		/**
		 * \brief 相机操作值 0：无操作， 1：移动相机 2：旋转相机 3：拉近拉远
		 */
		int32_t m_CameraOperation = 0;
	};
}
