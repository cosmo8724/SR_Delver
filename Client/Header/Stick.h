#pragma once
#include "Monster.h"

BEGIN(Engine)

class CCalculator;

class CStick : public CMonster
{
	enum STATE { IDLE, ANGER, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CStick(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStick(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CStick(const CMonster& rhs);
	virtual ~CStick();

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
	_float				m_fMoveTimeAcc;
	_float				m_fParticleTimeAcc = 0.f;

public:
	static CStick*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CStick*		Create(CMonster* pMonster);
	virtual void		Free(void);
};

END