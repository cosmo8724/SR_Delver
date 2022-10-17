#pragma once
#include "EcoObject.h"

class CStatue : public CEcoObject
{
private:
	explicit CStatue(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CStatue(const CEcoObject& rhs);
	virtual ~CStatue();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CStatue*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CStatue*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);
};

