#include "GamePad.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

class dae::GamePad::GamePadImpl
{
public:
	GamePadImpl(unsigned int controllerIndex);

	void Update();

	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

	float GetLeftStickX() const;
	float GetLeftStickY() const;
	float GetRightStickX() const;
	float GetRightStickY() const;

private:
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviusState{};

	WORD m_ButtonPressedThisFrame;
	WORD m_ButtonReleasedThisFrame;

	unsigned int m_GamePadIndex{ 0 };

	float m_LeftStickX{};
	float m_LeftStickY{};
	float m_RightStickX{};
	float m_RightStickY{};
};


dae::GamePad::GamePad(unsigned int controllerIndex)
{
	pImpl = new GamePadImpl(controllerIndex);
}

dae::GamePad::~GamePad()
{
	delete pImpl;
}

void dae::GamePad::Update()
{
	pImpl->Update();
}

bool dae::GamePad::IsDown(ControllerButton button)
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::GamePad::IsUp(ControllerButton button)
{
	return pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool dae::GamePad::IsPressed(ControllerButton button)
{
	return pImpl->IsPressed(static_cast<unsigned int>(button));
}

float dae::GamePad::GetLeftStickX() const
{
	return pImpl->GetLeftStickX();
}
float dae::GamePad::GetLeftStickY() const
{
	return pImpl->GetLeftStickY();
}
float dae::GamePad::GetRightStickX() const
{
	return pImpl->GetRightStickX();
}
float dae::GamePad::GetRightStickY() const
{
	return pImpl->GetRightStickY();
}

// GamePadImpl
dae::GamePad::GamePadImpl::GamePadImpl(unsigned int controllerIndex)
	: m_GamePadIndex{ controllerIndex }
	, m_ButtonPressedThisFrame{}
	, m_ButtonReleasedThisFrame{}
{
	ZeroMemory(&m_PreviusState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
}

void dae::GamePad::GamePadImpl::Update()
{
	CopyMemory(&m_PreviusState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_GamePadIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons xor m_PreviusState.Gamepad.wButtons; // ^ xor
	m_ButtonPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons); // ~invert

	m_LeftStickX = m_CurrentState.Gamepad.sThumbLX;
	m_LeftStickY = m_CurrentState.Gamepad.sThumbLY;
	m_RightStickX = m_CurrentState.Gamepad.sThumbRX;
	m_RightStickY = m_CurrentState.Gamepad.sThumbRY;
}

bool dae::GamePad::GamePadImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonPressedThisFrame & static_cast<int>(button);
}

bool dae::GamePad::GamePadImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonReleasedThisFrame & static_cast<int>(button);
}

bool dae::GamePad::GamePadImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & static_cast<int>(button);
}

float dae::GamePad::GamePadImpl::GetLeftStickX() const 
{ 
	return m_LeftStickX; 
}

float dae::GamePad::GamePadImpl::GetLeftStickY() const
{ 
	return m_LeftStickY; 
}

float dae::GamePad::GamePadImpl::GetRightStickX() const 
{ 
	return m_RightStickX; 
}

float dae::GamePad::GamePadImpl::GetRightStickY() const 
{ 
	return m_RightStickY; 
}


