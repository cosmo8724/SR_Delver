#include "stdafx.h"
#include "..\Header\Block.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "MiniMap.h"


CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBlock::CBlock(const CBlock& rhs)
	: CGameObject(rhs)
	, m_bSet(true)
	, m_bClone(true)
	, m_eCurrentType(rhs.m_eCurrentType)
	, m_eLastType(rhs.m_eLastType)
	, m_iTexture(rhs.m_iTexture)
{
	//memcpy(&m_bdBox, &rhs.m_bdBox, sizeof(BDBOX));
	m_pBufferCom = dynamic_cast<CCubeTex*>(rhs.m_pBufferCom->Clone());
	m_pTransCom = dynamic_cast<CTransform*>(rhs.m_pTransCom->Clone());
	m_pTextureCom = dynamic_cast<CTexture*>(rhs.m_pTextureCom->Clone()); // Tool!!
	m_pCalculatorCom = dynamic_cast<CCalculator*>(rhs.m_pCalculatorCom->Clone());

	m_mapComponent[ID_STATIC].insert({ L"Proto_BlockTexCom", m_pBufferCom });

	switch (m_eCurrentType)
	{
	case BLOCK_CAVE:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Cave_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_COLD:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Cold_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_DUNGEON:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Dungeon_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_ROOM:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Room_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_SEWER:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Sewer_BlockTexture", m_pTextureCom });
		break;

	case BLOCK_TEMPLE:
		m_mapComponent[ID_STATIC].insert({ L"Proto_Temple_BlockTexture", m_pTextureCom });
		break;
	}
	
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", m_pTransCom });
	m_mapComponent[ID_STATIC].insert({ L"Proto_CalculatorCom", m_pCalculatorCom });
}

CBlock::~CBlock()
{
}

HRESULT CBlock::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	//m_pTransCom->Get_Scale(&vScale);
	//m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	//m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	return S_OK;
}

_int CBlock::Update_Object(const _float & fTimeDelta)
{
	if (!m_bClone)
	{
		CTerrainTex*	pTerrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));

		_float TerrainSize = fabs(pTerrainTex->Get_VtxPos()[0].x - pTerrainTex->Get_VtxPos()[1].x);
		_float	CubeSize = fabs(m_pBufferCom->Get_VtxPos()[0].x - m_pBufferCom->Get_VtxPos()[1].x);
		m_fScale = TerrainSize / CubeSize;

		m_pTransCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	}

	_vec3 vPos, vScale;
	m_pTransCom->Get_Info(INFO_POS, &vPos);
	vScale = m_pTransCom->Get_Scale();
	m_bdBox.vMin = { vPos.x - vScale.x, vPos.y - vScale.y, vPos.z - vScale.z };
	m_bdBox.vMax = { vPos.x + vScale.x, vPos.y + vScale.y, vPos.z + vScale.z };

	m_bdSphere.vCenter = vPos;
	m_bdSphere.fRadius = vScale.x;

	if (!m_bSet)
		Chase_MousePT();

	if (!m_bClone)
	{
		_bool	bCheck = false;
		static _bool	bFirst = true;
		_vec3 vDistance, vCloseCubePos;
		static _float fDist = 0.f;
		CubePlane		CheckPlane = CUBEPLANE_END;
		CubePlane		LastPlane = CUBEPLANE_END;

		CLayer*		pLayer = Engine::Get_Layer(L"Layer_Tool_GameLogic");
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
					_float fDistance = D3DXVec3Length(&(ParentCubePos - vCameraPos));	// 카메라 위치부터 큐브까지 거리

					if (bFirst)
					{
						fDist = fDistance;
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
							vCloseCubePos = ParentCubePos;
							LastPlane = CheckPlane;
							bCheck = false;
						}
					}
				}
			}
		}

		_float	CubeSize = fabs(m_pBufferCom->Get_VtxPos()[0].x - m_pBufferCom->Get_VtxPos()[1].x);

		switch (LastPlane)
		{
		case FRONT_X:
			m_pTransCom->Set_Pos(vCloseCubePos.x + (CubeSize * m_fScale), vCloseCubePos.y, vCloseCubePos.z);
			break;

		case BACK_X:
			m_pTransCom->Set_Pos(vCloseCubePos.x - (CubeSize * m_fScale), vCloseCubePos.y, vCloseCubePos.z);
			break;

		case FRONT_Y:
			m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y + (CubeSize * m_fScale), vCloseCubePos.z);
			break;

		case BACK_Y:
			m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y - (CubeSize * m_fScale), vCloseCubePos.z);
			break;

		case FRONT_Z:
			m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y, vCloseCubePos.z + (CubeSize * m_fScale));
			break;

		case BACK_Z:
			m_pTransCom->Set_Pos(vCloseCubePos.x, vCloseCubePos.y, vCloseCubePos.z - (CubeSize * m_fScale));
			break;
		}
		bFirst = true;
	}

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
		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	else
		Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBlock::LateUpdate_Object(void)
{
	

	//Engine::CGameObject::LateUpdate_Object();
}

void CBlock::Render_Obejct(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (m_bSet && !m_bChanging)
	{
		m_pTextureCom->Set_Texture(m_iTexture);
		m_pBufferCom->Render_Buffer();
	}
	else
	{
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

void CBlock::Chase_MousePT()
{
	CTerrainTex*	pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TerrainTexCom", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, );

	CTransform*		pTerrainTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Tool_Environment", L"Terrain", L"Proto_TransformCom", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, );

	_vec3 vPickPos = m_pCalculatorCom->PickingOnTerrainIndex(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

	m_pTransCom->Set_Pos(vPickPos.x, vPickPos.y + m_fScale, vPickPos.z);
}

HRESULT CBlock::Change_BlockType()
{
	CComponent* pComponent = nullptr;

	if (m_eLastType != m_eCurrentType)
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

CBlock * CBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlock *	pInstance = new CBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
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
	/*if (m_bClone)
	{
		Safe_Release(m_pBufferCom);
		Safe_Release(m_pTransCom);
		Safe_Release(m_pTextureCom);
		Safe_Release(m_pCalculatorCom);
	}*/
	CGameObject::Free();
}
