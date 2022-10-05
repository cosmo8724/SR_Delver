#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPtBuffer : public CComponent
{
protected:
	explicit CPtBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPtBuffer(const CPtBuffer& rhs);
	virtual ~CPtBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void			Render_Buffer();
	virtual void			Free();

public:
	static CPtBuffer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent*			Clone();

public:
	void					Set_ParticleList(list<ATTINFO>* pList) { m_pParticles = pList; }

protected:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	_ulong						m_vbSize;
	_ulong						m_vbOffset;
	_ulong						m_vbBatchSize;
	list<ATTINFO>*			m_pParticles;
};

END