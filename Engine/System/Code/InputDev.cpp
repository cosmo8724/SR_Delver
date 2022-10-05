#include "..\..\Header\InputDev.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev()
	: m_pInputSDK(nullptr), m_pKeyBoard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


CInputDev::~CInputDev()
{
	Free();
}

_bool CInputDev::Mouse_Down(MOUSEKEYSTATE _MouseButton)
{
	if (!m_bMouseState[_MouseButton] && (m_MouseState.rgbButtons[_MouseButton] & 0x80))
	{
		m_bMouseState[_MouseButton] = true;
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_bMouseState[i] && !(m_MouseState.rgbButtons[i] & 0x80))
		{
			m_bMouseState[i] = false;
		}
	}

	return false;
}

_bool CInputDev::Key_Down(_ubyte byKeyID)
{
	if (!m_bKeyState[byKeyID] && Get_DIKeyState(byKeyID) & 0x80)
	{
		m_bKeyState[byKeyID] = true;
		return true;
	}

	return false;
}

_bool CInputDev::Is_DoubleClicked(MOUSEKEYSTATE _MouseButton, _float fDeltaTime)
{
	m_fDeltaTime += fDeltaTime;

	if (!m_bMouseState[_MouseButton] && (m_MouseState.rgbButtons[_MouseButton] & 0x80))
	{
		if ((m_fDeltaTime < 0.5f) && (1 == m_iClickedCnt)) // 1초 이내에 한번 클릭한 전적이 있으면 더블클릭
		{
			m_fDeltaTime = 0.f;	// 조건값들 초기화
			m_iClickedCnt = 0;
			return true;
		}
		else if (m_fDeltaTime > 0.5f)// 일정시간이 지나도 조건값 초기화
		{
			m_fDeltaTime = 0.f;
			m_iClickedCnt = 0;
		}

		m_bMouseState[_MouseButton] = true;
		m_iClickedCnt++;
		return false;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_bMouseState[i] && !(m_MouseState.rgbButtons[i] & 0x80))
		{
			m_bMouseState[i] = false;
		}
	}
	return false;
}

HRESULT CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDev::LateUpdate_InputDev()
{
	for (int i = 0; i < 256; ++i)
	{
		if (m_bKeyState[i] && !(Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = false;

		else if (!m_bKeyState[i] & (Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = true;
	}
}

void CInputDev::SetUp_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
