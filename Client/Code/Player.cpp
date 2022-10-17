#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_Function.h"
#include "BulletMgr.h"
#include "Inventory.h"
#include "InvImg.h"
#include "ImGuiMgr.h"
#include "ItemMgr.h"
#include "Block.h"
#include "MapUI.h"
#include "MiniMap.h"
#include "PlayerInfo.h"
#include "ParticleMgr.h"
#include "CrossHair.h"
#include "CameraMgr.h"
#include "BonFire.h"

#include "Monster.h"
#include "Bullet.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fAccel(0.01f)
	, m_fJSpeed0(0.15f)
	, m_fJSpeed(0.15f)
{
	memset(&m_tInfo, 0, sizeof(PLAYERINFO));
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	m_fTimeDelta = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransCom->Set_Pos(0.5f, 1.f, 1.5f);
	m_pTransCom->Set_Pos(3.f, 1.f, 3.f);

	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_fScale = 0.6f;
	m_pTransCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	// 플레이어 스탯정보
	//m_tInfo.iHp = 20;
	m_tInfo.iHp = 10;
	m_tInfo.iHpMax = 20;
	//m_tInfo.iHpMax = m_tInfo.iHp; // sh
	m_tInfo.iAtk = 1;
	//m_tInfo.iAtk = 10; // sh
	m_tInfo.iDef = 10;
	m_tInfo.iExp = 0;
	m_tInfo.iExpMax = 7;
	m_tInfo.iHunger = 5;
	m_tInfo.fSpeed = 5.f;
	m_tInfo.iLevel = 1;

	m_tInfo.fSlowSpeed = m_tInfo.fSpeed * 0.5f;
	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	// Create Minimap Icon
	static _bool	bOnce = false;
	if (!bOnce)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);
		bOnce = true;
	}
	// *Create Minimap Icon

//	
	if (!m_bDeadMotion && m_bDead)
	{
		CCameraMgr::GetInstance()->Action_PlayerDie();
		CCameraMgr::GetInstance()->Set_Camera(this, 0.3f, 1.f);
		m_bDeadMotion = true;
	}

	if (m_bDead)
	{
		m_fDeathTime += fTimeDelta;
		if (5.f < m_fDeathTime)
		{
			Respawn();
		}
		return 0;
	}


	m_fTimeDelta = fTimeDelta;

	if (!(GetKeyState(VK_TAB) & 0x80))		// Except Open Inventory
	{
		Mouse_Move();
	}

	// test area //////////////////



	///////////////////////////

	Key_Input(fTimeDelta);
	Jump(fTimeDelta);
	
	KnockBack(fTimeDelta);
	Stun(fTimeDelta);
	Slow(fTimeDelta);
	Hunger(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	_int iHp = m_tInfo.iHp;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	m_InvincibilityTimeAcc += fTimeDelta; // sh
	return 0;
}

void CPlayer::LateUpdate_Object(void)
{

	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	Mouse_Click(m_fTimeDelta);

	if (m_pColliderCom->Get_MinPoint().y < 0.1f)
	{
		Set_OnTerrain();
	}
	else
	{
		_vec3	vPlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vPlayerPos.y = m_pColliderCom->Get_MinPoint().y;

		if (m_eState == PLAYER_ON_BLOCK)
		{
			m_fJSpeed = 0.f;
			m_bJump = false;

			if (m_pCurrentBlock == nullptr)
			{
				m_eState = PLAYER_JUMP;
				m_bJump = true;
				return;
			}

			_vec3	vBlockPos;
			m_pCurrentBlock->m_pTransCom->Get_Info(INFO_POS, &vBlockPos);

			_float	fDistFromBlock;

			if (m_pCurrentBlock)
				fDistFromBlock = D3DXVec3Length(&(vBlockPos - vPlayerPos));

			_float	fBlockRange = m_fScale + D3DXVec3Length(&(dynamic_cast<CCollider*>(m_pCurrentBlock->Get_Component(L"Proto_ColliderCom", ID_STATIC))->Get_MaxPoint() - m_pCurrentBlock->m_pTransCom->Get_Pos()));

			if (fDistFromBlock < fBlockRange - 0.002f)
				m_bBlockChanged = false;
			else if (fDistFromBlock > fBlockRange - 0.002f && fDistFromBlock < fBlockRange - 0.001f)
			{
				m_bBlockChanged = true;
			}
			else if (fDistFromBlock > fBlockRange - 0.001f)
				m_pCurrentBlock = nullptr;
		}
	}



	// camera change Test
	wstring pObjTag = (m_pRight != nullptr ? m_pRight->Get_ObjTag() : L"");
	if (L"Arrow" == pObjTag && Get_DIMouseState(DIM_RB) & 0x80)
		m_bSnipper = true; 
	else
		m_bSnipper = false;



}

void CPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	if (m_tInfo.bStun) // sh
		return;

	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)	
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));
	}


	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		if (!m_bJump)
		{
			if (m_pCalculatorCom == nullptr || m_eState == PLAYER_ON_BLOCK)
				m_fJSpeed = m_fJSpeed0;
			m_eState = PLAYER_JUMP;
			m_bJump = true;
		}
	}

	if (Engine::Key_Down(DIK_M))
	{
		CMapUI* pMap = dynamic_cast<CMapUI*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Map"));

		if (pMap->Get_MapState())
			pMap->Set_CloseMap();
		else
			pMap->Set_OpenMap();
	}

	if (Engine::Key_Down(DIK_X))
	{
		CPlayerInfo* pPlayerInfo = dynamic_cast<CPlayerInfo*>(Engine::Get_GameObject(L"Layer_UI", L"UI_PlayerInfo"));
		
		if (pPlayerInfo->Get_InfoState())
			pPlayerInfo->Set_CloseInfo();
		else
			pPlayerInfo->Set_OpenInfo();
	}

	// camera test
	if (Key_Down(DIK_C))
	{
		// 한바퀴 돌기
		//CCameraMgr::GetInstance()->Change_Camera(CAM_OBJECT);
		//CCameraMgr::GetInstance()->Set_Camera(this, 3.f, 0.f);
		//CCameraMgr::GetInstance()->Action_Camera(360.f);

		// Deadmotion
		m_bDead = true;
		//CCameraMgr::GetInstance()->Action_PlayerDie();
		//CCameraMgr::GetInstance()->Set_Camera(this, 0.3f, 1.f);


	}
	if (Key_Down(DIK_V))
	{
		CCameraMgr::GetInstance()->Change_Camera(CAM_STATIC);
	}

}

void CPlayer::Mouse_Move(void)
{
	if (m_tInfo.bStun) // sh
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_pTransCom->Rotation(ROT_X, D3DXToRadian(dwMouseMove / 10.f));
	}
}

void CPlayer::Mouse_Click(const _float& fTimeDelta)
{
	m_fLBClick += fTimeDelta;

	//if ((Engine::Get_DIKeyState(DIK_X) & 0x80) && (0.3f<m_fLBClick))
	//{
	//	m_fLBClick = 0.f;
	//	_vec3 vPos, vLook;
	//	m_pTransCom->Get_Info(INFO_POS, &vPos);
	//	m_pTransCom->Get_Info(INFO_LOOK, &vLook);

	//	CBulletMgr::GetInstance()->Fire(BULLET_WAND);
	//}
}

void CPlayer::Set_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_float fHeight = Get_Height();

	m_pTransCom->Set_Y(fHeight);
}

void CPlayer::Jump(const _float & fTimeDelta)
{
	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		_float fHeight = Get_Height();

		if (m_fJTimeDelta > 2.f && 0.f >= m_pColliderCom->Get_MinPoint().y)
		{
			m_bJump = false;
			m_eState = PLAYER_GROUND;
			m_fJTimeDelta = 0.f;
			m_pCurrentBlock = nullptr;
			m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
			m_fJSpeed = m_fJSpeed0;
		}
		else
		{
			m_fJSpeed -= m_fAccel;
			m_pTransCom->Plus_PosY(m_fJSpeed);
			m_fJTimeDelta += 0.1f;
		}
	}

}

_float CPlayer::Get_Height()
{
	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	_int	iWidth, iDepth, iIntavel;

	CImGuiMgr::GetInstance()->Get_MapWidth(&iWidth, &iDepth, &iIntavel);

	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainTexCom, 0.f);

	return m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), 20, 20) + 1.f * m_fScale;
}

void CPlayer::CollisionEvent(CGameObject * pOtherObj)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(pOtherObj);
	if (pMonster == pOtherObj)
		OnHit(pMonster->Get_MonsterAttack());

	CBullet* pBullet = dynamic_cast<CBullet*>(pOtherObj);
	if (pBullet == pOtherObj)
		OnHit(pBullet->Get_BulletAttack());

	CItem*	pItem = dynamic_cast<CItem*>(pOtherObj);
	if (nullptr != pItem && STATE_GROUND == pItem->Get_State())
	{
		CInventory*		pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));

		CItem* pImg = nullptr;
		pImg = static_cast<CItem*>(CItemMgr::GetInstance()->Add_GameObject(L"Layer_UI", pItem->Get_TextureTag(), pItem));

		pInv->Set_Inventory(pImg);
	}

	CBlock*	pBlock = dynamic_cast<CBlock*>(pOtherObj);
	if (pBlock)
	{
		// AABB
		CCollider* BlockCollider = dynamic_cast<CCollider*>(pBlock->Get_Component(L"Proto_ColliderCom", ID_STATIC));
		BDBOX		PlayerBox = { m_pColliderCom->Get_MinPoint(), m_pColliderCom->Get_MaxPoint() };
		BDBOX		BlockBox = { BlockCollider->Get_MinPoint(), BlockCollider->Get_MaxPoint() };

		_float		fDistX = 0.f;
		_float		fDistY = 0.f;
		_float		fDistZ = 0.f;

		// Player at Leftside
 		if (PlayerBox.vMax.x > BlockBox.vMin.x && PlayerBox.vMax.x < BlockBox.vMax.x)
		{
			fDistX = BlockBox.vMin.x - PlayerBox.vMax.x;
		}

		// Player at Rightside
		else if (PlayerBox.vMin.x < BlockBox.vMax.x && PlayerBox.vMin.x > BlockBox.vMin.x)
		{
			fDistX = BlockBox.vMax.x - PlayerBox.vMin.x;
		}

		// Player at Frontside
		if (PlayerBox.vMin.z < BlockBox.vMax.z  && PlayerBox.vMin.z > BlockBox.vMin.z)
		{
			fDistZ = BlockBox.vMax.z - PlayerBox.vMin.z;
		}

		// Player at Backside
		else if (PlayerBox.vMax.z > BlockBox.vMin.z && PlayerBox.vMax.z < BlockBox.vMax.z)
		{
			fDistZ = BlockBox.vMin.z - PlayerBox.vMax.z;
		}

		// Player On Block
		if (PlayerBox.vMin.y < BlockBox.vMax.y && PlayerBox.vMin.y > BlockBox.vMin.y && m_eState != PLAYER_GROUND)
		{
			m_eState = PLAYER_ON_BLOCK;
			m_bJump = false;
			fDistX = 0.f;
			fDistY = BlockBox.vMax.y - PlayerBox.vMin.y;
			fDistZ = 0.f;

			if (m_pCurrentBlock != nullptr && m_pCurrentBlock != pBlock)
			{
				m_pCurrentBlock = pBlock;
				m_bBlockChanged = false;
				_vec3	PlayerPos;
				m_pTransCom->Get_Info(INFO_POS, &PlayerPos);
				_vec3 vTemp = { 0.f, fDistY, 0.f };
				_float	fBlockHeight = pBlock->Get_Height();
				m_pTransCom->Set_Y(fBlockHeight + (PlayerPos.y - PlayerBox.vMin.y) - 0.0001f);
				return;
			}

 			m_pCurrentBlock = pBlock;
			fDistY = 0.f;
		}

		// Player Under Block
		else if (PlayerBox.vMax.y > BlockBox.vMin.y && PlayerBox.vMax.y < BlockBox.vMax.y)
		{
			fDistY = BlockBox.vMin.y - PlayerBox.vMax.y;
			m_fJSpeed = 0.f;
		}

		_vec3	PlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &PlayerPos);

		//if (fabs(fabs(fDistX) - fabs(fDistZ)) < 0.15f)
		//	return;
		

		if ((PlayerPos.x < BlockBox.vMin.x || PlayerPos.x > BlockBox.vMax.x) && (PlayerPos.z < BlockBox.vMin.z || PlayerPos.z > BlockBox.vMax.z) && fabs(fDistX) > 0.0001f && fabs(fDistZ) > 0.0001f)
		{
			if (sqrtf(fDistX * fDistX + fDistZ * fDistZ) < fabs(BlockBox.vMax.x - BlockBox.vMin.x) * 0.2f)
				return;
			if (fabs(fDistX) > fabs(fDistZ))
				fDistX = 0.f;
			else if (fabs(fDistX) < fabs(fDistZ))
				fDistZ = 0.f;
		}
		else
			return;		

		_vec3	PlayerLook;
		m_pTransCom->Get_Info(INFO_LOOK, &PlayerLook);
		PlayerLook.y = 0.f;

		m_pTransCom->Set_Pos(PlayerPos.x + fDistX, PlayerPos.y, PlayerPos.z + fDistZ);
		//m_pTransCom->Move_Pos(&(_vec3(fDistX, 0.f, fDistZ) * m_fTimeDelta));
	}
}

void CPlayer::Respawn()
{
	m_bDead = false;
	m_bDeadMotion = false;
	m_fDeathTime = 0.f;

	CCameraMgr::GetInstance()->Change_Camera(CAM_STATIC);

	CTransform* pTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Bonfire", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK(pTransCom);

	_vec3 vPos = pTransCom->Get_Pos();
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

}

void CPlayer::Set_Info(ITEMINFO tInfo, _int iSign)
{
	m_tInfo.iAtk	+= iSign * tInfo.iAtk;
	m_tInfo.iDef	+= iSign * tInfo.iDef;
	m_tInfo.iHunger += iSign * tInfo.iHunger;
	m_tInfo.fSpeed	+= iSign * tInfo.fSpeed;
}

void CPlayer::OnHit(_int _HpMinus)
{
	if (0 >= m_tInfo.iHp)
	{
		//m_bKnockBack = true;
		m_tInfo.iHp = 0;
		return;
	}

	// 플레이어는 2초간 무적
	if (2.f < m_InvincibilityTimeAcc)
	{
		//CParticleMgr::GetInstance()->Set_Info(this,
		//	1,
		//	1.f,
		//	{ 1.f, 1.f, 1.f },
		//	2.f,
		//	{ 1.f, 0.f, 0.f, 0.01f });
		//CParticleMgr::GetInstance()->Call_Particle(PTYPE_FOUNTAIN, TEXTURE_7);

		m_bKnockBack = true;
		m_tInfo.iHp -= _HpMinus;
		m_InvincibilityTimeAcc = 0.f;
	}
}

void CPlayer::KnockBack(const _float& fTimeDelta)
{
	if (!m_bKnockBack)
		return;

	_vec3 vPos, vLook;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Info(INFO_LOOK, &vLook);

	_float fHeight = Get_Height();

	if (m_fJTimeDelta > 2.f && 0.f >= m_pColliderCom->Get_MinPoint().y)
	{
		m_bKnockBack = false;
		m_pCurrentBlock = nullptr;
		m_eState = PLAYER_GROUND;
		m_fJTimeDelta = 0.f;

		m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
		m_fJSpeed = m_fJSpeed0;
	}
	else
	{
		m_pTransCom->KnockBack_Target(&vLook, -3.f, fTimeDelta);

		m_fJSpeed -= m_fAccel;
		m_pTransCom->Plus_PosY(m_fJSpeed);
		m_fJTimeDelta += 0.1f;
	}
}

void CPlayer::Stun(const _float & fTimeDelta)
{
	if (!m_tInfo.bStun)
		return;

	if (!m_bStunParticle)
	{
		CParticleMgr::GetInstance()->Set_Info(this, 1, 1.f, { 0.f, 0.3f, 1.0f }, 
												2.f, {1.f, 1.f, 1.f, 1.f}, 5.f, true);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_SPOT, TEXTURE_3);
		m_bStunParticle = true;
	}

	m_fStunTimeAcc += fTimeDelta;
	if (2.f < m_fStunTimeAcc) // 2.f -> StunTime
	{
		m_bStunParticle = false;
		m_tInfo.bStun = false;
		m_fStunTimeAcc = 0.f;
	}
}

void CPlayer::Slow(const _float & fTimeDelta)
{
	if (!m_tInfo.bSlow)
		return;

	m_tInfo.fSpeed = m_tInfo.fSlowSpeed;

	m_fSlowTimeAcc += fTimeDelta;
	if (3.f < m_fSlowTimeAcc)
	{
		m_tInfo.bSlow = false;
		m_tInfo.fSpeed = m_tInfo.fSlowSpeed  * 2.f;
		m_fSlowTimeAcc = 0.f;
	}
}

void CPlayer::Hunger(const _float & fTimeDelta)
{
	// test
	if (Key_Down(DIK_K))
		Set_HungerMinus();

	if (Key_Down(DIK_L))
		Set_HungerPlus();
	// test

	if (0 >= m_tInfo.iHunger && !m_tInfo.bSlow)
		m_tInfo.fSpeed = m_tInfo.fSlowSpeed;
	else if (0 < m_tInfo.iHunger && !m_tInfo.bSlow)
		m_tInfo.fSpeed = m_tInfo.fSlowSpeed * 2;

	m_fHungerTimeAcc += fTimeDelta;
	if (30.f < m_fHungerTimeAcc)
	{
		Set_HungerMinus();
		m_fHungerTimeAcc = 0.f;
	}
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}
