#pragma once
#include <memory>
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(std::weak_ptr<GameObject> owner);
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;


		virtual void Update(float) {}
		virtual void FixedUpdate() {}
		virtual void Render() const {}

	protected:
		std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; }

	private:
		std::weak_ptr<GameObject> m_pOwner{};

	};
}

