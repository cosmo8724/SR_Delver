#pragma once
#include "GameObject.h"
#include "Engine_Include.h"
#include "Export_Function.h"

class CLoading_BG :	public CGameObject
{
private:
	explicit CLoading_BG(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading_BG();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	_float				m_fTime = 0.f;

public:
	static CLoading_BG*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

