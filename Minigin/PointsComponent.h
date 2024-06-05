#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class PointsComponent final : public BaseComponent
	{
	public:
		PointsComponent(std::shared_ptr<GameObject> pOwner, int points = 0);
		~PointsComponent() = default;

		PointsComponent(const PointsComponent&) = delete;
		PointsComponent(PointsComponent&&) = delete;
		PointsComponent& operator= (const PointsComponent&) = delete;
		PointsComponent& operator= (const PointsComponent&&) = delete;

		void Update(float deltaTime) override;
		void AddPoints(int amount);

		int GetPoints();
		Subject* GetPointSubject() const;

	private:
		std::unique_ptr<Subject> m_pPointsChanged;

		int m_Points;
	};
}
