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