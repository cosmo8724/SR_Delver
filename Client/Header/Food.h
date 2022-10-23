#pragma once
#include "Item.h"

class CFood : public CItem
{
private:
	explicit CFood(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFood(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CFood(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual ~CFood();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual void		CollisionEvent(CGameObject *pObj)		override;
	virtual void		InteractEvent()							override;
	virtual HRESULT		Add_Component(void);

public:
	static CFood*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual void		Free(void);

public:
	_int				Get_TexturId() { return m_iTextureIdx; }


private:
	_vec3				m_vPos;
	_int				m_iTextureIdx = 0;
	_bool				m_bEat = false;
};

