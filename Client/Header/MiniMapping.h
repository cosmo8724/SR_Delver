#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcCol;
class CTransform;
END

class CMiniMapping : public Engine::CGameObject
{
private:
	explicit CMiniMapping(LPDIRECT3DDEVICE9	pGraphicDev);
	virtual ~CMiniMapping();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);
	HRESULT				Set_Color();

public:
	static CMiniMapping*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pObj);
	virtual void				Free(void);

private:
	CRcCol*		m_pBufferCom = nullptr;
	CTransform*	m_pTransCom = nullptr;
	CGameObject* m_pObject = nullptr;

	D3DXCOLOR	m_Color = { 0.f, 0.f, 0.f, 0.f };
};

