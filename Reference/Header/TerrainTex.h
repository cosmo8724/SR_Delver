#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*	Get_VtxPos(void) const	{ return m_pPos; }
	_ulong			Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong			Get_VtxCntZ(void) const { return m_dwCntZ; }

public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv, _vec3 vPos);
	virtual void Render_Buffer(void) override;

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	_bool				m_bClone;

	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;

public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv,_vec3 vPos);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END