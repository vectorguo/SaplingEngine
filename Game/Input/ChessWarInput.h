#pragma once

#include "ChessWarEnum.h"
#include "GameObject/Component.h"

namespace ChessWar
{
	class ChessWarInput final : public SaplingEngine::Component
	{
	public:
		ChessWarInput() = default;
		~ChessWarInput() override = default;

		/*
		 * 禁止拷贝和移动
		 */
		ChessWarInput(const ChessWarInput&) = delete;
		ChessWarInput(ChessWarInput&&) = delete;
		ChessWarInput& operator= (const ChessWarInput&) = delete;
		ChessWarInput& operator= (ChessWarInput&&) = delete;
		
		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_ChessWarInput;
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
	};
}
