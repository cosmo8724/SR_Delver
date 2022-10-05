#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCamera;

class ENGINE_DLL CUI : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev) {};
	virtual ~CUI() {};

public:
	virtual HRESULT Ready_Object(void) { return S_OK; }
	virtual _int	Update_Object(const _float& fTimeDelta) { return 0; }
	virtual void	LateUpdate_Object(void) {}
	virtual	void	Render_Obejct(void) {}

protected:
	virtual void	Free(void);

protected:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CTransform*		m_pTransCom = nullptr;

	CCamera*		m_pCamera = nullptr;

protected:
	_float			m_fScaleX = 0.f;
	_float			m_fScaleY = 0.f;
	_float			m_fPosX = 0.f;
	_float			m_fPosY = 0.f;
};

END
