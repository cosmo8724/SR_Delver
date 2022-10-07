#pragma once
#include "Base.h"
#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void CollisionSphere(CGameObject* _pDest, CGameObject* _pSour);
	void CollisionAABB(CGameObject* _pDest, CGameObject* _pSour);
	void CollisionTest(CGameObject* _pDest, CGameObject* _pSour);
private:
	virtual void Free(void);
};

END

