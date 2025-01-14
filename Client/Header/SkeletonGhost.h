#pragma once
#include "Monster.h"
#include "Engine_Include.h"

class CSkeletonGhost : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CSkeletonGhost(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkeletonGhost(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CSkeletonGhost(const CMonster& rhs);
	virtual ~CSkeletonGhost();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		Target_Follow(const _float& fTimeDelta)	override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				Billboard()								override;
	void				Circle();
	
	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_float				m_fHeight;

	_bool				m_bCircle = false;

	_matrix				m_matWorld;
	_vec3				m_vTrans;
	_float				m_fAngle = 0.f;

	// Timer
	_float				m_fTimeAcc;
	_float				m_fAttackTimeAcc;
	_float				m_fHpMinusTimeAcc;

public:
	static CSkeletonGhost*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CSkeletonGhost* Create(CMonster* pMonster);
	virtual void			Free(void);
};

