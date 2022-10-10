#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex : public CVIBuffer
{
private:
	explicit CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereTex(const CSphereTex& rhs);
	virtual ~CSphereTex();

public:
	virtual HRESULT		Ready_Buffer(_vec3 vCenterPos, _float _fRadius, _ulong dwColor);
	virtual void		Render_Buffer(void) override;

	static	_ushort		Sphere_Vertex(_uint Slices, _int Slice, _int Stack)
	{
		return Stack * Slices + Slice + 1;
	}

public:
	_vec3*				Get_VtxPos(void)   { return m_pPos; }
	INDEX32*			Get_VtxIndex(void) { return m_pIndex; }
	_ulong*				Get_VtxCnt(void)   { return &m_dwVtxCnt; }

private:
	_bool				m_bClone;
	_vec3*				m_pPos = NULL;
	INDEX32*			m_pIndex = NULL;

public:
	static CSphereTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCenterPos = { 0.f, 0.f, 0.f }, _float fRadius = 1.f, _ulong dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	CComponent*			Clone(void);
	virtual void		Free(void) override;
};

END