#pragma once
#include "Equipment.h"

class CPants : public CEquipment
{
private:
	explicit CPants(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId);
	virtual ~CPants();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CPants*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

public:
	_int				Get_TexturId() { return m_iTextureType; }

private:
	_vec3				m_vPos;
	_float				m_fDotTime = 0.f;
	_int				m_iDot = 0;
	_int				m_iTextureType;
};

