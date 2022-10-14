#include "..\..\Header\TileTex.h"
#include "Export_Function.h"

USING(Engine)

CTileTex::CTileTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev), m_bClone(false), m_pPos(nullptr)
{
}


CTileTex::CTileTex(const CTileTex& rhs)
	: CVIBuffer(rhs)
	, m_bClone(true)
	, m_pPos(rhs.m_pPos)
{
}

CTileTex::~CTileTex()
{
}

HRESULT CTileTex::Ready_Buffer(const _ulong & dwVtxItv)
{
	m_dwVtxCnt = 4;
	m_pPos = new _vec3[m_dwVtxCnt];
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	_ulong	dwIndex = 0;

	VTXTEX*		pVertex = nullptr;

	//for (_ulong i = 0; i < 2; ++i)
	//{
	//	for (_ulong j = 0; j < 2; ++j)
	//	{
	//		dwIndex = i * 2 + j;

	//		pVertex[dwIndex].vPos = { _float(j) * dwVtxItv, 
	//								 0.f, //(pPixel[dwIndex] & 0x000000ff) / 20.f,  
	//								_float(i) * dwVtxItv };

	//		m_pPos[dwIndex] = pVertex[dwIndex].vPos;
	//		pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

	//		pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 20.f, 
	//									_float(i) / (dwCntZ - 1) * 20.f };
	//	}
	//}

	return S_OK;
}

void CTileTex::Render_Buffer(void)
{
}

CVIBuffer * CTileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong & dwVtxItv)
{
	return nullptr;
}

CComponent * CTileTex::Clone(void)
{
	return nullptr;
}

void CTileTex::Free(void)
{
}
