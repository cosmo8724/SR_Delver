#pragma once
#include "EcoObject.h"


class CBonFire : public CEcoObject
{
private:
	explicit CBonFire(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CBonFire(const CEcoObject& rhs);
	virtual ~CBonFire();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CBonFire*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CBonFire*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);

private:
	_float		m_fFrame = 0.f;
};

