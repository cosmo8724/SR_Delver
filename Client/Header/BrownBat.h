#pragma once
#include "Monster.h"
#include "Engine_Include.h"

class CBrownBat : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CBrownBat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBrownBat(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CBrownBat(const CMonster& rhs);
	virtual ~CBrownBat();

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

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_float				m_fHeight;


	// Timer
	_float				m_fTimeAcc;
	_float				m_fAttackTimeAcc;

public:
	static CBrownBat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CBrownBat*	Create(CMonster* pMonster);
	virtual void		Free(void);
};

