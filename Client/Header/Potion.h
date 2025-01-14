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
	void				Set_Potion() { m_bPotion = true; }

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
	virtual void		InteractEvent();


public:
	_int		Get_TexturId() { return m_iTextureIdx; }
	void		Cal_Cnt(_int iCnt) { m_iCnt += iCnt; m_bFinished = false; 
					m_iDot = 1; m_bPotion = false; m_fDotTime = 0.f;  }
	_int		Get_Cnt() { return m_iCnt; }
	void		RandomItem(const _float& fTimeDelta);

private:
	_vec3				m_vPos;
	_int				m_iTextureIdx = 0;
	_float				m_fDotTime = 0.f;
	_int				m_iDot = 1;
	_int				m_iCnt = 1;

	// potionType
	_int				m_tPotionType = 0;
	_float				m_fItemTimeAcc = 0.f;
	_bool				m_bPotion = false;

	_bool				m_bFinished = false;

	_float				m_fParticle = 0.f;

	_bool Test = false;
};

