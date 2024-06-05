#pragma once
#include <iostream>
#include "GameObject.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Execute(float deltaTime) = 0;
		virtual void Undo(float deltaTime);
	};

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<GameObject> object, float speed, glm::f32vec2 direction, bool useStickDir = false);

		void Execute(float deltaTime) override;
	private:
		std::weak_ptr<GameObject> m_pObject;
		float m_Speed;
		glm::f32vec2 m_Direction;
		bool m_UseStickDir;
	};

	class AddPointsCommand final : public Command
	{
	public:
		AddPointsCommand(std::shared_ptr<GameObject> object, int amount);

		void Execute(float deltaTime) override;
	private:
		std::weak_ptr<GameObject> m_pObject;
		int AmountPoints;
	};

	class RemoveHealthCommand final : public Command
	{
	public:
		RemoveHealthCommand(std::shared_ptr<GameObject> object);

		void Execute(float deltaTime) override;
	private:
		std::weak_ptr<GameObject> m_pObject;
	};
}
