#pragma once
#include "Monster.h"

class CSongBoss : public CMonster
{
	enum STATE { IDLE, MOVE, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_BULLET, SKILL_STUN, SKILL_FLOOR, SKILL_END };

private:
	explicit CSongBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSongBoss();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

public:
	void				Set_StunCount() { m_iStunCount += 1; }

private:
	virtual HRESULT		Add_Component(void)						override;
	//virtual void		Dead()									override;

	void				SKill_Update(const _float & fTimeDelta);
	void				SKillBullet_Update(const _float& fTimeDelta);
	void				SKillStun_Update(const _float& fTimeDelta);
	void				SKillFloor_Update(const _float& fTimeDelta);

	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;

	_int				m_iStunCount = 0;
	_float				m_fStunTimeAcc = 0.f;

	// Timer
	_float				m_fTimeAcc ;
	_float				m_fIdleTimeAcc;
	_float				m_fAttackTimeAcc;
	_float				m_fLightningTimeAcc = 0.f;

public:
	static CSongBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};
