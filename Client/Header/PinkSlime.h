#pragma once
#include "Monster.h"

class CPinkSlime : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_JUMP, SKILL_FOLLOW, SKILL_SCALE, SKILL_END };
	enum SKILLSCALE { SKILLSCALE_BIG, SKILLSCALE_MEDIUM, SKILLSCALE_SMALL, SKILLSCALE_END };

private:
	explicit CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPinkSlime(const CPinkSlime& rhs);
	virtual ~CPinkSlime();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void) override;

	void				SKill_Update(const _float & fTimeDelta);
	void				SKillJump_Update(const _float& fTimeDelta);
	void				SKillFollow_Update(const _float& fTimeDelta, _float fDist, _vec3* vPlayerPos);
	void				SKillScale_Update(const _float& fTimeDelta);


	void				Motion_Change();

private:
	CGameObject*		pGameObject = nullptr;

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;
	SKILLSCALE			m_eSkill_Scale;

	_float				m_fScale;
	_float				m_fHeight;

	_bool				m_bHit = false;

	// 스킬 순서
	_float				m_bSkillJumpStart = false;
	_float				m_bSkillFollowStart = false;
	_float				m_SkillJumpTimeAcc = 0.f;

	// jump
	_bool				m_bJump;
	_float				m_fJSpeed; 			// 점프 속도
	_float				m_fJSpeed0; 		// 점프 초기 속도
	_float				m_fAccel;			// 중력가속도

	// Timer
	_float				m_fTimeAcc;
	_float				m_fJumpTimeAcc;
	_float				m_fHitTimeAcc = 0.f;

	// 중간 생성 관련 변수
	_float				m_fTime = 0.f; 
	_bool				m_bClone = false;

public:
	static CPinkSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPinkSlime*	Create(const CPinkSlime& rhs);
	virtual void		Free(void);
};


