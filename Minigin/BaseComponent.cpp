#include "BaseComponent.h"

using namespace dae;

BaseComponent::BaseComponent(std::weak_ptr<GameObject> owner) :
	m_pOwner(owner)
{}
