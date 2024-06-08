#pragma once
#include <memory>
#include <map>
#include <vector>
#include "GamePad.h"
#include "Command.h"
#include "Singleton.h"
#include "KeyBoard.h"
#include "glm/vec2.hpp"

namespace dae
{
    enum class KeyState
    {
        keyPressed,
        keyUp,
        keyDown
    };

    struct CommandData
    {
        CommandData()
            : command(nullptr), inputType(KeyState::keyPressed), canBeDeleted(true) {}

        CommandData(std::unique_ptr<Command> pCmd, KeyState state, bool deletable = true)
            : command(std::move(pCmd)), inputType(state), canBeDeleted(deletable) {}

        std::unique_ptr<Command> command;
        KeyState inputType;
        bool canBeDeleted;
    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();

        bool ProcessInput(float deltaTime);

        void SetGamePadCommand(GamePad::ControllerButton button, Command* pCommand, KeyState inputType, bool canBeDeleted = false);
        void SetGamePadStickCommand(GamePad::ControllerStick button, Command* pCommand, bool canBeDeleted = false);
        void SetKeyboardCommand(SDL_Scancode key, Command* pCommand, KeyState inputType, bool canBeDeleted = false);

        glm::vec2 GetControllerStickValues(GamePad::ControllerStick stick) const;

        void ClearInputs();

        void HandleTextInputEvent(const SDL_TextInputEvent& e);
        std::string GetTypedString() const;
    private:
        KeyState m_InputType = KeyState::keyDown;
        std::vector<std::unique_ptr<GamePad>> m_pControllers;
        std::vector<std::unique_ptr<Keyboard>> m_pKeyboards;

        std::map<GamePad::ControllerButton, CommandData> m_GamePadCommands;
        std::map<GamePad::ControllerStick, CommandData> m_GamePadStickCommands;
        std::map<SDL_Scancode, CommandData> m_KeyboardCommands;

        std::string m_TypedString;
    };
}
