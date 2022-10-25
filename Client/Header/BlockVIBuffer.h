#pragma once
#include "Engine_Include.h"
#include "Export_Function.h"


class CBlockVIBuffer
{
	DECLARE_SINGLETON(CBlockVIBuffer)

	typedef struct BufferPerTexture
	{
		LPDIRECT3DVERTEXBUFFER9		m_pVB;
		LPDIRECT3DINDEXBUFFER9			m_pIB;

		_ulong				m_dwVtxCnt;
		_ulong				m_dwTriCnt;
		_ulong				m_dwBlockCnt;
		vector<CTransform*>	m_vecMatWorld;
		IDirect3DBaseTexture9*	m_pTexture;

		BufferPerTexture()
			: m_pVB(nullptr)
			, m_pIB(nullptr)
			, m_pTexture(nullptr)
			, m_dwVtxCnt(0)
			, m_dwTriCnt(0)
			, m_dwBlockCnt(0)
		{
			m_vecMatWorld.reserve(500);
		}
		~BufferPerTexture()
		{
			if (m_pVB)
				Safe_Release(m_pVB);
			if (m_pIB)
				Safe_Release(m_pIB);
		}

	}BPT;

private:
	CBlockVIBuffer();
	~CBlockVIBuffer();

public:
	HRESULT		Add_Instancing(BLOCKTYPE eType, CTexture* pTextureCom, _int iTexture, CTransform* pTransformCom);
	HRESULT		Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, BLOCKTYPE eType, _int iTexture);
	void		Render_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, BLOCKTYPE eType, _int iTexture);
	void		Set_Scene(LOADINGID eID); // { if (eID == LOADING_BOSS) g_bBoss = true; }

public:
	void			Free();

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	BPT**				ppBPT;
	
	_ulong				m_dwVtxSize;
	_ulong				m_dwFVF;
	
	_ulong				m_dwIdxSize;
	D3DFORMAT		m_IdxFmt;

	_matrix			m_matView;
	_matrix			m_matProj;
};