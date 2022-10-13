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

class CPlayer : public CGameObject
{
	enum PLAYER_STATE { PLAYER_GROUND, PLAYER_ON_BLOCK, PLAYER_JUMP, PLAYER_STATE_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Obejct(void) override;

public:
	void				Set_HpPlus() { m_tInfo.iHp += 1; }
	void				OnHit(_int _HpMinus); // sh
	int					Get_PlayerAttack() { return m_tInfo.iAtk; } // sh
	PLAYERINFO			Get_PlayerInfo() { return m_tInfo; } // sh

private:
	HRESULT				Add_Component(void);
	void				Key_Input(const _float& fTimeDelta);
	void				Mouse_Move(void);
	void				Mouse_Click(const _float& fTimeDelta);
	void				Set_OnTerrain(void);
	void				Jump(const _float& fTimeDelta);
	_float				Get_Height();
	void				CollisionEvent(CGameObject * pOtherObj);
	void				KnockBack(const _float& fTimeDelta); // sh

public:
	void				Set_Right(CItem* pRight) { m_pRight = pRight; }
	void				Set_Left(CItem* pLeft) { m_pLeft = pLeft; }
	_bool				Is_Snippered() { return m_bSnipper; }
	void				Respawn();

private:
	CRcTex*				m_pBufferCom = nullptr;
	CTransform*			m_pTransCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;

	CBlock*				m_pCurrentBlock = nullptr;
	_bool				m_bBlockChanged = true;

	PLAYER_STATE		m_eState = PLAYER_GROUND;

	_vec3				m_vDirection;
	_float				m_fSpeed = 5.f;
	_float				m_fScale = 1.f;

	// Related to Jump
	_bool				m_bJump = false;
	_float				m_fJSpeed; 		
	_float				m_fJSpeed0; 	
	_float				m_fAccel;		
	_float				m_fJTimeDelta = 0.f;

	// KnockBack
	_bool				m_bKnockBack = false;

	// click Test?
	_float				m_fLBClick = 0.f;
	_float				m_fTimeDelta = 0.f;

	// Releated to Item
	CItem*				m_pRight	= nullptr;	
	CItem*				m_pLeft		= nullptr;	

	_bool				m_bSnipper = false;

	PLAYERINFO			m_tInfo;

	_float				InvincibilityTimeAcc = 0.1f;

	// Related to Death & Respawn
	_float				m_fDeathTime = 0.f;
	_bool				m_bDeadMotion = false;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

};

