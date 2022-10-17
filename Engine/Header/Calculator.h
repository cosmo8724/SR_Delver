#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CCubeTex;

class ENGINE_DLL CCalculator :	public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator(void);

	_float		HeightOnTerrain(_vec3* pPos, 
								const _vec3* pTerrainVtxPos, 
								const _ulong& dwCntX, 
								const _ulong& dwCntZ,
								const _ulong& dwVtxItv = 1);

	_vec3			PickingOnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);
	_vec3			PickingOnTerrainIndex(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);
	CubePlane		PickingOnCube(HWND hWnd, const CCubeTex * pCubeTexBufferCom, const CTransform * pCubeTransCom, _bool* pCheck, _vec3* pVec);
	_bool			CheckIntersectCube(HWND hWnd, const CCubeTex* pCubeTexBufferCom, const CTransform* pCubeTransCom, _float* fDistance);
	
	_bool			Picking(HWND hWnd, CRcTex* pRcTex, const CTransform* pTransCom);

public:
	virtual CComponent* Clone(void) override;
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

END