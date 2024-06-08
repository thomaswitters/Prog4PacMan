#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <string>
#include "Texture2D.h"
#include "GameObject.h"


dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> pOwner, const std::string& filename) : BaseComponent(pOwner)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> pOwner, std::shared_ptr<Texture2D> pTexture) : BaseComponent{ pOwner }
{
	m_pTexture = pTexture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_pPreviousTexture = m_pTexture; 
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

void dae::RenderComponent::Render() const
{
	auto owner = GetOwner();
	if (owner.lock() == nullptr)
	{
		return;
	}
	auto transform = owner.lock()->GetComponent<TransformComponent>();
	if (transform == nullptr)
	{
		return;
	}
	glm::vec3 pos = transform->GetWorldPosition();
	float rotationAngle = transform->GetRotation();
	if (m_pTexture != nullptr)
	{
		//auto size = glm::vec2(m_Texture.get()->GetSize().x, m_Texture.get()->GetSize().y) * 2.f;
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, rotationAngle);
	}
}

void dae::RenderComponent::SetPosition(float x, float y)
{
	const auto owner = GetOwner();
	if (owner.lock() == nullptr)
	{
		return;
	}
	auto transform = owner.lock()->GetComponent<TransformComponent>();
	if (transform == nullptr)
	{
		return;
	}

	glm::vec3 pos{ x,y,0.f };
	transform->SetLocalPosition(pos);
}