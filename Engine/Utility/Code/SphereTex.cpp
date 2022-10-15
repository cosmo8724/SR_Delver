#include "..\..\Header\SphereTex.h"
#include "Export_Function.h"

USING(Engine)

CSphereTex::CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}


Engine::CSphereTex::CSphereTex(const CSphereTex& rhs)
	: CVIBuffer(rhs)
{

}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer(_vec3 vCenterPos, _float _fRadius, _ulong dwColor)
{
	_float	fRadius = _fRadius;         // 구의 반지름
	UINT32  StackCount = 15;	   // 가로 분할
	UINT32  SliceCount = 15;       // 세로 분할 
	
	_float  fStackStep = D3DX_PI / StackCount;
	_float  fSliceStep = 2 * D3DX_PI / SliceCount;

	_float	fDeltaU = 1.f / static_cast<_float>(SliceCount);
	_float	fDeltaV = 1.f / static_cast<_float>(StackCount);

	_int	iNumber_of_Vertices = 2 + SliceCount * (StackCount - 1);
	_int	iNumber_of_Faces = 2 * SliceCount + (StackCount - 2) * (2 * SliceCount);

	_float	fTheta_Step, fTheta, fSin_Theta, fCos_Theta;
	_float	fPhi_Step, fPhi_Start; 

	_int	iCurVertex = 0;
	_uint	iSlice, iStack;
	_int    iFace = 0;
	iStack = 0;

	//////////////////////////////////////////////////////////////////////////////
	m_dwVtxCnt = iNumber_of_Vertices;
	m_dwTriCnt = iNumber_of_Faces;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);
	
	VTXCOL*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	INDEX16*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	//////////////////////////////////////////////////////////////////////////////

	fPhi_Step = -2 * D3DX_PI / SliceCount;
	fPhi_Start = D3DX_PI / 2;

	fTheta_Step = D3DX_PI / StackCount;
	fTheta = fTheta_Step;

	pVertex[iCurVertex].vPos = { 0.0f, 0.0f, fRadius };
	// Color
	pVertex[iCurVertex].dwColor = dwColor;// D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	iCurVertex++;

#pragma region rere

	for (iStack = 0; iStack < StackCount - 1 ; ++iStack)
	{	
		//_float fPhi = y * fStackStep;
		fSin_Theta = sinf(fTheta);
		fCos_Theta = cosf(fTheta);
		
		for (iSlice = 0; iSlice < SliceCount; ++iSlice)
		{
			_float fX = (_float)(fRadius * fSin_Theta * cosf(fPhi_Start));
			_float fY = (_float)(fRadius * fSin_Theta * sinf(fPhi_Start));
			_float fZ = (_float)(fRadius * fCos_Theta);

			// 이미지 위치영역
			pVertex[iCurVertex].vPos = _vec3(fX, fY, fZ);
			//pVertex[iCurVertex].vNormal.x = fSin_Theta * cosf(fPhi_Start);
			//pVertex[iCurVertex].vNormal.y = fSin_Theta * sinf(fPhi_Start);
			//pVertex[iCurVertex].vNormal.z = fCos_Theta;

			// 이미지 출력영역
			//pVertex[iCurVertex].vTexUV = { fDeltaU * iSlice, fDeltaV * (iStack + 1) };

			// Color
			pVertex[iCurVertex].dwColor = dwColor;
			iCurVertex++;

			fPhi_Start += fPhi_Step;

			if (iSlice > 0)
			{
				if (iStack == 0)
				{
					pIndex[iFace]._0 = 0;
					pIndex[iFace]._1 = iSlice + 1;
					pIndex[iFace]._2 = iSlice;
					iFace++;
				}
				else
				{
					pIndex[iFace]._0 = Sphere_Vertex(SliceCount, iSlice - 1, iStack - 1);
					pIndex[iFace]._1 = Sphere_Vertex(SliceCount, iSlice, iStack - 1);
					pIndex[iFace]._2 = Sphere_Vertex(SliceCount, iSlice - 1, iStack);
					iFace++;

					pIndex[iFace]._0 = Sphere_Vertex(SliceCount, iSlice, iStack - 1);
					pIndex[iFace]._1 = Sphere_Vertex(SliceCount, iSlice, iStack);
					pIndex[iFace]._2 = Sphere_Vertex(SliceCount, iSlice - 1, iStack);
					iFace++;
				}
			}
		}

		fTheta += fTheta_Step;

		if (iStack == 0)
		{
			pIndex[iFace]._0 = 0;
			pIndex[iFace]._1 = 1;
			pIndex[iFace]._2 = iSlice;
			iFace++;
		}
		else
		{
			pIndex[iFace]._0 = Sphere_Vertex(SliceCount, iSlice - 1, iStack - 1);
			pIndex[iFace]._1 = Sphere_Vertex(SliceCount, 0, iStack - 1);
			pIndex[iFace]._2 = Sphere_Vertex(SliceCount, iSlice - 1, iStack);
			iFace++;

			pIndex[iFace]._0 = Sphere_Vertex(SliceCount, 0, iStack - 1);
			pIndex[iFace]._1 = Sphere_Vertex(SliceCount, 0, iStack);
			pIndex[iFace]._2 = Sphere_Vertex(SliceCount, iSlice - 1, iStack);
			iFace++;

		}
	}
	pVertex[iCurVertex].vPos = { 0.f, 0.f, -fRadius};

	// Color
	pVertex[iCurVertex].dwColor = dwColor;
	//pVertex[iCurVertex].vTexUV = { 0.5f,1.f };
	//pVertex[iCurVertex].vNormal = { 0.f,0.f,-1.f };

	for (iSlice = 1; iSlice < SliceCount; ++iSlice)
	{
		pIndex[iFace]._0 = Sphere_Vertex(SliceCount, iSlice - 1, iStack - 1);
		pIndex[iFace]._1 = Sphere_Vertex(SliceCount, iSlice, iStack - 1);
		pIndex[iFace]._2 = iCurVertex;
		iFace++;
	}
	pIndex[iFace]._0 = Sphere_Vertex(SliceCount, iSlice - 1, iStack - 1);
	pIndex[iFace]._1 = Sphere_Vertex(SliceCount, 0, iStack - 1);
	pIndex[iFace]._2 = iCurVertex;
	iFace++;

#pragma endregion rere

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CSphereTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CSphereTex * CSphereTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCenterPos, _float fRadius, _ulong dwColor)
{
	CSphereTex*	pInstance = new CSphereTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(vCenterPos, fRadius, dwColor)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSphereTex::Clone(void)
{
	return new CSphereTex(*this);
}

void CSphereTex::Free(void)
{
	CVIBuffer::Free();
}
