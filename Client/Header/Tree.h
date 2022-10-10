#pragma once
#include "EcoObject.h"
class CTree :	public CEcoObject
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CTree(const CEcoObject& rhs);
	virtual ~CTree();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			LateUpdate_Object(void) override;
	virtual void			Render_Obejct(void) override;

private:
	HRESULT				Add_Component(void);

public:
	static CTree*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CTree*		Create(CEcoObject* pEcoObject);
	virtual void		Free(void);
};

