#include "stdafx.h"
#include "Equipment.h"
#include "Export_Function.h"
#include "CullingMgr.h"

_int CEquipment::Update_Object(const _float & fTimeDelta)
{
	return CItem::Update_Object(fTimeDelta);
}

void CEquipment::LateUpdate_Object(void)
{
	//if (CCullingMgr::GetInstance()->Is_Inside(this))
	//	Add_RenderGroup(RENDER_ALPHA, this);
}

void CEquipment::CollisionEvent(CGameObject * pOtherObj)
{

}

void CEquipment::InteractEvent()
{
	if (STATE_GROUND == m_eState)
	{
		m_eState = STATE_INV;
		m_pColliderCom->Set_Free(true);
	}
}
