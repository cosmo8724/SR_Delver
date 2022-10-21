#include "stdafx.h"
#include "..\Header\Door.h"
#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEcoObject(pGraphicDev)
{
}

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CEcoObject(pGraphicDev)
{
	m_eType = ECO_DOOR;
	m_vPos = vPos;
}

CDoor::CDoor(const CEcoObject & rhs)
	:CEcoObject(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (!m_bClone)
		m_pTransCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	m_pTransCom->Set_Scale(0.08f, 1.f, 0.8f);

	return S_OK;
}

_int CDoor::Update_Object(const _float & fTimeDelta)
{

	CEcoObject::Update_Object(fTimeDelta);

	if (!m_bReady)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		if (nullptr != pPlayer)
		{
			pPlayer->Add_CollisionGroup(this);
			m_bReady = true;
		}
		else
			return 0;
	}

	if (m_bMove)
	{
		_vec3 vScale = m_pTransCom->Get_Scale();
		_matrix matScale;
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);

		_vec3 vRight;
		m_pTransCom->Get_Info(INFO_RIGHT, &vRight);
		D3DXVec3Normalize(&vRight, &vRight);

		_matrix matTrans;
		//D3DXMatrixTranslation(&matTrans, )

		

		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans, 1.f, 0.f, 0.f);

		_matrix matRot;
		m_fAngle += 10.f * fTimeDelta;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fAngle));

		_matrix matPos;
		D3DXMatrixTranslation(&matPos, m_vPos.x, m_vPos.y, m_vPos.z);

		_matrix matWorld;
		matWorld = matScale * matTrans * matRot * matPos;

		m_pTransCom->Set_WorldMatrix(&matWorld);

	}



	return OBJ_NOEVENT;
}

void CDoor::LateUpdate_Object(void)
{
	if(CCullingMgr::GetInstance()->Is_Inside(this))
		Add_RenderGroup(RENDER_ALPHA, this);


	CEcoObject::LateUpdate_Object();
}

void CDoor::Render_Obejct(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrixPointer());

	// 알파값 변화
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	this->m_pBufferCom->Render_Buffer();

	// 알파값 변화 해제
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	CEcoObject::Render_Obejct();	// collider 출력
}

HRESULT CDoor::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = this->m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_CubeTexCom"));
	NULL_CHECK_RETURN(this->m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_CubeTexCom", pComponent });

	if (!m_bClone)
	{
		pComponent = m_pTransCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_TransformCom"));
		NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
		m_mapComponent[ID_DYNAMIC].insert({ L"Proto_TransformCom", pComponent });
	}

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Clone_Proto(L"Proto_ColliderCom"));
	NULL_CHECK_RETURN(m_pTransCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ColliderCom", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_RockFall_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RockFall_Texture", pComponent });


	return S_OK;
}

CDoor * CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CDoor *	pInstance = new CDoor(pGraphicDev, vPos);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CDoor * CDoor::Create(CEcoObject * pEcoObject)
{
	CDoor *	pInstance = new CDoor(*pEcoObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoor::Free(void)
{
	CEcoObject::Free();
}

void CDoor::CollisionEvent(CGameObject * pObj)
{

}

void CDoor::InteractEvent()
{
	MSG_BOX("tq");
	m_bMove = true;
}
