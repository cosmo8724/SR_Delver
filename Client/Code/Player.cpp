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
//#include "ParticleMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vDirection(0.f, 0.f, 0.f)
	, m_fAccel(0.01f)
	, m_fJSpeed0(0.15f)
	, m_fJSpeed(0.3f)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	m_fTimeDelta = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransCom->Set_Pos(20.5f, 1.f, 20.5f);

	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pTransCom->Set_Scale(0.4f, 0.4f, 0.4f);
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

	m_fTimeDelta = fTimeDelta;

	if (!(GetKeyState(VK_TAB) & 0x80))		// Except Open Inventory
	{
		Mouse_Move();
	}

	Key_Input(fTimeDelta);
	Jump(fTimeDelta);
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

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

			if (fDistFromBlock < sqrtf(2.8f))
				m_bBlockChanged = false;
			else if (fDistFromBlock > sqrtf(2.8f) && fDistFromBlock < sqrtf(2.9f))
			{
				m_bBlockChanged = true;
			}
			else if (fDistFromBlock > sqrtf(2.9f))
				m_pCurrentBlock = nullptr;
		}
	}
}

void CPlayer::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

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

	// �������� ���� ������Ʈ
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// �ؽ��� �İ�ü ���� ������Ʈ
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	// ������� ��ȯ Ʈ������ ������Ʈ
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// ����Ÿ��� ��� ������Ʈ
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
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)	
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_vDirection *= -1.f;
		m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));
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
}

void CPlayer::Mouse_Move(void)
{
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

	if ((Engine::Get_DIKeyState(DIK_X) & 0x80) && (0.3f<m_fLBClick))
	{
		m_fLBClick = 0.f;
		_vec3 vPos, vLook;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		m_pTransCom->Get_Info(INFO_LOOK, &vLook);

		CBulletMgr::GetInstance()->Fire(BULLET_WAND);
	}
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

	return m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), iWidth, iDepth) + 1 * m_fScale;
}

void CPlayer::CollisionEvent(CGameObject * pOtherObj)
{
	CItem*	pItem = dynamic_cast<CItem*>(pOtherObj);
	if (nullptr != pItem)
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

			if (m_bBlockChanged && m_pCurrentBlock != nullptr && m_pCurrentBlock != pBlock)
			{
				m_pCurrentBlock = pBlock;
				m_bBlockChanged = false;
				_vec3	PlayerPos;
				m_pTransCom->Get_Info(INFO_POS, &PlayerPos);
				_vec3 vTemp = { 0.f, fDistY, 0.f };
				_float	fBlockHeight = pBlock->Get_Height();
				m_pTransCom->Set_Y(fBlockHeight + (PlayerPos.y - PlayerBox.vMin.y));
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

		if (fabs(fDistX) > fabs(fDistZ))
		fDistX = 0.f;
		else if (fabs(fDistX) < fabs(fDistZ))
		fDistZ = 0.f;

		_vec3	PlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &PlayerPos);

		_vec3	PlayerLook;
		m_pTransCom->Get_Info(INFO_LOOK, &PlayerLook);
		PlayerLook.y = 0.f;

		m_pTransCom->Set_Pos(PlayerPos.x + fDistX, PlayerPos.y, PlayerPos.z + fDistZ);
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
