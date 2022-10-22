#pragma once
#include "Monster.h"

class CSongBoss : public CMonster
{
	enum STATE { IDLE, MOVE, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_BULLET, SKILL_STUN, SKILL_FLOOR, SKILL_END };

private:
	explicit CSongBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSongBoss(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CSongBoss(const CMonster& rhs);
	virtual ~CSongBoss();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

public:
	void				Set_StunCount() { m_iStunCount += 1; }
	_int				Get_StunCount() { return m_iStunCount; }

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				SKill_Update(const _float & fTimeDelta);
	void				SKillBullet_Update(const _float& fTimeDelta);
	void				SKillStun_Update(const _float& fTimeDelta);
	void				SKillFloor_Update(const _float& fTimeDelta);

	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;

	_bool				m_bMoveAni = false;
	_bool				m_bAni = false;

	// rand
	_int				m_bSkillBullet = 3;
	_bool				m_bSKill = false;
	_bool				m_bSkillStun = false;
	_bool				m_bSkillFloor = false;
	_float				m_fSkillTimeAcc = 0.f;

	// Skill_Bullet
	_bool				m_bBullet = false;

	// Skill_Stun
	_bool				m_bStun = false;
	_int				m_iStunCount = 0;
	_int				m_iStunCreate = 0;
	_float				m_fStunTimeAcc = 0.f;

	// Skill_Floor
	_bool				m_bFloor = false;
	_bool				m_bFloorOneCheck = false;
	_float				m_fFloorTimeAcc = 0.f;
	_int				m_iFloorCreate = 0;
	_int				m_iLightningCreate = 0;

	// Timer
	_float				m_fTimeAcc;
	_float				m_fIdleTimeAcc;
	_float				m_fAttackTimeAcc;
	_float				m_fMoveTimeAcc;

public:
	static CSongBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
	static CSongBoss*		Create(CMonster* pMonster);
	virtual void			Free(void);
};
