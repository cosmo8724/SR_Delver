#pragma once
#include "EcoObject.h"

class CEcoMush : public CEcoObject
{
private:
	explicit CEcoMush(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CEcoMush(const CEcoObject& rhs);
	virtual ~CEcoMush();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CEcoMush*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CEcoMush*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);
};

