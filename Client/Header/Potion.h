#pragma once
#include "Item.h"

class CPotion : public CItem
{
private:
	explicit CPotion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPotion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CPotion(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual ~CPotion();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);
	void				Billboard();

public:
	static CPotion*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);


private:
	_vec3				m_vPos;
	_matrix				m_matWorld;
	_int				m_iTextureIdx = 0;
	_float				m_fDotTime = 0.f;
	_int				m_iDot = 0;
};

