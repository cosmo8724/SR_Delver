#pragma once
#include "Item.h"

class CGold : public CItem
{
private:
	explicit CGold(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CGold(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual ~CGold();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual void		CollisionEvent(CGameObject *pObj)		override;
	virtual HRESULT		Add_Component(void);
	virtual void		InteractEvent() override;


public:
	static CGold*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual void		Free(void);

public:
	_int				Get_TexturId() { return m_iTextureIdx; }


private:
	_vec3				m_vPos;
	_int				m_iTextureIdx = 0;
	_int				m_iGold = 0;
};

