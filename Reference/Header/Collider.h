#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CVIBuffer
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	virtual HRESULT			Ready_Buffer(void) override;
	virtual void				Render_Buffer(void) override;
	virtual CComponent*	Clone(void);
	virtual void				Free(void) override;

public:
	static CCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void		Calculate_WorldMatrix(_matrix matWorld);
	_bool		CollideAABB(CCollider* _collider);
	const _vec3&		Get_MinPoint() { return m_vMin; }
	const _vec3&		Get_MaxPoint() { return m_vMax; }
	_matrix		Get_WorldMatrix() { return m_matWorld; }
	void		Set_Free(_bool bFree) { m_bFree = bFree; }
	_bool		Is_Free() { return m_bFree; }

private:
	_bool		m_bClone;
	_vec3*		m_pPos;	// 8�� ������

	_bool		m_bFree;
	_vec3		m_vMin;
	_vec3		m_vMax;
	_vec3		m_vCenter;
	_float		m_fRadius;

	_matrix		m_matWorld;
};
END