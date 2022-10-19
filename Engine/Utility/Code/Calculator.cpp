#include "..\..\Header\Calculator.h"
#include "TerrainTex.h"
#include "Transform.h"

#include "Export_Function.h"

USING(Engine)

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CCalculator::CCalculator(const CCalculator& rhs)
	:CComponent(rhs)
{

}

CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator(void)
{

	return S_OK;
}

Engine::_float CCalculator::HeightOnTerrain(_vec3* pPos, const _vec3* pTerrainVtxPos, 
											const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{

	_ulong	dwIndex = _ulong(pPos->z / dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE		Plane;

	// 오른쪽 위 삼각형
	if (fWidth > fHeight)
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + dwCntX + 1],
			&pTerrainVtxPos[dwIndex + 1]);

	}
	// 왼쪽 아래 삼각형
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}

	//ax + by + cz + d = 0;

	//by = -ax - cz - d
	// y = (-ax - cz - d) / b

	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;

	return pPos->y;
}

_bool CCalculator::Picking(HWND hWnd, CRcTex * pRcTex, const CTransform * pTransCom)
{
	//POINT	ptMouse{};

	//GetCursorPos(&ptMouse);
	//ScreenToClient(hWnd, &ptMouse);

	//_vec3	vPoint;

	//D3DVIEWPORT9		ViewPort;
	//ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	//m_pGraphicDev->GetViewport(&ViewPort);

	//// 뷰포트 -> 투영
	//vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	//vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	//vPoint.z = 0.f;

	//// 투영 -> 뷰 스페이스
	//_matrix		matProj;

	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//D3DXMatrixInverse(&matProj, nullptr, &matProj);
	//D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	//_vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태
	//vRayPos = { 0.f, 0.f, 0.f };
	//vRayDir = vPoint - vRayPos;

	//// 뷰 스페이스 -> 월드
	//_matrix		matView;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixInverse(&matView, nullptr, &matView);
	//D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	//D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	return true;
}

Engine::_vec3 Engine::CCalculator::PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vPoint;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰포트 -> 투영
	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3	vRayDir, vRayPos;		// 뷰 스페이스 영역에 있는 상태

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;	

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬
	_matrix		matWorld;
	
	pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]], 
				&pTerrainVtx[dwVtxIdx[2]], 
				&vRayPos, &vRayDir, 
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU, 
							0.f,
							 pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}
			
			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}
	
	return _vec3(0.f, 0.f, 0.f);
}

_vec3 CCalculator::PickingOnTerrainIndex(HWND hWnd, const CTerrainTex * pTerrainBufferCom, const CTransform * pTerrainTransformCom)
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3	vPoint;

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	_matrix	matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3 vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;

	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pTerrainTransformCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong		dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong		dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				_float MiddleX = (pTerrainVtx[dwVtxIdx[2]].x + pTerrainVtx[dwVtxIdx[0]].x) / 2.f;
				_float MiddleZ = (pTerrainVtx[dwVtxIdx[0]].z + pTerrainVtx[dwVtxIdx[2]].z) / 2.f;

				return _vec3(MiddleX, 0.f, MiddleZ);
			}

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				_float MiddleX = (pTerrainVtx[dwVtxIdx[0]].x + pTerrainVtx[dwVtxIdx[1]].x) / 2.f;
				_float MiddleZ = (pTerrainVtx[dwVtxIdx[0]].z + pTerrainVtx[dwVtxIdx[1]].z) / 2.f;

				return _vec3(MiddleX, 0.f, MiddleZ);
			}
		}
	}

	return vRayDir;
}

CubePlane CCalculator::PickingOnCube(HWND hWnd, const CCubeTex * pCubeTexBufferCom, const CTransform * pCubeTransCom, _bool * pCheck, _vec3 * pVec)
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3	vPoint;

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	_matrix	matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3 vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;

	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pCubeTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = pCubeTexBufferCom->Get_VtxPos();

	CubePlane		Plane = CUBEPLANE_END;
	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist, fLastDist = 0.f;
	_vec3	Middle;

	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_X;
			}
		}
	}

	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_X;
			}
		}
	}

	// -X
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 0;
	dwVtxIdx[2] = 3;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_X;
			}
		}
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_X;
			}
		}
	}

	// +Y
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 1;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_Y;
			}
		}
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_Y;
			}
		}
	}

	// -Y
	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 6;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_Y;
			}
		}
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_Y;
			}
		}
	}

	// +Z
	dwVtxIdx[0] = 5;
	dwVtxIdx[1] = 4;
	dwVtxIdx[2] = 7;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_Z;
			}
		}
	}

	dwVtxIdx[0] = 5;
	dwVtxIdx[1] = 7;
	dwVtxIdx[2] = 6;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = FRONT_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = FRONT_Z;
			}
		}
	}

	// -Z
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_Z;
			}
		}
	}

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;
	Middle = (pCubtVtx[dwVtxIdx[0]] + pCubtVtx[dwVtxIdx[2]]) / 2.f;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(*pCheck))
		{
			fLastDist = fDist;
			*pCheck = true;
			*pVec = Middle;
			Plane = BACK_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*pVec = Middle;
				Plane = BACK_Z;
			}
		}
	}
	return Plane;
}

_bool CCalculator::CheckIntersectCube(HWND hWnd, const CCubeTex * pCubeTexBufferCom, const CTransform * pCubeTransCom, _float * fDistance)
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3	vPoint;

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vPoint.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vPoint.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vPoint.z = 0.f;

	_matrix	matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	D3DXVec3TransformCoord(&vPoint, &vPoint, &matProj);

	_vec3 vRayDir, vRayPos;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vPoint - vRayPos;

	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix		matWorld;
	pCubeTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3*	pCubtVtx = pCubeTexBufferCom->Get_VtxPos();

	CubePlane		Plane = CUBEPLANE_END;
	_ulong	dwVtxIdx[3]{};
	_float	fU, fV, fDist, fLastDist = 0.f;
	_bool	bChecked = false;

	// +X
	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 6;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_X;
			}
		}
	}

	dwVtxIdx[0] = 1;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 2;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_X;
			}
		}
	}

	// -X
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 0;
	dwVtxIdx[2] = 3;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_X;
			}
		}
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 3;
	dwVtxIdx[2] = 7;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_X;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_X;
			}
		}
	}

	// +Y
	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 5;
	dwVtxIdx[2] = 1;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_Y;
			}
		}
	}

	dwVtxIdx[0] = 4;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 0;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_Y;
			}
		}
	}

	// -Y
	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 6;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_Y;
			}
		}
	}

	dwVtxIdx[0] = 3;
	dwVtxIdx[1] = 6;
	dwVtxIdx[2] = 7;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_Y;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_Y;
			}
		}
	}

	// +Z
	dwVtxIdx[0] = 5;
	dwVtxIdx[1] = 4;
	dwVtxIdx[2] = 7;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_Z;
			}
		}
	}

	dwVtxIdx[0] = 5;
	dwVtxIdx[1] = 7;
	dwVtxIdx[2] = 6;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = FRONT_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = FRONT_Z;
			}
		}
	}

	// -Z
	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 1;
	dwVtxIdx[2] = 2;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_Z;
			}
		}
	}

	dwVtxIdx[0] = 0;
	dwVtxIdx[1] = 2;
	dwVtxIdx[2] = 3;

	if (D3DXIntersectTri(&pCubtVtx[dwVtxIdx[1]],
		&pCubtVtx[dwVtxIdx[0]],
		&pCubtVtx[dwVtxIdx[2]],
		&vRayPos, &vRayDir,
		&fU, &fV, &fDist))
	{
		if (!(bChecked))
		{
			fLastDist = fDist;
			bChecked = true;
			Plane = BACK_Z;
		}
		else
		{
			if (fLastDist > fDist)
			{
				*fDistance = fDist;
				Plane = BACK_Z;
			}
		}
	}

	return bChecked;
}


CComponent* CCalculator::Clone(void)
{
	return new CCalculator(*this);
}

CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		MSG_BOX("Calculator Create Failed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCalculator::Free(void)
{
	CComponent::Free();
}
