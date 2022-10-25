#pragma once
#include "Equipment.h"

class CRing : public CEquipment
{
private:
	explicit CRing(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId);
	virtual ~CRing();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void);

public:
	static CRing*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int texId);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

public:
	_int				Get_TexturId() { return m_iTextureType; }

private:
	_vec3				m_vPos;
	_float				m_fDotTime = 0.f;
	_int				m_iDot = 0;
	_int				m_iTextureType;

	_vec3				m_vPopAngle;
};

