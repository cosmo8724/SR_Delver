#pragma once
#include "Monster.h"
#include "Engine_Include.h"

class CJellyfish : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CJellyfish(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJellyfish(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CJellyfish(const CMonster& rhs);
	virtual ~CJellyfish();

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
	static CJellyfish*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CJellyfish*	Create(CMonster* pMonster);
	virtual void		Free(void);
};

