#include "stdafx.h"
#include "BonFire.h"
#include "Export_Function.h"

CBonFire::CBonFire(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	:CEcoObject(pGraphicDev)
{
	m_eType = ECO_BONFIRE;
	m_vPos = vPos;
}

CBonFire::CBonFire(const CEcoObject & rhs)
	: CEcoObject(rhs)
{
}

CBonFire::~CBonFire()
{
}

HRESULT CBonFire::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CBonFire::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CEcoObject::Update_Object(fTimeDelta);

	m_fFrame += fTimeDelta * 10.f;
	_int iFrameEnd = m_pTextureCom->Get_FrameEnd();
	if (iFrameEnd <= m_fFrame)
		m_fFrame = 0.f;
	
	// sound
	if (!g_bIsTool)
	{
		CTransform*		pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_TransformCom", ID_DYNAMIC));
		NULL_CHECK(pPlayerTransformCom);

		_vec3		vPlayerPos, vPos;
		pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransCom->Get_Info(INFO_POS, &vPos);

		_float fDist = D3DXVec3Length(&(vPlayerPos - vPos));

		if (fDist < 10.f)
			Engine::Play_Sound(L"E_Torch.mp3", SOUND_BONFIRE, 1.f);
		else
			Engine::StopSound(SOUND_BONFIRE);
	}
	Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CBonFire::LateUpdate_Object(void)
{
	if (m_bDead)
		return;

	Billboard();
	CEcoObject::LateUpdate_Object();
}

void CBonFire::Render_Obejct(void)
{
	if (m_bDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->Set_Texture((_ulong)m_fFrame);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	CEcoObject::Render_Obejct();	// collider ���
}

HRESULT CBonFire::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTexCom"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_EcoBonfire_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_EcoBonfire_Texture", pComponent });

	return S_OK;
}

CBonFire * CBonFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBonFire *	pInstance = new CBonFire(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CBonFire * CBonFire::Create(CEcoObject * pEcoObject)
{
	CBonFire *	pInstance = new CBonFire(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBonFire::Free(void)
{
	CEcoObject::Free();
}
