#pragma once
#include "GameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
	class CAnimator;
}

class CCat : public CGameObject
{
	enum STATE { IDLE, WALKING, HILL, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CCat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCat(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CCat();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;
	virtual void		CollisionEvent(CGameObject* pObj) override;

private:
	HRESULT				Add_Component(void);

	void				Billboard();
	void				Motion_Change();

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CAnimator*			m_pAnimtorCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	
	_vec3				m_vDirection;
	_bool				m_bWireFrame = false;
	_int				m_iTextureIndex = 0;

private:
	_matrix				m_matOldBill;

	STATE				m_ePreState;
	STATE				m_eCurState;

	// Info
	_vec3				m_vPos;
	_uint				m_tHp;
	_uint				m_tAttack;
	_float				m_fSpeed;

	// Rotation
	_float				m_fAngle = 0.f;
	_matrix				m_matWorld;


public:
	static CCat*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};