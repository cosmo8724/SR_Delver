#pragma once
#include "Base.h"
#include "GameObject.h"


BEGIN(Engine)
class CLayer;
class CBullet;
END

class CBulletMgr : public CBase
{
	DECLARE_SINGLETON(CBulletMgr)

private:
	explicit CBulletMgr();
	virtual ~CBulletMgr();

public:
	HRESULT			Ready_Proto(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT			Ready_Clone(CLayer* pLayer, LPDIRECT3DDEVICE9 pGraphicDev);
	void			Collect_Obj(_int iIdx, BULLETID eID);		// 사용을 마친 오브젝트를 회수하는 함수
	CGameObject*	Reuse_Obj(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir);	// 현재 풀에 있는 오브젝트를 재사용하는 함수
	void			Fire(BULLETID _eID);
	vector<CGameObject*>*	Get_Bullets(BULLETID _eID) { return &m_vecObjPool[_eID]; }
	bool			Is_Fired(CGameObject* pObj);

	void			Pre_Setting(BULLETID eID,  _float fSet);

	//void			FireTest(BULLETID _eID);

public:
	virtual void	Free() override;

private:
	//vector<wchar_t*>		m_vecObjTags[BULLET_END];
	vector<wstring*>			m_vecObjTags[BULLET_END];
	vector<CGameObject*>		m_vecObjPool[BULLET_END];
	//int							m_CurIdx[BULLET_END];
	int							m_MaxIdx[BULLET_END];
	queue<int>					m_IdxQue[BULLET_END];	// 사용가능한 모든 인덱스들을 삽입하고, 사용시 pop, 복구 시 다시 집어넣는다.

};

