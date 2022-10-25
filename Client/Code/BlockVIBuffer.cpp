#include "stdafx.h"
#include "BlockVIBuffer.h"
#include "CameraMgr.h"

IMPLEMENT_SINGLETON(CBlockVIBuffer)

CBlockVIBuffer::CBlockVIBuffer()
	: m_dwVtxSize(0)
	, m_dwFVF(0)
	, m_dwIdxSize(0)	
{
	ppBPT = new BPT*[BLOCKTYPE_END];
	ppBPT[BLOCK_CAVE]		= new BPT[CAVETEX_CNT];
	ppBPT[BLOCK_COLD]		= new BPT[COLDTEX_CNT];
	ppBPT[BLOCK_DUNGEON]	= new BPT[DUNGEONTEX_CNT];
	ppBPT[BLOCK_ROOM]		= new BPT[ROOMTEX_CNT];
	ppBPT[BLOCK_SEWER]		= new BPT[SEWERTEX_CNT];
	ppBPT[BLOCK_TEMPLE]		= new BPT[TEMPLETEX_CNT];

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}


CBlockVIBuffer::~CBlockVIBuffer()
{
	Free();
}

HRESULT CBlockVIBuffer::Add_Instancing(BLOCKTYPE eType, CTexture* pTextureCom, _int iTexture, CTransform* pTransformCom)
{
	NULL_CHECK_RETURN(pTextureCom, E_FAIL);

	BPT*	pBPT = &ppBPT[eType][iTexture];
	pBPT->m_dwBlockCnt++;
	pBPT->m_dwVtxCnt = 8 * pBPT->m_dwBlockCnt;
	pBPT->m_dwTriCnt = 12 * pBPT->m_dwBlockCnt;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	pBPT->m_vecMatWorld.push_back(pTransformCom);

	if (pBPT->m_pTexture == nullptr)
		pBPT->m_pTexture = pTextureCom->Get_Texture()[iTexture];

	return S_OK;
}

HRESULT CBlockVIBuffer::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, BLOCKTYPE eType, _int iTexture)
{
	BPT*	pBPT = &ppBPT[eType][iTexture];
	if (pBPT->m_dwBlockCnt == 0)
		return E_FAIL;
	
	FAILED_CHECK_RETURN(pGraphicDev->CreateVertexBuffer(
		pBPT->m_dwVtxCnt * m_dwVtxSize,
		0,
		m_dwFVF,
		D3DPOOL_MANAGED,
		&pBPT->m_pVB,
		nullptr)
		, E_FAIL);

	FAILED_CHECK_RETURN(pGraphicDev->CreateIndexBuffer(
		m_dwIdxSize * pBPT->m_dwTriCnt,
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&pBPT->m_pIB,
		nullptr)
		, E_FAIL);

	VTXCUBE*		pVertex = nullptr;
	_ulong			dwIndex = 0;

	pBPT->m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < pBPT->m_dwBlockCnt; ++i)
	{
		dwIndex = i * 8;

		// FRONT
		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(-1.f, 1.f, -1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { -1.f, 1.f, -1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(1.f, 1.f, -1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { 1.f, 1.f, -1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(1.f, -1.f, -1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { 1.f, -1.f, -1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(-1.f, -1.f, -1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { -1.f, -1.f, -1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		// BACK
		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(-1.f, 1.f, 1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { -1.f, 1.f, 1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(1.f, 1.f, 1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { 1.f, 1.f, 1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(1.f, -1.f, 1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { 1.f, -1.f, 1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;

		pVertex[dwIndex].vPos = *D3DXVec3TransformCoord(&pVertex[dwIndex].vPos, &_vec3(-1.f, -1.f, 1.f), pBPT->m_vecMatWorld[i]->Get_WorldMatrixPointer());
		pVertex[dwIndex].vTexUV = { -1.f, -1.f, 1.f };
		D3DXVec3Normalize(&pVertex[dwIndex].vNormal, &pVertex[dwIndex].vPos);
		++dwIndex;
	}

	pBPT->m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;
	dwIndex = 0;

	pBPT->m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < pBPT->m_dwBlockCnt; ++i)
	{
		pIndex[dwIndex]._0 = i * 8 + 1;
		pIndex[dwIndex]._1 = i * 8 + 5;
		pIndex[dwIndex]._2 = i * 8 + 6;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8 + 1;
		pIndex[dwIndex]._1 = i * 8 + 6;
		pIndex[dwIndex]._2 = i * 8 + 2;
		++dwIndex;

		pIndex[dwIndex]._0 = i * 8 + 4;
		pIndex[dwIndex]._1 = i * 8;
		pIndex[dwIndex]._2 = i * 8 + 3;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8 + 4;
		pIndex[dwIndex]._1 = i * 8 + 3;
		pIndex[dwIndex]._2 = i * 8 + 7;
		++dwIndex;

		pIndex[dwIndex]._0 = i * 8 + 4;
		pIndex[dwIndex]._1 = i * 8 + 5;
		pIndex[dwIndex]._2 = i * 8 + 1;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8 + 4;
		pIndex[dwIndex]._1 = i * 8 + 1;
		pIndex[dwIndex]._2 = i * 8;
		++dwIndex;

		pIndex[dwIndex]._0 = i * 8 + 3;
		pIndex[dwIndex]._1 = i * 8 + 2;
		pIndex[dwIndex]._2 = i * 8 + 6;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8 + 3;
		pIndex[dwIndex]._1 = i * 8 + 6;
		pIndex[dwIndex]._2 = i * 8 + 7;
		++dwIndex;

		pIndex[dwIndex]._0 = i * 8 + 7;
		pIndex[dwIndex]._1 = i * 8 + 6;
		pIndex[dwIndex]._2 = i * 8 + 5;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8 + 7;
		pIndex[dwIndex]._1 = i * 8 + 5;
		pIndex[dwIndex]._2 = i * 8 + 4;
		++dwIndex;

		pIndex[dwIndex]._0 = i * 8;
		pIndex[dwIndex]._1 = i * 8 + 1;
		pIndex[dwIndex]._2 = i * 8 + 2;
		++dwIndex;
		pIndex[dwIndex]._0 = i * 8;
		pIndex[dwIndex]._1 = i * 8 + 2;
		pIndex[dwIndex]._2 = i * 8 + 3;
		++dwIndex;
	}

	pBPT->m_pIB->Unlock();

	return S_OK;
}

void CBlockVIBuffer::Render_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, BLOCKTYPE eType, _int iTexture)
{
	BPT*	pBPT = &ppBPT[eType][iTexture];
	if (pBPT->m_pVB == nullptr)
		return;

	CCamera* pObj = nullptr;
	
	pObj = CCameraMgr::GetInstance()->Get_CurCamera();

	if (!pObj)
		return;

	D3DXMatrixIdentity(&m_matProj);
	pObj->Get_ViewMatrix(m_matView);
	pObj->Get_ProjectMatrix(m_matProj);

	_matrix	matWorld = *D3DXMatrixIdentity(&matWorld);

	pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//// Set Material
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	if (g_vPlayerPos.x < 0 || g_vPlayerPos.z < 0)
	{
		g_fAmbient -= 0.001f;
		if (0.2f >= g_fAmbient)
			g_fAmbient = 0.2f;
		tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // 환경반사
	}
	else
	{
		g_fAmbient += 0.01f;
		if (1.f <=  g_fAmbient)
			g_fAmbient = 1.f;
		tMtrl.Ambient = D3DXCOLOR(g_fAmbient, g_fAmbient, g_fAmbient, 1.f); // 환경반사
	}
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	pGraphicDev->SetMaterial(&tMtrl);
	//// *Set Material
	
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetTexture(0, pBPT->m_pTexture);
	pGraphicDev->SetStreamSource(0, pBPT->m_pVB, 0, m_dwVtxSize);
	pGraphicDev->SetFVF(m_dwFVF);
	pGraphicDev->SetIndices(pBPT->m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pBPT->m_dwVtxCnt, 0, pBPT->m_dwTriCnt);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CBlockVIBuffer::Free()
{
	//for (int i = 0; i < CAVETEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_CAVE];
		ppBPT[BLOCK_CAVE] = nullptr;
	}
	//for (int i = 0; i < COLDTEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_COLD];
		ppBPT[BLOCK_COLD] = nullptr;
	}
	//for (int i = 0; i < DUNGEONTEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_DUNGEON];
		ppBPT[BLOCK_DUNGEON] = nullptr;
	}
	//for (int i = 0; i < ROOMTEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_ROOM];
		ppBPT[BLOCK_ROOM] = nullptr;
	}
	//for (int i = 0; i < SEWERTEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_SEWER];
		ppBPT[BLOCK_SEWER] = nullptr;
	}
	//for (int i = 0; i < TEMPLETEX_CNT; ++i)
	{
		delete[] ppBPT[BLOCK_TEMPLE];
		ppBPT[BLOCK_TEMPLE] = nullptr;
	}
	//for (int i = 0; i < BLOCKTYPE_END; ++i)
	{
		delete[] ppBPT;
		ppBPT = nullptr;
	}

	Safe_Release(m_pGraphicDev);
}
