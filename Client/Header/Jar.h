#pragma once
#include "EcoObject.h"

class CJar : public CEcoObject
{
private:
	explicit CJar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJar(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CJar(const CEcoObject& rhs);
	virtual ~CJar();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CJar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CJar*		Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

};

