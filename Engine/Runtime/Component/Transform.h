#pragma once

#include "Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
		friend class GameObject;

	public:
		Transform();

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_Transform;
		}

		/**
		 * \brief	获取位置
		 * \return	位置（不可修改）
		 */
		const Vector3& GetPosition();

		/**
		 * \brief	获取旋转
		 * \return	旋转（不可修改）
		 */
		const Quaternion& GetRotation();
		
		/**
		 * \brief	获取局部坐标下的位置
		 * \return	局部坐标下的位置（不可修改）
		 */
		const Vector3& GetLocalPosition();

		/**
		 * \brief	获取局部坐标下的旋转
		 * \return	局部坐标下的旋转（不可修改）
		 */
		const Quaternion& GetLocalRotation();

		/**
		 * \brief	获取缩放
		 * \return	缩放（不可修改）
		 */
		const Vector3& GetLocalScale();

		/**
		 * \brief	获取forward向量
		 * \return	forward向量
		 */
		Vector3 GetForward();

		/**
		 * \brief	获取right向量
		 * \return	right向量
		 */
		Vector3 GetRight();

		/**
		 * \brief	获取up向量
		 * \return	up向量
		 */
		Vector3 GetUp();
		
		/**
		 * \brief	获取局部坐标到世界坐标的变换矩阵
		 * \return	局部坐标到世界坐标的变换矩阵
		 */
		const Matrix4x4& GetLocalToWorldMatrix()
		{
			RefreshMatrix();
			return m_LocalToWorldMatrix;
		}

		const Matrix4x4& GetWorldToLocalMatrix()
		{
			RefreshMatrix();
			return m_WorldToLocalMatrix;
		}

		/**
		 * \brief	设置位置
		 * \param	position		位置
		 */
		void SetPosition(const Vector3& position);

		/**
		 * \brief	设置位置
		 * \param	x				x坐标
		 * \param	y				y坐标
		 * \param	z				z坐标
		 */
		void SetPosition(float x, float y, float z);

		/**
		 * \brief	设置旋转
		 * \param	rotation		旋转
		 */
		void SetRotation(const Quaternion& rotation);

		/**
		 * \brief	设置旋转的欧拉角
		 * \param	eulerAngles		欧拉角
		 */
		void SetEulerAngles(const Vector3& eulerAngles);

		/**
		 * \brief	设置旋转的欧拉角
		 */
		void SetEulerAngles(float x, float y, float z);

		/**
		 * \brief	设置局部坐标下的位置
		 * \param	localPosition	局部坐标下的位置
		 */
		void SetLocalPosition(const Vector3& localPosition);

		/**
		 * \brief	设置局部坐标下的位置
		 * \param	x				x坐标
		 * \param	y				y坐标
		 * \param	z				z坐标
		 */
		void SetLocalPosition(float x, float y, float z);

		/**
		 * \brief	设置局部坐标下的旋转
		 * \param	localRotation	局部坐标下的旋转
		 */
		void SetLocalRotation(const Quaternion& localRotation);

		void SetLocalRotation(float x, float y, float z, float w);

		/**
		 * \brief	设置局部坐标下旋转的欧拉角
		 * \param	eulerAngles		欧拉角
		 */
		void SetLocalEulerAngles(const Vector3& eulerAngles);

		/**
		 * \brief	设置局部坐标下旋转的欧拉角
		 */
		void SetLocalEulerAngles(float x, float y, float z);

		/**
		 * \brief	设置缩放
		 * \param	localScale		缩放
		 */
		void SetLocalScale(const Vector3& localScale);

		void SetLocalScale(float x, float y, float z);

		/**
		 * \brief	LookAt
		 * \param	focusPosition	LookAt目标位置
		 */
		void LookAt(const Vector3 focusPosition);

		/**
		 * \brief	获取parent指针
		 * \return	parent指针
		 */
		inline Transform* GetParent() const
		{
			return m_Parent;
		}

		/**
		 * \brief	获取parent的智能指针
		 * \return	parent智能指针
		 */
		inline TransformSptr GetParentSptr() const
		{
			return SharedFromThis(m_Parent);
		}

		/**
		 * \brief	是否有父节点
		 * \return	是否有父节点
		 */
		inline bool HasParent() const
		{
			return m_Parent != nullptr;
		}

		/**
		 * \brief	设置parent
		 * \param	pParent			parent指针
		 */
		void SetParent(Transform* pParent);

		/**
		 * \brief	设置parent
		 * \param	parentSptr		parent智能指针
		 */
		void SetParent(const TransformSptr& parentSptr)
		{
			SetParent(parentSptr.Get());
		}

	protected:
		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;

		/**
		 * \brief	Transform被销毁时的回调
		 */
		void OnDestroy() override;

	private:
		/**
		 * \brief	对应标记的数据是否是脏数据
		 *			0x01: 世界空间下的位置
		 *			0x02: 世界空间下的旋转
		 *			0x04: 局部空间下的位置
		 *			0x08: 局部空间下的旋转
		 *			0x10: 局部空间下的缩放
		 *			0x20: 变换矩阵
		 * \param	flag		标记类型
		 */
		inline bool IsDirty(uint8_t flag) const
		{
			return (m_DirtyFlag & flag) != 0;
		}

		/**
		 * \brief	设置对应标记的数据是脏数据
		 *			0x01: 世界空间下的位置
		 *			0x02: 世界空间下的旋转
		 *			0x04: 局部空间下的位置
		 *			0x08: 局部空间下的旋转
		 *			0x10: 局部空间下的缩放
		 * 			0x20: 变换矩阵
		 * \param	flag		标记类型
		 * \param	isDirty		是否是脏数据
		 */
		inline void SetDirty(uint8_t flag, bool isDirty)
		{
			if (isDirty)
			{
				m_DirtyFlag |= flag;
			}
			else
			{
				m_DirtyFlag &= ~flag;
			}
		}

		/**
		 * \brief	刷新变换矩阵
		 */
		void RefreshMatrix();

		/**
		 * \brief	添加子节点
		 * \param	pChild		子节点指针
		 */
		void AddChild(Transform* pChild)
		{
			m_Children.push_back(pChild);
		}

		/**
		 * \brief	删除子节点
		 * \param	pChild		子节点指针
		 */
		void RemoveChild(Transform* pChild)
		{
			m_Children.erase(std::find(m_Children.begin(), m_Children.end(), pChild));
		}

	private:
		/**
		 * \brief	world position
		 */
		Vector3 m_Position;
		
		/**
		 * \brief	local position
		 */
		Vector3 m_LocalPosition;

		/**
		 * \brief	world rotation
		 */
		Quaternion m_Rotation;
		
		/**
		 * \brief	local rotation
		 */
		Quaternion m_LocalRotation;

		/**
		 * \brief	local scale
		 */
		Vector3 m_LocalScale;

		/**
		 * \brief	局部到世界坐标变换矩阵
		 */
		Matrix4x4 m_LocalToWorldMatrix;

		/**
		 * \brief	世界到局部坐标变换矩阵
		 */
		Matrix4x4 m_WorldToLocalMatrix;

		/**
		 * \brief	数据是否有变化
		 */
		uint8_t m_DirtyFlag = 0x20;

		/**
		 * \brief	父节点
		 */
		Transform* m_Parent = nullptr;

		/**
		 * \brief	所有子节点
		 */
		std::vector<Transform*> m_Children;
	};
}