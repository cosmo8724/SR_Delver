#pragma once
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRcTex;
	class CTexture;
	class CCalculator;
	class CItem;
}
class CBlock;
class CInventory;
class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	HRESULT		Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	void				Set_HpFull()				{ m_tInfo.iHp = m_tInfo.iHpMax; }
	void				Set_HpPlus(_int iHp = 1); 
	void				Set_HpMinus(_int iHp = 1)	{ if (0 < m_tInfo.iHp) m_tInfo.iHp -= iHp; }
	void				Set_HungerPlus()			{ if (5 > m_tInfo.iHunger) m_tInfo.iHunger++; }
	void				Set_HungerMinus()			{ if (0 < m_tInfo.iHunger) m_tInfo.iHunger--; }
	void				Set_GoldPlus(_int iGold)	{ m_tInfo.iGold += iGold; }
	void				Set_GoldMinus(_int iGold)	{ m_tInfo.iGold -= iGold; }

	void				Set_Stun() { m_tInfo.bStun = true; }
	void				Set_Slow() { m_tInfo.bSlow = true; }
	void				Set_KnockBack() { m_bKnockBack = true; }

	void				OnHit(_int _HpMinus); // sh
	int					Get_PlayerAttack() { return m_tInfo.iAtk; } // sh
	PLAYERINFO			Get_Info() { return m_tInfo; } // sh
	_int				Get_Hp() { return m_tInfo.iHp; } // sh
	void				Set_Level(const _int& iMonsterHp, const _int& iMonsterExp); // sh

	const _float&		Get_CurSpeed() { return m_tInfo.fSpeed; }
	const PLAYER_STATE&	Get_CurState() { return m_eState; }
	void				Set_CurBlock(CBlock* pBlock) { m_pCurrentBlock = pBlock; }
	void				Set_JSpeed(_float fJSpeed) { m_fJSpeed = fJSpeed; }

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Mouse_Move(void);
	void				Mouse_Click(const _float& fTimeDelta);
	//void				Set_OnTerrain(void);
	void				Jump(const _float& fTimeDelta);
	//_float				Get_Height();
	void				CollisionEvent(CGameObject * pOtherObj);
	void				KnockBack(const _float& fTimeDelta); // sh
	void				Stun(const _float& fTimeDelta); // sh
	void				Slow(const _float& fTimeDelta); // sh
	void				Hunger(const _float& fTimeDelta); // sh

public:
	void				Set_Right(CItem* pRight) { m_pRight = pRight; }
	void				Set_Left(CItem* pLeft) { m_pLeft = pLeft; }
	CItem*				Get_Right() { return m_pRight; }
	CItem*				Get_Left() { return m_pLeft; }
	_bool				Is_Snippered() { return m_bSnipper; }
	void				Respawn();
	void				Set_Info(ITEMINFO tInfo, _int iSign);
	void				Add_CollisionGroup(CGameObject* pObj)
	{
		m_CollisionGroup.push_back(pObj);
	}
	vector<CGameObject*>*	Get_CollisionGroup()
	{
		return &m_CollisionGroup;
	}
	_float			Get_MinAngle() { return m_fDotAngle; }
	void			Set_Interact(CGameObject* pObj) { m_pInteractObj = pObj; }
	void			Cal_Money(_int iPrice) { m_tInfo.iGold += iPrice; }


private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	CBlock*				m_pCurrentBlock = nullptr;
	_bool				m_bBlockChanged = true;
	_bool				m_bOnce = false;

	PLAYER_STATE		m_eState = PLAYER_GROUND;

	_vec3				m_vDirection;
	//_float			m_fSpeed = 5.f;
	_float				m_fScale = 1.f;

	// Related to Jump
	_bool				m_bJump = false;
	_float				m_fJSpeed;
	_float				m_fJSpeed0;
	_float				m_fAccel;
	_float				m_fJTimeDelta = 0.f;

	// Hit
	_float				m_InvincibilityTimeAcc = 0.f;

	// KnockBack
	_bool				m_bKnockBack = false;

	// Stun
	_bool				m_bStunParticle = false;
	_float				m_fStunTimeAcc = 0.f;

	// Slow
	_float				m_fSlowTimeAcc = 0.f;

	// Hunger
	_float				m_fHungerTimeAcc = 0.f;
	_bool				m_bHungerOneCheck = false;

	// click Test?
	_float				m_fLBClick = 0.f;
	_float				m_fTimeDelta = 0.f;

	// Releated to Item
	CItem*				m_pRight = nullptr;
	CItem*				m_pLeft = nullptr;

	_bool				m_bSnipper = false;

	PLAYERINFO			m_tInfo;


	// Related to Death & Respawn
	_float				m_fDeathTime = 0.f;
	_bool				m_bDeadMotion = false;

	CInventory*			m_pInv = nullptr;

	// Collision Group for Player
	vector<CGameObject*>	m_CollisionGroup;

	// Interaction
	CGameObject*			m_pInteractObj = nullptr;
	_float					m_fDotAngle = 360.f;
	_bool					m_bText = false;
	wstring					m_str;

	// Heal Potion
	_float					m_fHealTime = 0.f;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void		Free(void);

};

