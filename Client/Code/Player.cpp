#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_Function.h"
#include "BulletMgr.h"
#include "Inventory.h"
#include "InvImg.h"
#include "ImGuiMgr.h"
#include "ItemMgr.h"
#include "Block.h"

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
	m_pTransCom->Set_Pos(20.f, 1.f, 20.f);

	// 충돌처리 테스트
	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z };


	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	if (!(GetKeyState(VK_TAB) & 0x80))
	{
		Mouse_Move();
	}
	Key_Input(fTimeDelta);
	Jump(fTimeDelta);

	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	vScale = { 0.5f, 0.9f, 0.5f };
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	m_bdSphere.vCenter = vPos;
	m_bdSphere.fRadius = vScale.x;

	//D3DXVec3TransformCoord(&m_bdBox.vMin, &m_bdBox.vMin, &matWorld);
	//D3DXVec3TransformCoord(&m_bdBox.vMax, &m_bdBox.vMax, &matWorld);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this); // TestPlayer를 렌더그룹에 포함
	return 0;
}

void CPlayer::LateUpdate_Object(void)
{
	// 충돌처리 테스트
	_vec3 vPos, vScale;
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z };

	Mouse_Click(m_fTimeDelta);

	if (m_bdBox.vMin.y < 0.1f)
	{
		Set_OnTerrain();
	}
	else
	{
		_vec3	vPlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &vPlayerPos);
		vPlayerPos.y = m_bdBox.vMin.y;

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

			/*if (m_bBlockChanged)
			{
			m_bBlockChanged = false;
			return;
			}*/

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

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	// TestPlayer의 버퍼 컴포넌트를 출력 후 원래의 셋팅을 복원해야만 다른 오브젝트에 반영되지 않는다.
	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// 렌더링할 버퍼 컴포넌트
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	// 텍스쳐 컴객체 관리 컴포넌트
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_PlayerTexture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	// 월드행렬 변환 트랜스폼 컴포넌트
	pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

	// 지형타기용 계산 컴포넌트
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pTransCom->Get_Info(INFO_LOOK, &m_vDirection);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)	// 전진
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)	// 후진
	{
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)	// 왼쪽 게걸음
	{
		m_pTransCom->Get_Info(INFO_RIGHT, &m_vDirection);
		m_vDirection.y = 0.f;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
		m_pTransCom->Move_Pos(&(m_vDirection * -m_fSpeed * fTimeDelta));
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)	// 오른쪽 게걸음
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

}

void CPlayer::Mouse_Move(void)
{
	_long	dwMouseMove = 0;

	// 좌우회전은 대부분 y축기준으로 회전
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
	//if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
	m_fLBClick += fTimeDelta;

	if ((Engine::Get_DIKeyState(DIK_X) & 0x80) && (0.3f<m_fLBClick))
	{
		m_fLBClick = 0.f;
		_vec3 vPos, vLook;
		m_pTransCom->Get_Info(INFO_POS, &vPos);
		m_pTransCom->Get_Info(INFO_LOOK, &vLook);

		CBulletMgr::GetInstance()->Fire(BULLET_WAND);
		//CBulletMgr::GetInstance()->Reuse_Obj(vPos, vLook);


	}
}

void CPlayer::Set_OnTerrain(void)
{
	//m_bJump = false;

	_vec3		vPos;
	m_pTransCom->Get_Info(INFO_POS, &vPos);

	//Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	//NULL_CHECK(pTerrainTexCom);

	//_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	_float fHeight = Get_Height();

	m_pTransCom->Set_Y(fHeight);
}

void CPlayer::Jump(const _float & fTimeDelta)
{
	if (m_bJump)
	{
		_vec3 vPos;
		m_pTransCom->Get_Info(INFO_POS, &vPos);


		/*Engine::CTerrainTex*	pTerrainTexCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
		NULL_CHECK(pTerrainTexCom);

		_float fHeight = m_pCalculatorCom->HeightOnTerrain(&vPos, pTerrainTexCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);*/
		_float fHeight = Get_Height();

		if (m_fJTimeDelta > 2.f && 0.f >= m_bdBox.vMin.y)
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
			//_float fDist = fHeight + m_fJSpeed0*m_fJTimeDelta - 0.5 * m_fAccel * m_fJTimeDelta * m_fJTimeDelta;
			//_float fVelocity = m_fJSpeed0 - m_fAccel;
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

	// 충돌한 물체가 아이템이라면 인벤토리에 넣어야 함.
	// 획득한 아이템은 INV상태가 되고, 이 상태일 경우 모든 연산(렌더링포함)에서 빠지게 된다.
	// 아이템 획득 시, INV에 들어갈 객체를 따로 생성한다.
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
		// 구 충돌
		/*_vec3	BlockPos = pBlock->Get_bdSphere()->vCenter;
		_float fDistance = (m_bdSphere.fRadius + pBlock->Get_bdSphere()->fRadius) -
		sqrtf(pow(BlockPos.x - m_bdSphere.vCenter.x, 2) +
		pow(BlockPos.y - m_bdSphere.vCenter.y, 2) +
		pow(BlockPos.z - m_bdSphere.vCenter.z, 2));

		_vec3	PlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &PlayerPos);

		_vec3	PlayerLook;
		m_pTransCom->Get_Info(INFO_LOOK, &PlayerLook);

		_vec3 vDir = PlayerLook - PlayerPos;
		PlayerLook *= -fDistance * m_fSpeed * 2.f * m_fTimeDelta;
		PlayerLook.y = 0.f;
		vDir *= m_fSpeed * m_fTimeDelta;
		m_pTransCom->Move_Pos(&PlayerLook);*/

		// AABB
		BDBOX	*	BlockBox = pBlock->Get_bdBox();

		_float		fDistX = 0.f;
		_float		fDistY = 0.f;
		_float		fDistZ = 0.f;

		// 플레이어가 박스 왼쪽
		if (m_bdBox.vMax.x > BlockBox->vMin.x && m_bdBox.vMax.x < BlockBox->vMax.x)
		{
			fDistX = BlockBox->vMin.x - m_bdBox.vMax.x;
		}

		// 플레이어가 박스 오른쪽
		else if (m_bdBox.vMin.x < BlockBox->vMax.x && m_bdBox.vMin.x > BlockBox->vMin.x)
		{
			fDistX = BlockBox->vMax.x - m_bdBox.vMin.x;
		}

		// 플레이어가 박스 앞쪽
		if (m_bdBox.vMin.z < BlockBox->vMax.z  && m_bdBox.vMin.z > BlockBox->vMin.z)
		{
			fDistZ = BlockBox->vMax.z - m_bdBox.vMin.z;
		}

		// 플레이어가 박스 뒤쪽
		else if (m_bdBox.vMax.z > BlockBox->vMin.z && m_bdBox.vMax.z < BlockBox->vMax.z)
		{
			fDistZ = BlockBox->vMin.z - m_bdBox.vMax.z;
		}

		// 플레이어가 박스 위쪽
		if (m_bdBox.vMin.y < BlockBox->vMax.y && m_bdBox.vMin.y > BlockBox->vMin.y && m_eState != PLAYER_GROUND)
		{

			m_eState = PLAYER_ON_BLOCK;
			m_bJump = false;
			fDistX = 0.f;
			fDistY = BlockBox->vMax.y - m_bdBox.vMin.y;
			fDistZ = 0.f;

			if (m_bBlockChanged && m_pCurrentBlock != nullptr && m_pCurrentBlock != pBlock)
			{
				m_pCurrentBlock = pBlock;
				m_bBlockChanged = false;
				_vec3	PlayerPos;
				m_pTransCom->Get_Info(INFO_POS, &PlayerPos);
				_vec3 vTemp = { 0.f, fDistY, 0.f };
				//m_pTransCom->Set_Pos(PlayerPos.x, PlayerPos.y + fDistY, PlayerPos.z);
				_float	fBlockHeight = pBlock->Get_Height();
				m_pTransCom->Set_Y(fBlockHeight + (PlayerPos.y - m_bdBox.vMin.y));

				//m_pTransCom->Move_Pos(&vTemp);
				return;
			}

			m_pCurrentBlock = pBlock;
			fDistY = 0.f;
		}

		// 플레이어가 박스 아래쪽
		else if (m_bdBox.vMax.y > BlockBox->vMin.y && m_bdBox.vMax.y < BlockBox->vMax.y)
		{
			fDistY = BlockBox->vMin.y - m_bdBox.vMax.y;
			m_fJSpeed = 0.f;
		}

		/*if (fabs(fDistX) > fabs(fDistZ))
		fDistZ = 0.f;
		else if (fabs(fDistX) <= fabs(fDistZ))
		fDistX = 0.f;*/

		_vec3	PlayerPos;
		m_pTransCom->Get_Info(INFO_POS, &PlayerPos);

		_vec3	PlayerLook;
		m_pTransCom->Get_Info(INFO_LOOK, &PlayerLook);

		_vec3 vDir = { fDistX, fDistY, fDistZ };
		vDir *= m_fSpeed * m_fTimeDelta;
		m_pTransCom->Move_Pos(&vDir);
		//m_pTransCom->Set_Pos(PlayerPos.x + fDistX, PlayerPos.y + fDistY, PlayerPos.z + fDistZ);
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
