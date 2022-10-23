#pragma once
#include "Item.h"

class CKey : public CItem
{
private:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CKey();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CKey*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);
	virtual void		InteractEvent() override;


private:
	_vec3				m_vPos;
	_matrix				m_matWorld;
};

