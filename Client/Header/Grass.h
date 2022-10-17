#pragma once
#include "EcoObject.h"

class CGrass : public CEcoObject
{
private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CGrass(const CEcoObject& rhs);
	virtual ~CGrass();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CGrass*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CGrass*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);
};

