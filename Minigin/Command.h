#pragma once
#include <iostream>
#include "GameObject.h"
#include "PacManMoveComponent.h"
#include "PointsComponent.h"
#include "HealthComponent.h"
#include "GameMode.h"

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


	class ChangeMoveDirCommand final : public Command
	{
	public:
		ChangeMoveDirCommand(std::shared_ptr<GameObject> object, PacManMoveComponent::Movement movement, float angle);

		void Execute(float deltaTime) override;
	private:
		std::weak_ptr<GameObject> m_pObject;
		float m_Angle;
		std::shared_ptr<PacManMoveComponent> m_pMoveComponent;
		std::shared_ptr<TransformComponent> m_pTransformComponent;
		PacManMoveComponent::Movement m_Movement;
	};

	class AddPointsCommand final : public Command
	{
	public:
		AddPointsCommand(std::shared_ptr<GameObject> object, int amount);

		void Execute(float deltaTime) override;
	private:
		std::shared_ptr<GameObject> m_pObject;
		int AmountPoints;
		std::shared_ptr<PointsComponent> m_pPoints;
	};

	class RemoveHealthCommand final : public Command
	{
	public:
		RemoveHealthCommand(std::shared_ptr<GameObject> object);

		void Execute(float deltaTime) override;
	private:
		std::weak_ptr<GameObject> m_pObject;
		std::shared_ptr<HealthComponent> m_pHealth;
	};

	class SwitchBetweenGameModesCommand final : public Command
	{
	public:
		SwitchBetweenGameModesCommand(std::shared_ptr<GameObject> object);

		void Execute(float deltaTime) override;

		std::string GetStartLevel()
		{
			return m_StartString;
		}
	private:
		std::weak_ptr<GameObject> m_pObject;
		std::shared_ptr<RenderComponent> m_pRenderComponent;
		std::string m_StartString;
		static int m_CurrentLevelIndex;
	};

	class StartGame final : public Command
	{
	public:
		StartGame(std::vector<std::shared_ptr<dae::GameMode>> gameModes, SwitchBetweenGameModesCommand& switchCommand);

		void Execute(float deltaTime) override;
	private:
		std::vector<std::shared_ptr<dae::GameMode>> m_GameModes;
		std::string m_StartString;
		SwitchBetweenGameModesCommand& m_SwitchCommand;
	};
	
}
