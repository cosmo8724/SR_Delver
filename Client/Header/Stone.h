#pragma once
#include "EcoObject.h"

class CStone : public CEcoObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CStone(const CEcoObject& rhs);
	virtual ~CStone();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CStone*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);
};

