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
#include "EcoObject.h"
#include "RockFall.h"

#include "Monster.h"
#include "Bullet.h"
#include "Mimic.h"
#include "TreasureBox.h"
#include "Gold.h"
#include "Door.h"
#include "KnifeTrap.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fAccel(0.01f)
	, m_fJSpeed0(0.2f)
	, m_fJSpeed(0.2f)
{
	memset(&m_tInfo, 0, sizeof(PLAYERINFO));
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(_vec3 vPos)
{
	m_fTimeDelta = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransCom->Set_Pos(0.5f, 1.f, 1.5f);
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	// 플레이어 스탯정보
	m_tInfo.iHp = 20;
	m_tInfo.iHpMax = 20;
	m_tInfo.iAtk = 1;
	m_tInfo.iDef = 10;
	m_tInfo.iExp = 0;
	m_tInfo.iExpMax = 10;
	m_tInfo.iHunger = 5;
	m_tInfo.fSpeed = 5.f;
	m_tInfo.fSlowSpeed = m_tInfo.fSpeed * 0.5f;
	m_tInfo.iLevel = 1;
	m_tInfo.iGold = 0;

	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	// Create Minimap Icon
	if (!m_bOnce)
	{
		CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
		pMiniMap->Add_Icon(m_pGraphicDev, this);

		m_bOnce = true;
	}
	// *Create Minimap Icon

//	
	
	if (!m_pInv)
		m_pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));


	if (!m_bDeadMotion && m_bDead)
	{
		CCameraMgr::GetInstance()->Action_PlayerDie();
		//CCameraMgr::GetInstance()->Set_Camera(this, 0.3f, 1.f);
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

	if (!((Get_DIKeyState(DIK_TAB) & 0x80) || m_pInv->Is_Open()))
	{
		Mouse_Move();
	}


	// test area //////////////////

	if (Key_Down(DIK_U))
	{
		OnHit(1);
	}

	///////////////////////////

	Key_Input(fTimeDelta);
	Jump(fTimeDelta);
	
	KnockBack(fTimeDelta);
	Stun(fTimeDelta);
	Slow(fTimeDelta);
	Hunger(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	
	
	//CLightMgr::GetInstance()->Update_Pos(1, m_pTransCom->Get_Pos());


	Add_RenderGroup(RENDER_NONALPHA, this);

	_int iHp = m_tInfo.iHp;

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

	m_InvincibilityTimeAcc += fTimeDelta; // sh

	if (!m_bKnockBack)
		m_pCurrentBlock = nullptr;



	_vec3 vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	// sh
	//cout << (_int)vPos.x << "  " << (_int)vPos.y << "  " << (_int)vPos.z << endl;

	return 0;
}

void CPlayer::LateUpdate_Object(void)
{

	_vec3 vPos, vScale, vLook;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pTransCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	Mouse_Click(m_fTimeDelta);

	_vec3	vPlayerPos;
	m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos.y = m_pColliderCom->Get_MinPoint().y;



	if (m_eState == PLAYER_ON_BLOCK)
	{
		m_fJSpeed = m_fJSpeed0;
		m_bJump = false;

		if (m_pCurrentBlock == nullptr)
		{
			m_fJSpeed = 0.f;
			m_eState = PLAYER_JUMP;
			m_bJump = true;
		}
	}

	// camera change Test
	//wstring pObjTag = (m_pRight != nullptr ? m_pRight->Get_ObjTag() : L"");
	//if (L"Arrow" == pObjTag && Get_DIMouseState(DIM_RB) & 0x80)
	//	m_bSnipper = true; 
	//else
	//	m_bSnipper = false;
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

	if(m_bText)
		Render_Font(L"Font_Jinji", m_str.c_str(), &_vec2(WINCX*0.5f, WINCY*0.5f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



#ifdef _DEBUG
	// Collider
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		&(m_pColliderCom->Get_WorldMatrix()));
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pColliderCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

void CPlayer::Set_HpPlus(_int iHp)
{
	if (m_tInfo.iHpMax > m_tInfo.iHp) m_tInfo.iHp += iHp;



		CParticleMgr::GetInstance()->Set_Info(this, 3, 0.5f, { 1.f, 1.f, 2.f },
			1.f, { 1.f, 1.f, 1.f, 1.f }, 5.f, true);
		CParticleMgr::GetInstance()->Add_Info_Spot(true, false);
		CParticleMgr::GetInstance()->Call_Particle(PTYPE_MOOD, TEXTURE_16);


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

		Engine::Play_Sound(L"P_Move1.mp3", SOUND_PLAYER, 0.3f);
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));

		Engine::Play_Sound(L"P_Move2.mp3", SOUND_PLAYER, 0.3f);
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));

		Engine::Play_Sound(L"P_Move1.mp3", SOUND_PLAYER, 0.3f);
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_tInfo.fSpeed * fTimeDelta));

		Engine::Play_Sound(L"P_Move2.mp3", SOUND_PLAYER, 0.3f);
	}

	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		Engine::StopSound(SOUND_PLAYER);
		Engine::Play_Sound(L"P_Jump.mp3", SOUND_PLAYER, 0.3f);

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

	if (Engine::Key_Down(DIK_P))
	{
		CPlayerInfo* pPlayerInfo = dynamic_cast<CPlayerInfo*>(Engine::Get_GameObject(L"Layer_UI", L"UI_PlayerInfo"));
		
		if (pPlayerInfo->Get_InfoState())
			pPlayerInfo->Set_CloseInfo();
		else
			pPlayerInfo->Set_OpenInfo();
	}

	// camera test
	if (Engine::Key_Down(DIK_C))
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
	if (Engine::Key_Down(DIK_V))
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

//void CPlayer::Set_OnTerrain(void)
//{
//	_vec3		vPos;
//	m_pTransCom->Get_Info(INFO_POS, &vPos);
//
//	_float fHeight = Get_Height();
//
//	m_pTransCom->Set_Y(fHeight);
//}

void CPlayer::Jump(const _float & fTimeDelta)
{
	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		//_float fHeight = Get_Height();
		if (m_pCurrentBlock)
		{
			if (m_fJTimeDelta > 2.f && m_pCurrentBlock->Get_Height() >= m_pColliderCom->Get_MinPoint().y)
			{
				m_bJump = false;
				m_eState = PLAYER_ON_BLOCK;
				m_fJTimeDelta = 0.f;
				//m_pCurrentBlock = nullptr;
				//m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
				m_fJSpeed = m_fJSpeed0;
			}
			else
			{
				m_fJSpeed -= m_fAccel;
				m_pTransCom->Plus_PosY(m_fJSpeed);
				m_fJTimeDelta += 0.1f;
			}
		}
		else
		{
			m_fJSpeed -= m_fAccel;
			m_pTransCom->Plus_PosY(m_fJSpeed);
			m_fJTimeDelta += 0.1f;
		}
	}

}

//_float CPlayer::Get_Height()
//{
//	_vec3 vPos;
//	m_pTransCom->Get_Info(INFO_POS, &vPos);
//
//	_int	iWidth, iDepth, iIntavel;
//
//	CImGuiMgr::GetInstance()->Get_MapWidth(&iWidth, &iDepth, &iIntavel);
//
//	Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
//	NULL_CHECK_RETURN(pTerrainTexCom, 0.f);
//
//	return m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), 20, 20) + 1.f * m_fScale;
//}

void CPlayer::CollisionEvent(CGameObject * pOtherObj)
{
	CMimic*	pMimic = dynamic_cast<CMimic*>(pOtherObj);
	CGold*	pGold = dynamic_cast<CGold*>(pOtherObj);

	bool bText = false;

	if (pMimic)
	{
		if (Engine::Key_Down(DIK_E))
			pMimic->InteractEvent();
	}
	if (pGold)
	{
		if (Engine::Key_Down(DIK_E))
			pGold->InteractEvent();
	}


	CTreasureBox*	pTreasureBox = dynamic_cast<CTreasureBox*>(pOtherObj);
	if (pTreasureBox)
	{
		if (pTreasureBox->Get_CurrentTexture() == 0)
		{
			m_str = L"E : Open";
			bText = true;
		}
		if (Engine::Key_Down(DIK_E))
			pTreasureBox->InteractEvent();
	}

	CMonster* pMonster = dynamic_cast<CMonster*>(pOtherObj);
	if (pMonster == pOtherObj)
		OnHit(pMonster->Get_MonsterAttack());

	CBullet* pBullet = dynamic_cast<CBullet*>(pOtherObj);
	if (pBullet == pOtherObj)
		OnHit(pBullet->Get_BulletAttack());


	CEcoObject* pEco = dynamic_cast<CEcoObject*>(pOtherObj);
	if (nullptr != pEco)
	{
		if (ECO_ROCKFALL == pEco->Get_Type())
		{
			OnHit(static_cast<CRockFall*>(pEco)->Get_Attack());
		}
		else if (ECO_KNIFETRAP == pEco->Get_Type())
		{
			OnHit(static_cast<CKnifeTrap*>(pEco)->Get_Attack());
		}
		else if (ECO_DOOR == pEco->Get_Type())
		{

			m_str = static_cast<CDoor*>(pEco)->Get_String();
			if (Engine::Key_Down(DIK_E))
			{
				pOtherObj->InteractEvent();
			}
			bText = true;
		}
	}

	CItem*	pItem = dynamic_cast<CItem*>(pOtherObj);
	if (nullptr != pItem && STATE_GROUND == pItem->Get_State() && pItem->Get_ItemType() != ITEM_GOLD)
	{
		m_str = L"E : Get";
		if (Key_Down(DIK_E))
		{
			CInventory*		pInv = static_cast<CInventory*>(Engine::Get_GameObject(L"Layer_UI", L"UI_Inventory"));
			if (!pInv->Is_Full())
			{
				pOtherObj->InteractEvent();
				pInv->Set_Inventory(pItem);

			}
		}
		bText = true;
	}

	
	m_bText = bText;


	
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

		// Player On Block
		if (PlayerBox.vMin.y < BlockBox.vMax.y && PlayerBox.vMin.y > BlockBox.vMin.y)
		{
			if (m_pCurrentBlock == pBlock)
			{
				m_eState = PLAYER_ON_BLOCK;
				m_bJump = false;
				m_fJTimeDelta = 0.f;

				fDistY = BlockBox.vMax.y - PlayerBox.vMin.y;

				if (fDistY < 0.01f)
					return;

 				_vec3	PlayerPos = m_pTransCom->Get_Pos();
				_float	fBlockHeight = m_pCurrentBlock->Get_Height();
				m_pTransCom->Set_Pos(PlayerPos.x, PlayerPos.y + fDistY, PlayerPos.z);
				return;
			}
			else
				return;
		}

		// Player Under Block
		/*else if (PlayerBox.vMax.y > BlockBox.vMin.y && PlayerBox.vMax.y < BlockBox.vMax.y)
		{
			_vec3	PlayerPos = m_pTransCom->Get_Pos();
			if ((PlayerPos.x < BlockBox.vMin.x || PlayerPos.x > BlockBox.vMax.x)
				&& (PlayerPos.z < BlockBox.vMin.z || PlayerPos.z > BlockBox.vMax.z))
				return;

			if (m_pCurrentBlock == nullptr && m_eState == PLAYER_JUMP)
			{
				fDistY = BlockBox.vMin.y - PlayerBox.vMax.y;
				m_fJSpeed = 0.f;
				m_pTransCom->Set_Pos(PlayerPos.x, PlayerPos.y + fDistY, PlayerPos.z);
			}
		}*/
	}


}

void CPlayer::Respawn()
{
	m_bDead = false;
	m_bDeadMotion = false;
	m_fDeathTime = 0.f;
	m_tInfo.iHp = 10;

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
	Engine::StopSound(SOUND_PLAYER);
	Engine::Play_Sound(L"P_Hit.mp3", SOUND_PLAYER, 1.f);

	if (0 >= m_tInfo.iHp)
	{
		//m_bKnockBack = true;
		m_tInfo.iHp = 0;
		// m_bDead = true;
		return;
	}

	// 플레이어는 2초간 무적
	if (2.f < m_InvincibilityTimeAcc)
	{	
		if(0 < _HpMinus)
		m_bKnockBack = true;
		cout << "아야 " << endl;
		m_tInfo.iHp -= _HpMinus;
		m_InvincibilityTimeAcc = 0.f;
	}
}

void CPlayer::KnockBack(const _float& fTimeDelta)
{
	if (!m_bKnockBack)
		return;

	// Light
	D3DLIGHT9		tLightInfo;
	m_pGraphicDev->GetLight(LIGHT_PLAYER, &tLightInfo);
	tLightInfo.Diffuse = { 1.f, 0.f, 0.f ,1.f };
	tLightInfo.Ambient = { 1.f, 0.f, 0.f ,1.f };
	m_pGraphicDev->SetLight(LIGHT_PLAYER, &tLightInfo);
	m_pGraphicDev->LightEnable(LIGHT_PLAYER, TRUE);
	if (0.5f < m_fJTimeDelta)
		m_pGraphicDev->LightEnable(LIGHT_PLAYER, FALSE);


	_vec3 vPos, vLook;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	m_pTransCom->Get_Info(INFO_LOOK, &vLook);

	m_eState = PLAYER_JUMP;

	//_float fHeight = Get_Height();
	if (m_pCurrentBlock)
	{
		if (m_fJTimeDelta > 2.f && m_pCurrentBlock->Get_Height() + 0.2f >= m_pColliderCom->Get_MinPoint().y)
		{
			m_bKnockBack = false;
			//m_pCurrentBlock = nullptr;
			m_eState = PLAYER_ON_BLOCK;
			m_fJTimeDelta = 0.f;
			m_pTransCom->Set_Y(m_pCurrentBlock->Get_Height() + (m_pTransCom->Get_Pos().y - m_pColliderCom->Get_MinPoint().y));
			//m_pTransCom->Set_Pos(vPos.x, fHeight, vPos.z);
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
	else
	{
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
		Engine::StopSound(SOUND_PLAYER);
		Engine::Play_Sound(L"P_Stun.mp3", SOUND_PLAYER, 1.f);

		CParticleMgr::GetInstance()->Set_Info(this, 1, 1.f, { 0.f, 0.3f, 1.0f }, 
												2.f, {1.f, 1.f, 1.f, 1.f}, 5.f, true);
		CParticleMgr::GetInstance()->Add_Info_Spot(true, false);
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
	if (8.f < m_fSlowTimeAcc)
	{
		m_tInfo.bSlow = false;
		m_tInfo.fSpeed = m_tInfo.fSlowSpeed  * 2.f;
		m_fSlowTimeAcc = 0.f;
	}
}

void CPlayer::Hunger(const _float & fTimeDelta)
{
	// test
	if (Engine::Key_Down(DIK_K))
		Set_HungerMinus();

	if (Engine::Key_Down(DIK_L))
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


void CPlayer::Set_Level(const _int& iMonsterHp, const _int& iMonsterExp)
{
	if (1 >= iMonsterHp)
	{
		if (m_tInfo.iExpMax > m_tInfo.iExp)
			m_tInfo.iExp += iMonsterExp;
		else
		{
			m_tInfo.iLevel += 1;
			m_tInfo.iExp = 0;
			m_tInfo.iExpMax += 10;
		}
	}
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CPlayer *	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	m_bOnce = false;
	m_CollisionGroup.clear();
	m_CollisionGroup.swap(vector<CGameObject*>());
	CGameObject::Free();
}
