#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTileTex : public CVIBuffer
{
private:
	explicit CTileTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTileTex(const CTileTex& rhs);
	virtual ~CTileTex();

public:
	const _vec3*		Get_VtxPos(void) const { return m_pPos; }

public:
	HRESULT		Ready_Buffer(const _ulong& dwVtxItv);
	virtual void	Render_Buffer(void) override;

private:
	_vec3*				m_pPos;
	_bool				m_bClone;

public:
	static CVIBuffer*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxItv);
	CComponent*	Clone(void);
	virtual void Free(void) override;
};

END