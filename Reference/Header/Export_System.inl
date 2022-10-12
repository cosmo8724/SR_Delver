HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void			Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void			Render_End(HWND hWnd)
{
	CGraphicDev::GetInstance()->Render_End(hWnd);
}

// InputDev

_byte		Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
{
	return CInputDev::GetInstance()->Get_DIMouseState(byMouseID);
}
 
_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMoveState);
}

_bool		Mouse_Down(MOUSEKEYSTATE _MouseButton)
{
	return CInputDev::GetInstance()->Mouse_Down(_MouseButton);
}

_bool		Key_Down(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Key_Down(byKeyID);
}

HRESULT		Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

void		LateUpdate_InputDev()
{
	CInputDev::GetInstance()->LateUpdate_InputDev();
}

void			SetUp_InputDev(void)
{
	CInputDev::GetInstance()->SetUp_InputDev();
 }

_bool		Is_DoubleClicked(MOUSEKEYSTATE _MouseButton, _float fDeltaTime)
{
	return CInputDev::GetInstance()->Is_DoubleClicked(_MouseButton, fDeltaTime);
}


_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}
void			Update_Timer(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Update_Timer(pTimerTag);
}

_bool		IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// FontMgr

HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,	 // 키값
	const _tchar* pFontType, // 서체
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

// SoundMgr
HRESULT	Ready_Sound()
{
	return CSoundMgr::GetInstance()->Ready_Sound();
}

HRESULT	Play_Sound(const _tchar* pSoundKey, CHANNELID eID, _float fVolume)
{
	return CSoundMgr::GetInstance()->Play_Sound(pSoundKey, eID, fVolume);
}

HRESULT	PlayBGM(const _tchar* pSoundKey, _float fVolume)
{
	return CSoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}

HRESULT	StopSound(CHANNELID eID)
{
	return CSoundMgr::GetInstance()->StopSound(eID);
}

HRESULT	StopAllSound()
{
	return CSoundMgr::GetInstance()->StopAllSound();
}

void			Set_ChannelVolume(CHANNELID eID, _float fVolume)
{
	CSoundMgr::GetInstance()->Set_ChannelVolume(eID, fVolume);
}

inline void			Release_System(void)
{
	CSoundMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}
