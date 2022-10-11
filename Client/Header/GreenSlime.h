#pragma once
#include "Monster.h"

class CGreenSlime : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGreenSlime();

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void		Target_Follow(const _float& fTimeDelta);
	virtual void		OnHit(const _float& fTimeDelta);

	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	//_bool				m_bHit = false;
	//_float				m_fHitTimeAcc = 0.f;

	// Timer
	_float				m_fTimeAcc;

public:
	static CGreenSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};


