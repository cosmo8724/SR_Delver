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

public:
	void				Reset();
	void				Set_Fire(_bool _bFire) { m_bFire = _bFire; m_bDead = false; }

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void		Target_Follow(const _float& fTimeDelta) override;

	void				Skill_Update(const _float& fTimeDelta);
	void				SkillMove_Update(const _float& fTimeDelta);
	void				SkillAttack_Update(const _float& fTimeDelta);
	void				SkillAnger_Update(const _float& fTimeDelta);



	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;

	_uint				m_iTransparency;

	// 분신 변수
	_int				m_iAngerCount = 0;
	_float				m_fFrame = 0.f;
	_bool				m_bDead = false;
	_bool				m_bFire = false;
	_bool				m_bReady = false;
	_float				m_fLifeTime = 0.f;

	//Timer
	_float				m_fTimeAcc;
	_float				m_fTransparencyTimeAcc;
	_float				m_AngerTimeAcc;


public:
	static CStick*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iAngerCount);
	virtual void		Free(void);
};

END