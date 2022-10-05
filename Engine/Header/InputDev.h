#pragma once

#include "Base.h"
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev :	public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long*)&m_MouseState) + eMoveState);
	}
	_bool Mouse_Down(MOUSEKEYSTATE _MouseButton);
	_bool Key_Down(_ubyte byKeyID);
	_bool Is_DoubleClicked(MOUSEKEYSTATE _MouseButton, _float fDeltaTime);


public:
	HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void				LateUpdate_InputDev();
	void				SetUp_InputDev(void);

private:
	_byte			m_byKeyState[256];
	_bool			m_bKeyState[256];
	_bool			m_bMouseState[4];
	DIMOUSESTATE	m_MouseState;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

	LPDIRECTINPUT8				m_pInputSDK;

	_int		m_iClickedCnt = 0;
	_float		m_fDeltaTime = 0.f;

public:
	virtual void		Free(void);
};

END