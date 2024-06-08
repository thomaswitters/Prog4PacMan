#include "TypeNameComponent.h"


dae::TypeNameComponent::TypeNameComponent(std::weak_ptr<GameObject> pOwner)
	: BaseComponent(pOwner)
	, m_Name()
{
	m_pTextChanged = std::make_unique<Subject>();
}

void dae::TypeNameComponent::Update(float)
{
}

void dae::TypeNameComponent::ChangeText()
{
	m_pTextChanged.get()->Notify(*GetOwner().lock(), Event::TypeName);
}

std::string dae::TypeNameComponent::GetName() const
{
	return m_Name;
}

dae::Subject* dae::TypeNameComponent::GetNameSubject() const
{
	return m_pTextChanged.get();
}