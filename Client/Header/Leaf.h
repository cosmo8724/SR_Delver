#pragma once
#include "Monster.h"

class CLeaf : public CMonster
{
	enum STATE { IDLE, LEAF, ATTACK, HIT, DIE, MOTION_END };
	enum SKILL { SKILL_BULLET, SKILL_STUN, SKILL_FLOOR, SKILL_END };

private:
	explicit CLeaf(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLeaf();

public:
	virtual HRESULT		Ready_Object(void)						override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void)					override;
	virtual void		Render_Obejct(void)						override;

private:
	virtual HRESULT		Add_Component(void) override;
	
	void				SKill_Update(const _float & fTimeDelta);
	void				SKillTeleporting_Update(const _float& fTimeDelta);
	void				Teleporting(_float fPlayerPosX, _float fPlayerPosZ);
	void				SKillStun_Update(const _float& fTimeDelta);
	void				SKillFloor_Update(const _float& fTimeDelta);

	void				Motion_Change(const _float& fTimeDelta);

private:
	STATE				m_ePreState;
	STATE				m_eCurState;

	SKILL				m_eSkill;

	_vec3				m_OriginalPos;

	// Timer
	_float				m_fTimeAcc ;
	_float				m_fTeleportingTimeAcc;
	_float				m_fAttackTimeAcc;

public:
	static CLeaf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);
};
