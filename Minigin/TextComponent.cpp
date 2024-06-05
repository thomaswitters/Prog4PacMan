#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

TextComponent::TextComponent(std::weak_ptr<GameObject> owner, const std::string& text, const std::shared_ptr<Font>& font,
	const SDL_Color& color = { 255, 255, 255, 255 }) :
	BaseComponent(owner),
	m_Text(text),
	m_pFont(std::move(font)),
	m_Color(color),
	m_pTextTexture(nullptr)
{}

void TextComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void TextComponent::Render() const
{
	auto renderComponent = GetOwner().lock()->GetComponent<RenderComponent>();
	if (renderComponent)
	{
		renderComponent->SetTexture(m_pTextTexture);
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}