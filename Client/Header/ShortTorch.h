#pragma once
#include "EcoObject.h"

class CShortTorch : public CEcoObject
{
private:
	explicit CShortTorch(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CShortTorch(const CEcoObject& rhs);
	virtual ~CShortTorch();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CShortTorch*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CShortTorch*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);

private:
	_float		m_fFrame = 0.f;
};

