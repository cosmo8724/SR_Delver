#pragma once
#include "Item.h"

class CEquipment : public CItem
{
protected:
	explicit CEquipment(LPDIRECT3DDEVICE9 pGraphicDev) : CItem(pGraphicDev) {};
	virtual ~CEquipment() {};

public:
	virtual HRESULT Ready_Object(void) { return S_OK; }
	virtual _int	Update_Object(const _float& fTimeDelta);
	virtual void	LateUpdate_Object(void);
	virtual	void	Render_Obejct(void) {}

	virtual void	CollisionEvent(CGameObject * pOtherObj);
	virtual void	InteractEvent();
protected:
	virtual void		Free(void) {}

protected:
	_bool		m_bReady = false;
	_matrix		m_matView;
	_vec3		m_vPos;
};

