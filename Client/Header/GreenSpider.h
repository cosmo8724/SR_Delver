#pragma once
#include "Monster.h"

BEGIN(Engine)

class CCalculator;

class CGreenSpider : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CGreenSpider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenSpider(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CGreenSpider(const CMonster& rhs);
	virtual ~CGreenSpider();

public:
	virtual HRESULT		Ready_Object()							override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		Target_Follow(const _float& fTimeDelta) override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_bool				m_bParticle;

	//Timer
	_float				m_fIdleTimeAcc;
	_float				m_fAttackTimeAcc;

public:
	static CGreenSpider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CGreenSpider*		Create(CMonster* pMonster);
	virtual void		Free(void);
};

END