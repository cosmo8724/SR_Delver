#include "..\..\Header\CollisionMgr.h"
#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
	Free();
}

void CCollisionMgr::CollisionSphere(CGameObject * _pDest, CGameObject * _pSour)
{
	_vec3		DestPos = _pDest->Get_bdSphere()->vCenter;
	_vec3		SourPos = _pSour->Get_bdSphere()->vCenter;

	if (pow(SourPos.x - DestPos.x, 2) +
		pow(SourPos.y - DestPos.y, 2) +
		pow(SourPos.z - DestPos.z, 2) <
		pow(_pSour->Get_bdSphere()->fRadius + _pDest->Get_bdSphere()->fRadius, 2))
	{
		_pDest->CollisionEvent(_pSour);
		_pSour->CollisionEvent(_pDest);
	}
}

void CCollisionMgr::CollisionAABB(CGameObject * _pDest, CGameObject * _pSour)
{
	BDBOX*	pDestBox = _pDest->Get_bdBox();
	BDBOX*	pSourBox = _pSour->Get_bdBox();

	if ((pDestBox->vMax.x < pSourBox->vMin.x) || (pDestBox->vMin.x > pSourBox->vMax.x))
		return;
	if ((pDestBox->vMax.y < pSourBox->vMin.y) || (pDestBox->vMin.y > pSourBox->vMax.y))
		return;
	if ((pDestBox->vMax.z < pSourBox->vMin.z) || (pDestBox->vMin.z > pSourBox->vMax.z))
		return;

	_pDest->CollisionEvent(_pSour);
	_pSour->CollisionEvent(_pDest);
}

void CCollisionMgr::Free(void)
{
}
