#pragma once
#include "Monster.h"

class CPinkSlime : public CMonster
{
	enum STATE { IDLE, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_JUMP, SKILL_FOLLOW, SKILL_SCALE, SKILL_END };
	enum SKILLSCALE { SKILLSCALE_BIG, SKILLSCALE_MEDIUM, SKILLSCALE_SMALL, SKILLSCALE_END };

private:
	explicit CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPinkSlime(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	explicit CPinkSlime(const CPinkSlime& rhs);
	virtual ~CPinkSlime();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;
	virtual	void		CollisionEvent(CGameObject* pObj)		override;

private:
	virtual HRESULT		Add_Component(void)						override;
	virtual void		OnHit(const _float& fTimeDelta)			override;
	virtual void		Dead()									override;

	void				Motion_Change();

	void				SKill_Update(const _float & fTimeDelta);
	void				SKillJump_Update(const _float& fTimeDelta);
	void				SKillFollow_Update(const _float& fTimeDelta, _float fDist, _vec3* vPlayerPos);
	void				SKillScale_Update(const _float& fTimeDelta);

private:
	CGameObject*		pGameObject = nullptr;

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;
	SKILLSCALE			m_eSkill_Scale;

	_float				m_fScale;
	_float				m_fHeight;

	// ��ų ����
	_bool				m_bSkillJumpStart = false;
	_bool				m_bSkillFollowStart = false;
	_float				m_fSkillJumpTimeAcc = 0.f;

	// jump
	_bool				m_bJump;
	_float				m_fJSpeed; 			// ���� �ӵ�
	_float				m_fJSpeed0; 		// ���� �ʱ� �ӵ�
	_float				m_fAccel;			// �߷°��ӵ�

	// Timer
	_float				m_fTimeAcc;
	_float				m_fJumpTimeAcc;
	_float				m_fHitTimeAcc = 0.f;

	// �߰� ���� ���� ����
	_float				m_fTime = 0.f; 
	_bool				m_bClone = false;
	_bool				m_bDead = false;

public:
	static CPinkSlime*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	static CPinkSlime*	Create(const CPinkSlime& rhs);
	virtual void		Free(void);
};


