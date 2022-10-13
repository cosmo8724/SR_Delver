#pragma once
#include "Monster.h"

BEGIN(Engine)

class CCalculator;

class CStick : public CMonster
{
	enum STATE { IDLE, ANGER, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_MOVE, SKILL_ATTACK, SKILL_ANGER, SKILL_END };

private:
	explicit CStick(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStick();

public:
	virtual HRESULT		Ready_Object(_int iAngerCount);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		Target_Follow(const _float& fTimeDelta) override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				Skill_Update(const _float& fTimeDelta);
	void				SkillMove_Update(const _float& fTimeDelta);
	void				SkillAttack_Update(const _float& fTimeDelta);
	void				SkillAnger_Update(const _float& fTimeDelta);
	void				Teleporting(const _float& fPlayerPosX, const _float& fPlayerPosZ);

	void				Motion_Change();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;

	_vec3				m_OriginalPos;

	_bool				m_bParticle = false;

	//Timer
	_float				m_fMoveTimeAcc;

public:
	static CStick*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iAngerCount);
	virtual void		Free(void);
};

END