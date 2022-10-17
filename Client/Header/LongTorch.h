#pragma once
#include "EcoObject.h"

class CLongTorch : public CEcoObject
{
private:
	explicit CLongTorch(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CLongTorch(const CEcoObject& rhs);
	virtual ~CLongTorch();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CLongTorch*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CLongTorch*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);

private:
	_float		m_fFrame = 0.f;
};

