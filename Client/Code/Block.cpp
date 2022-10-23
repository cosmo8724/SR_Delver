#include "stdafx.h"
#include "..\Header\Block.h"
#include "../Default/ImGui/ImGuizmo.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "MiniMap.h"
#include "BlockVIBuffer.h"
#include "Player.h"
#include "CullingMgr.h"


CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bSet(false)
	, m_bClone(false)
{
}

CBlock::CBlock(const CBlock& rhs)
	: CGameObject(rhs)
	, m_bSet(true)
	, m_bClone(true)
	, m_eCurrentType(rhs.m_eCurrentType)
	, m_eLastType(rhs.m_eLastType)
	, m_fScale(rhs.m_fScale)
	, m_iTexture(rhs.m_iTexture)
{
	m_pBufferCom = dynamic_cast<CCubeTex*>(rhs.m_pBufferCom->Clone());
	m_pTransCom = dynamic_cast<CTransform*>(rhs.m_pTransCom->Clone());
	m_pCalculatorCom = dynamic_cast<CCalculator*>(rhs.m_pCalculatorCom->Clone());

	m_mapComponent[ID_STATIC].insert({ L"Proto_BlockTexCom", m_pBufferCom });

	switch (m_eCurrentType)
	{
	case BLOCK_CAVE:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cave_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Cave_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_COLD:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cold_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Cold_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_DUNGEON:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Dungeon_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Dungeon_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_ROOM:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Room_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Room_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_SEWER:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Sewer_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Sewer_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_TEMPLE:
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Temple_BlockTexture"));
		m_mapComponent[ID_STATIC].insert({ L"Proto_Temple_BlockTexture", m_pTextureCom });
		break;
	}
	
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", m_pTransCom });
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", m_pCalculatorCom });

	_matrix	matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	if (!g_bIsTool)
		CBlockVIBuffer::GetInstance()->Add_Instancing(m_eCurrentType, m_pTextureCom, m_iTexture, m_pTransCom);
}

CBlock::~CBlock()
{
}

HRESULT CBlock::Ready_Object(_vec3* vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (vPos)
		m_pTransCom->Set_Pos(vPos->x, vPos->y, vPos->z);

	m_pTransCom->Update_Component(10.f);

	m_matOriginWorld = *m_pTransCom->Get_WorldMatrixPointer();

	return S_OK;
}

_int CBlock::Update_Object(const _float & fTimeDelta)
{
	if (Check_ParentDead())
		return 1;

	m_fTimeDelta = fTimeDelta;

	/*if (!m_bClone)
	{
		CTerrainTex*	pTerrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));

		_float TerrainSize = fabs(pTerrainTex->Get_VtxPos()[0].x - pTerrainTex->Get_VtxPos()[1].x);
		_float	CubeSize = fabs(m_pBufferCom->Get_VtxPos()[0].x - m_pBufferCom->Get_VtxPos()[1].x);
		m_fScale = TerrainSize / CubeSize;

		m_pTransCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	}*/
	//m_fScale = 0.5f;
	if (m_pParentBlock)
	{
		m_iTexture = m_pParentBlock->GetTextureIndex();
		m_pTransCom->Set_WorldMatrix(&m_matOriginWorld);
		MultiParentWorld();
	}

	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	m_bdSphere.vCenter = vPos;
	m_bdSphere.fRadius = vScale.x;

	//if (!m_bSet)
	//	Chase_MousePT();

	if (!m_bClone)
		Chase_Block();

	Change_BlockType();

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bClone)
	{
		if (!m_bCreateIcon)
		{
			// Only For Stage
			/*CMiniMap* pMiniMap = dynamic_cast<CMiniMap*>(Engine::Get_GameObject(L"Layer_UI", L"UI_MiniMap"));
			pMiniMap->Add_Icon(m_pGraphicDev, this);
			m_bCreateIcon = true;*/
		}
		m_pColliderCom->Calculate_WorldMatrix(*m_pTransCom->Get_WorldMatrixPointer());

		if (g_bIsTool)
		{
			if (CCullingMgr::GetInstance()->Is_Inside(this))
				Add_RenderGroup(RENDER_NONALPHA, this);
		}
	}
	else
	{
		if (g_bIsTool)
			Add_RenderGroup(RENDER_ALPHA, this);
	}
	return 0;
}

void CBlock::LateUpdate_Object(void)
{
	if (Check_ParentDead())
		return;

	Engine::CGameObject::LateUpdate_Object();
}

void CBlock::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	if (m_bSet && !m_bChanging)
	{
		if (g_bIsTool)
		{
			m_pTextureCom->Set_Texture(m_iTexture);
			m_pBufferCom->Render_Buffer();
		}
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);


		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(90, 0, 0, 0));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pTextureCom->Set_Texture(m_iTexture);


		// 재질 /////////////
		D3DMATERIAL9		tMtrl;
		ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

		tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); // 원색
		tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tMtrl.Ambient = D3DXCOLOR(1.f,1.f,1.f, 1.f); // 환경반사

														  // 수행 x
		tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		tMtrl.Power = 0.f;

		m_pGraphicDev->SetMaterial(&tMtrl);
		// ~재질 /////////////

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CBlock::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	if (!m_bClone)
	{
		pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_BlockTexCom"));
		NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_BlockTexCom", pComponent });

		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cave_BlockTexture"));
		NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_Cave_BlockTexture", pComponent });

		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });

		pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_CalculatorCom"));
		NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", pComponent });
	}
	// Collider Component
	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	return S_OK;
}

void CBlock::CollisionEvent(CGameObject * pOtherObj)
{
	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(pOtherObj);
	if (pPlayer)
	{
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
		CCollider*		pPlayerCollider = dynamic_cast<CCollider*>(pPlayer->Get_Component(L"Proto_ColliderCom", ID_STATIC));
		BDBOX			tPlayerBox	= { pPlayerCollider->Get_MinPoint(), pPlayerCollider->Get_MaxPoint() };
		BDBOX			tBlockBox		= { m_pColliderCom->Get_MinPoint(), m_pColliderCom->Get_MaxPoint() };

		_vec3			vPlayerPos = pPlayerTransform->Get_Pos();
		if (vPlayerPos.x > tBlockBox.vMin.x && vPlayerPos.x < tBlockBox.vMax.x
			&& vPlayerPos.z > tBlockBox.vMin.z && vPlayerPos.z < tBlockBox.vMax.z)
		{
			if (vPlayerPos.y < tBlockBox.vMax.y + (tBlockBox.vMax.y - tBlockBox.vMin.y)
				&& vPlayerPos.y > m_pTransCom->Get_Pos().y)
				pPlayer->Set_CurBlock(this);
			
			//return;
		}

		_float			fDistX = 0.f, fDistY = 0.f, fDistZ = 0.f;

		fDistY = 0.f;

		//if (fabs(fDistY) < 0.01f || pPlayer->Get_CurState() == PLAYER_JUMP)
		//	return;

		if (tPlayerBox.vMin.y >= m_pTransCom->Get_Pos().y)
			return;

		_float		fDist = D3DXVec3Length(&(m_pTransCom->Get_Pos() - pPlayerTransform->Get_Pos()));
		

		// Player at Leftside
		if (tPlayerBox.vMax.x > tBlockBox.vMin.x && tPlayerBox.vMax.x < tBlockBox.vMax.x)
		{
			fDistX = tBlockBox.vMin.x - tPlayerBox.vMax.x;
		}

		// Player at Rightside
		else if (tPlayerBox.vMin.x < tBlockBox.vMax.x && tPlayerBox.vMin.x > tBlockBox.vMin.x)
		{
			fDistX = tBlockBox.vMax.x - tPlayerBox.vMin.x;
		}

		// Player at Frontside
		if (tPlayerBox.vMin.z < tBlockBox.vMax.z  && tPlayerBox.vMin.z > tBlockBox.vMin.z)
		{
			fDistZ = tBlockBox.vMax.z - tPlayerBox.vMin.z;
		}

		// Player at Backside
		else if (tPlayerBox.vMax.z > tBlockBox.vMin.z && tPlayerBox.vMax.z < tBlockBox.vMax.z)
		{
			fDistZ = tBlockBox.vMin.z - tPlayerBox.vMax.z;
		}

		// Player On Block		-> Player CollisionEvent
		// Player Under Block	-> Player CollisionEvent
		if (tPlayerBox.vMax.y > tBlockBox.vMin.y && tPlayerBox.vMax.y < m_pTransCom->Get_Pos().y)
		{
			_vec3	PlayerPos = pPlayerTransform->Get_Pos();
			if ((PlayerPos.x > tBlockBox.vMin.x && PlayerPos.x < tBlockBox.vMax.x)
				&& (PlayerPos.z > tBlockBox.vMin.z && PlayerPos.z < tBlockBox.vMax.z)
				&& pPlayer->Get_CurState() == PLAYER_JUMP)
			{
				fDistX = 0.f;
				fDistY = tBlockBox.vMin.y - tPlayerBox.vMax.y;
				fDistZ = 0.f;
				pPlayer->Set_JSpeed(0.f);
				//pPlayerTransform->Set_Pos(PlayerPos.x, PlayerPos.y + fDistY, PlayerPos.z);
			}
		}

		//if ((vPlayerPos.x < tBlockBox.vMin.x || vPlayerPos.x > tBlockBox.vMax.x)
		//	&& (vPlayerPos.z < tBlockBox.vMin.z || vPlayerPos.z > tBlockBox.vMax.z)
		//	&& fabs(fDistX) > 0.0001f && fabs(fDistZ) > 0.0001f)
		{
			//if (sqrtf(fDistX * fDistX + fDistZ * fDistZ) < fabs(tBlockBox.vMax.x - tBlockBox.vMin.x) * 0.2f)
			//	return;
			if (fabs(fDistX) > fabs(fDistZ))
				fDistX = 0.f;
			else if (fabs(fDistX) < fabs(fDistZ))
				fDistZ = 0.f;
		}
		//else
		//	return;

		_vec3	vDir = { fDistX, 0.f, fDistZ };

		//pPlayerTransform->Move_Pos(&(vDir * pPlayer->Get_CurSpeed() * m_fTimeDelta));
		pPlayerTransform->Set_Pos(vPlayerPos.x + fDistX, vPlayerPos.y + fDistY, vPlayerPos.z + fDistZ);
	}
}

void CBlock::Chase_MousePT()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );

	_vec3 vPickPos = m_pCalculatorCom->PickingOnTerrainIndex(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(vPickPos.x, vPickPos.y + m_fScale, vPickPos.z);
}

void CBlock::Chase_Block()
{
	_bool	bCheck = false;
	static _bool	bFirst = true;
	_vec3 vDistance, vCloseCubePos;
	static _float fDist = 0.f;
	CubePlane		CheckPlane = CUBEPLANE_END;
	CubePlane		LastPlane = CUBEPLANE_END;

	CLayer*		pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");
	CBlock*		pClosestBlock = nullptr;
	for (auto iter = pLayer->Get_mapGameObject()->begin(); iter != pLayer->Get_mapGameObject()->end(); ++iter)
	{
		CBlock* pGameObject = dynamic_cast<CBlock*>(iter->second);

		if (pGameObject)
		{
			if (!pGameObject->IsClone())
				continue;

			CheckPlane = m_pCalculatorCom->PickingOnCube(g_hWnd, pGameObject->m_pBufferCom, pGameObject->m_pTransCom, &bCheck, &vDistance);

			if (bCheck)
			{
				CDynamicCamera* pCamera = dynamic_cast<CDynamicCamera*>(Engine::Get_GameObject(L"Layer_Tool_Environment", L"DynamicCamera"));
				_vec3			vCameraPos = pCamera->Get_Eye();

				_vec3 ParentCubePos = pGameObject->m_pTransCom->Get_Pos();
				_float fDistance = D3DXVec3Length(&(ParentCubePos - vCameraPos));

				if (bFirst)
				{
					fDist = fDistance;
					pClosestBlock = pGameObject;
					vCloseCubePos = ParentCubePos;
					LastPlane = CheckPlane;
					bFirst = false;
					bCheck = false;
				}
				else
				{
					if (fDist < fDistance)
						continue;
					else
					{
						fDist = fDistance;
						pClosestBlock = pGameObject;
						vCloseCubePos = ParentCubePos;
						LastPlane = CheckPlane;
						bCheck = false;
					}
				}
			}
		}
	}

	_float	CubeSize = fabs(m_pBufferCom->Get_VtxPos()[0].x - m_pBufferCom->Get_VtxPos()[1].x);

	if (pClosestBlock && pClosestBlock->m_pParentBlock != nullptr)
	{
		_vec3	vGrandParentPos = pClosestBlock->m_pParentBlock->m_pTransCom->Get_Pos();
		vCloseCubePos.x += vGrandParentPos.x;
		vCloseCubePos.y += vGrandParentPos.y;
		vCloseCubePos.z += vGrandParentPos.z;
	}

	switch (LastPlane)
	{
	case FRONT_X:
		m_pTransCom->Set_Pos(vCloseCubePos.x + (2.f), vCloseCubePos.y, vCloseCubePos.z);
		break;

	case BACK_X:
		m_pTransCom->Set_Pos(vCloseCubePos.x - (2.f), vCloseCubePos.y, vCloseCubePos.z);
		break;

	case FRONT_Y:
		m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y + (2.f), vCloseCubePos.z);
		break;

	case BACK_Y:
		m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y - (2.f), vCloseCubePos.z);
		break;

	case FRONT_Z:
		m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y, vCloseCubePos.z + (2.f));
		break;

	case BACK_Z:
		m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y, vCloseCubePos.z - (2.f));
		break;
	}
	bFirst = true;
}

void CBlock::MultiParentWorld()
{
	_matrix	matWorld = *m_pTransCom->Get_WorldMatrixPointer();
	_matrix	matParentWorld = *m_pParentBlock->m_pTransCom->Get_WorldMatrixPointer();

	//D3DXMatrixIdentity(&matWorld);
	matWorld *= matParentWorld;

	//m_pTransCom->Set_WorldMatrix(&matWorld);
	_vec3	vScale, vAngle, vPos;
	D3DXQUATERNION qRot;

	//D3DXMatrixDecompose(&vScale, &qRot, &vPos, &matWorld);

	ImGuizmo::DecomposeMatrixToComponents(matWorld, vPos, vAngle, vScale);
	//memcpy(&m_pTransCom->m_vInfo[INFO_POS], vPos, sizeof(vPos));
	//memcpy(&m_pTransCom->m_vAngle, vAngle * D3DX_PI / 180.f, sizeof(vAngle));
	//memcpy(&m_pTransCom->m_vScale, vScale, sizeof(vScale));
	m_pTransCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransCom->Set_Scale(vScale.x, vScale.y, vScale.z);
	m_pTransCom->Set_Angle(vAngle.x * D3DX_PI / 180.f, vAngle.y * D3DX_PI / 180.f, vAngle.z * D3DX_PI / 180.f);
}

HRESULT CBlock::Change_BlockType()
{
	CComponent* pComponent = nullptr;

	if (m_eLastType != m_eCurrentType || (m_pParentBlock && m_eCurrentType != m_pParentBlock->GetBlockType()))
	{
		auto iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Cave_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}

		iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Cold_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}

		iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Dungeon_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}

		iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Room_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}

		iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Sewer_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}
		
		iter = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Temple_BlockTexture"));
		if (iter != m_mapComponent[ID_STATIC].end())
		{
			Safe_Release(iter->second);
			iter = m_mapComponent[ID_STATIC].erase(iter);
		}

		if (m_pParentBlock)
			m_eCurrentType = m_pParentBlock->GetBlockType();

		switch (m_eCurrentType)
		{
		case BLOCK_CAVE:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cave_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Cave_BlockTexture", pComponent });
			break;

		case BLOCK_COLD:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Cold_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Cold_BlockTexture", pComponent });
			break;

		case BLOCK_DUNGEON:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Dungeon_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Dungeon_BlockTexture", pComponent });
			break;

		case BLOCK_ROOM:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Room_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Room_BlockTexture", pComponent });
			break;

		case BLOCK_SEWER:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Sewer_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Sewer_BlockTexture", pComponent });
			break;

		case BLOCK_TEMPLE:
			pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Temple_BlockTexture"));
			NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
			m_mapComponent[ID_STATIC].insert({ L"Proto_Temple_BlockTexture", pComponent });
			break;

		case BLOCKTYPE_END:
			return E_FAIL;
		}
		m_eLastType = m_eCurrentType;
	}

	return S_OK;
}

CBlock * CBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* vPos)
{
	CBlock *	pInstance = new CBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CBlock * CBlock::Create(const CBlock & rhs)
{
	CBlock *	pInstance = new CBlock(rhs);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBlock::Free(void)
{
	if (!m_pParentBlock)
		m_bDeleted = true;
	CGameObject::Free();
}
