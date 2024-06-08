#pragma once
#include "BaseComponent.h"
#include <vector>
#include <algorithm>
#include <memory>
#include "GameObject.h"
#include "TransformComponent.h"

namespace dae
{
    class FOVComponent : public BaseComponent
    {
    public:
        FOVComponent(std::weak_ptr<GameObject> pOwner, float viewAngle, float viewDistance);

        void Update(float deltaTime) override;

        bool IsPlayerInSight() const;
        std::shared_ptr<GameObject> GetPlayerInVision() const;

    private:
        float m_ViewDistance;
        float m_ViewAngle;
        bool m_PlayerInSight;
        float m_Angle;
        std::weak_ptr<GameObject> m_pPlayerInVision;
        std::shared_ptr<TransformComponent> m_pOwnerTransform;
        std::shared_ptr<TransformComponent> m_pPlayerTransform;
    };
}
