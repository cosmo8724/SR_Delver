#pragma once
#include "EcoObject.h"

class CJam : public CEcoObject
{
private:
	explicit CJam(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CJam(const CEcoObject& rhs);
	virtual ~CJam();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CJam*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CJam*		Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
};

