#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class TypeNameComponent final : public BaseComponent
	{
	public:
		TypeNameComponent(std::weak_ptr<GameObject> pOwner);
		~TypeNameComponent() = default;

		TypeNameComponent(const TypeNameComponent&) = delete;
		TypeNameComponent(TypeNameComponent&&) = delete;
		TypeNameComponent& operator= (const TypeNameComponent&) = delete;
		TypeNameComponent& operator= (const TypeNameComponent&&) = delete;

		void Update(float deltaTime) override;
		void ChangeText();

		std::string GetName() const;
		Subject* GetNameSubject() const;
	private:
		std::unique_ptr<Subject> m_pTextChanged;

		std::string m_Name;
	};
}

