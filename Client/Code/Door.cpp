#include "stdafx.h"
#include "..\Header\Door.h"
#include "Export_Function.h"
#include "Player.h"
#include "CullingMgr.h"
#include "Player.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEcoObject(pGraphicDev)
{
}

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
	: CEcoObject(pGraphicDev)
{
	m_eType = ECO_DOOR;
	m_vPos = vPos;
	m_str = L"<Door>\nType:Door\nOpen the door";
}

CDoor::CDoor(const CEcoObject & rhs)
	: CEcoObject(rhs)
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

	m_pTransCom->Set_Scale(1.f, 1.f, 0.1f);
	//m_pTransCom->Set_Angle(0.f, D3DXToRadian(30.f), 0.f);
	m_iTexture = 0;

	return S_OK;
}

_int CDoor::Update_Object(const _float & fTimeDelta)
{

	CEcoObject::Update_Object(fTimeDelta);

	if (!g_bIsTool && !m_bReady)
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		NULL_CHECK_RETURN(pPlayer,0);

		pPlayer->Add_CollisionGroup(this);
		m_bReady = true;
	}


	if (m_fAngle == 0.f)
		m_str = L"E : Open";
	else
		m_str = L"E : Close";


	if (m_bMove)
	{
		_matrix matScale;
		_vec3 vScale = m_pTransCom->Get_Scale();
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);

		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans, vScale.x, 0.f, 0.f);

		_matrix matRot;
		m_fAngle += m_fAngleSpeed;

		if (m_fAngle <= -90.f)
		{
			m_fAngle = -90.f;
			m_fAngleSpeed *= -1;
			m_bMove = false;
		}
		else if (m_fAngle >= 0.f)
		{
			m_fAngle = 0.f;
			m_fAngleSpeed *= -1;
			m_bMove = false;
		}
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fAngle));

		_matrix invmatTrans;
		D3DXMatrixInverse(&invmatTrans, 0, &matTrans);

		_matrix matOriginRot;
		_vec3 vRot = m_pTransCom->Get_Angle();
		D3DXMatrixRotationY(&matOriginRot, vRot.y);


		_matrix matPos;
		_vec3 vPos = m_pTransCom->Get_Pos();
		D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);

		_matrix matWorld;
		matWorld = matScale * matTrans * matRot * invmatTrans * matOriginRot * matPos;


		m_pTransCom->Set_WorldMatrix(&matWorld);
	}
	else
	{
		_matrix matScale;
		_vec3 vScale = m_pTransCom->Get_Scale();
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);

		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans, vScale.x, 0.f, 0.f);

		_matrix matRot;
		D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fAngle));

		_matrix invmatTrans;
		D3DXMatrixInverse(&invmatTrans, 0, &matTrans);

		_matrix matOriginRot;
		_vec3 vRot = m_pTransCom->Get_Angle();
		D3DXMatrixRotationY(&matOriginRot, vRot.y);


		_matrix matPos;
		_vec3 vPos = m_pTransCom->Get_Pos();
		D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);

		_matrix matWorld;
		matWorld = matScale * matTrans * matRot * invmatTrans * matOriginRot * matPos;


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

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTextureCom->Set_Texture(m_iTexture);
	this->m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	CEcoObject::Render_Obejct();	// collider Ãâ·Â
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Door_Texture"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Door_Texture", pComponent });


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
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
	if (pPlayer)
	{
		//pPlayer->
	}
}

void CDoor::InteractEvent()
{
	if (!m_bMove)
	{
		Engine::StopSound(SOUND_BLUEBAT);
		Engine::Play_Sound(L"E_Door.mp3", SOUND_BLUEBAT, 1.f);

		m_bMove = true;
	}

}
