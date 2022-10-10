#include "..\..\Header\Weapon.h"

USING(Engine)

_int CWeapon::Update_Object(const _float & fTimeDelta)
{
	return CItem::Update_Object(fTimeDelta);
}
