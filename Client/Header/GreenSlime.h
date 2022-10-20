#pragma once
#include "Monster.h"

class CGreenSlime : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };

private:
	explicit CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenSlime(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual ~CGreenSlime();

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		Target_Follow(const _float& fTimeDelta) override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;
	void				Lock(const _float& fTimeDelta);

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	_bool				m_bLock = false;

	// Timer
	_float				m_fTimeAcc;
	_float				m_fLockTimeAcc = 0.f;

public:
	//static CGreenSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CGreenSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);
};