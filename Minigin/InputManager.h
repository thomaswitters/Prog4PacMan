#pragma once
#include <memory>
#include <map>
#include "GamePad.h"
#include "Command.h"
#include "Singleton.h"
#include "KeyBoard.h"

namespace dae
{
	enum class KeyState
	{
		keyPressed,
		keyUp,
		keyDown
	};

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();

        bool ProcessInput(float deltaTime);

        void SetGamePadCommand(GamePad::ControllerButton button, Command* command, KeyState inputType);
        void SetGamePadStickCommand(GamePad::ControllerStick button, Command* command);
        void SetKeyboardCommand(SDL_Scancode key, Command* command, KeyState inputType);

        glm::vec2 GetControllerStickValues(GamePad::ControllerStick stick) const;

    private:
        KeyState m_InputType = KeyState::keyDown;
        std::vector<std::unique_ptr<GamePad>> m_pControllers;
        std::vector<std::unique_ptr<Keyboard>> m_pKeyboards;

        std::map<GamePad::ControllerButton, std::pair<std::unique_ptr<Command>, KeyState>> m_GamePadCommands;
        std::map<GamePad::ControllerStick, std::unique_ptr<Command>> m_GamePadStickCommands;

        std::map<SDL_Scancode, std::pair<std::unique_ptr<Command>, KeyState>> m_KeyboardCommands;
    };
}
