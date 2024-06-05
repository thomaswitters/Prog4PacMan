#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <string>
#include "Texture2D.h"
#include "GameObject.h"


dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> owner, const std::string& filename) : BaseComponent(owner)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> owner, std::shared_ptr<Texture2D> texture) : BaseComponent{ owner }
{
	m_Texture = texture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
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
	if (m_Texture != nullptr)
	{
		//auto size = glm::vec2(m_Texture.get()->GetSize().x, m_Texture.get()->GetSize().y) * 2.f;
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
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