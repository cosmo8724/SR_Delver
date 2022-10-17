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

public:
	static CPotion*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _eType);
	virtual void		Free(void);
	virtual void		CollisionEvent(CGameObject *pObj);

public:
	_int		Get_TexturId() { return m_iTextureIdx; }
	void		Cal_Cnt(_int iCnt) { m_iCnt += iCnt; }
	_int		Get_Cnt() { return m_iCnt; }

private:
	_vec3				m_vPos;
	_int				m_iTextureIdx = 0;
	_float				m_fDotTime = 0.f;
	_int				m_iDot = 0;
	_int				m_iCnt = 1;
};

