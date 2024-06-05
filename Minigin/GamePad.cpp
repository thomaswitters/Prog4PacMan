#include "GamePad.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

namespace dae {
    class GamePad::GamePadImpl {
    public:
        GamePadImpl(unsigned int index);
        void Update();

        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsButtonPressed(unsigned int button) const;

        float GetLeftThumbstickX() const;
        float GetLeftThumbstickY() const;
        float GetRightThumbstickX() const;
        float GetRightThumbstickY() const;
    private:
        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};

        WORD m_PressedThisFrame;
        WORD m_ReleasedThisFrame;

        unsigned int m_GamePadIndex{ 0 };

        float m_LeftThumbstickX{};
        float m_LeftThumbstickY{};
        float m_RightThumbstickX{};
        float m_RightThumbstickY{};
    };

    GamePad::GamePad(unsigned int index) {
        pImpl = new GamePadImpl(index);
    }

    GamePad::~GamePad() {
        delete pImpl;
    }

    void GamePad::Update() {
        pImpl->Update();
    }

    bool GamePad::IsButtonDown(ControllerButton button) {
        return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
    }

    bool GamePad::IsButtonUp(ControllerButton button) {
        return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
    }

    bool GamePad::IsButtonPressed(ControllerButton button) {
        return pImpl->IsButtonPressed(static_cast<unsigned int>(button));
    }

    float GamePad::GetLeftThumbstickX() const {
        return pImpl->GetLeftThumbstickX();
    }

    float GamePad::GetLeftThumbstickY() const {
        return pImpl->GetLeftThumbstickY();
    }

    float GamePad::GetRightThumbstickX() const {
        return pImpl->GetRightThumbstickX();
    }

    float GamePad::GetRightThumbstickY() const {
        return pImpl->GetRightThumbstickY();
    }


    GamePad::GamePadImpl::GamePadImpl(unsigned int index)
        : m_GamePadIndex{ index }, m_PressedThisFrame{}, m_ReleasedThisFrame{} {
        ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    }

    void GamePad::GamePadImpl::Update() {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_GamePadIndex, &m_CurrentState);

        auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        m_PressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
        m_ReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

        m_LeftThumbstickX = m_CurrentState.Gamepad.sThumbLX;
        m_LeftThumbstickY = m_CurrentState.Gamepad.sThumbLY;
        m_RightThumbstickX = m_CurrentState.Gamepad.sThumbRX;
        m_RightThumbstickY = m_CurrentState.Gamepad.sThumbRY;
    }

    bool GamePad::GamePadImpl::IsDownThisFrame(unsigned int button) const {
        return m_PressedThisFrame & static_cast<int>(button);
    }

    bool GamePad::GamePadImpl::IsUpThisFrame(unsigned int button) const {
        return m_ReleasedThisFrame & static_cast<int>(button);
    }

    bool GamePad::GamePadImpl::IsButtonPressed(unsigned int button) const {
        return m_CurrentState.Gamepad.wButtons & static_cast<int>(button);
    }

    float GamePad::GamePadImpl::GetLeftThumbstickX() const {
        return m_LeftThumbstickX;
    }

    float GamePad::GamePadImpl::GetLeftThumbstickY() const {
        return m_LeftThumbstickY;
    }

    float GamePad::GamePadImpl::GetRightThumbstickX() const {
        return m_RightThumbstickX;
    }

    float GamePad::GamePadImpl::GetRightThumbstickY() const {
        return m_RightThumbstickY;
    }
}


