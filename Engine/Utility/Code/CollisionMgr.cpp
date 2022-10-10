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
	//BDBOX*	pDestBox = _pDest->Get_bdBox();
	//BDBOX*	pSourBox = _pSour->Get_bdBox();

	//if ((pDestBox->vMax.x < pSourBox->vMin.x) || (pDestBox->vMin.x > pSourBox->vMax.x))
	//	return;
	//if ((pDestBox->vMax.y < pSourBox->vMin.y) || (pDestBox->vMin.y > pSourBox->vMax.y))
	//	return;
	//if ((pDestBox->vMax.z < pSourBox->vMin.z) || (pDestBox->vMin.z > pSourBox->vMax.z))
	//	return;

	//_pDest->CollisionEvent(_pSour);
	//_pSour->CollisionEvent(_pDest);

	CTransform*	pDest = static_cast<CTransform*>(_pDest->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	CTransform*	pSour = static_cast<CTransform*>(_pSour->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	CCollider*	pDestCol = static_cast<CCollider*>(_pDest->Get_Component(L"Proto_ColliderCom", ID_STATIC));
	CCollider*	pSourCol = static_cast<CCollider*>(_pSour->Get_Component(L"Proto_ColliderCom", ID_STATIC));

	if (pDestCol->CollideAABB(pSourCol))
	{
		_pDest->CollisionEvent(_pSour);
		_pSour->CollisionEvent(_pDest);
	}
	
}

void CCollisionMgr::CollisionTest(CGameObject * _pDest, CGameObject * _pSour)
{
	CTransform*	pDest = static_cast<CTransform*>(_pDest->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));
	CTransform*	pSour = static_cast<CTransform*>(_pSour->Get_Component(L"Proto_TransformCom", ID_DYNAMIC));

	_vec3 vDest, vSour;
	pDest->Get_Info(INFO_POS, &vDest);
	pSour->Get_Info(INFO_POS, &vSour);

	CCollider*	pDestCol = static_cast<CCollider*>(_pDest->Get_Component(L"Proto_ColliderCom", ID_STATIC));
	CCollider*	pSourCol = static_cast<CCollider*>(_pSour->Get_Component(L"Proto_ColliderCom", ID_STATIC));
	
	if (pDestCol->CollideSphere(pSourCol))
	{
		if (true == pDestCol->CollideAABB(pSourCol))
		{
			_pDest->CollisionEvent(_pSour);
			_pSour->CollisionEvent(_pDest);
		}
	}
}

void CCollisionMgr::Free(void)
{
}
