#pragma once
#include "EcoObject.h"

class CEcoWeb : public CEcoObject
{
private:
	explicit CEcoWeb(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CEcoWeb(const CEcoObject& rhs);
	virtual ~CEcoWeb();


public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CEcoWeb*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CEcoWeb*		Create(CEcoObject* pEcoObject);
	virtual void			Free(void);
};

