#pragma once
#include "BaseComponent.h"
#include <memory>
#include "string"
#include "Transform.h"
namespace dae
{
	class Texture2D;
	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(std::weak_ptr<GameObject> pOwner) : BaseComponent(pOwner) {};
		RenderComponent(std::weak_ptr<GameObject> pOwner, std::shared_ptr<Texture2D>texture);
		RenderComponent(std::weak_ptr<GameObject> pOwner, const std::string& filename);
		virtual void Render() const override;
		virtual void Update(float) override {};
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
		void SetPosition(float x, float y);
	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

