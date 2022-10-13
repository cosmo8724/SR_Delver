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
	void			Collect_Obj(_int iIdx, BULLETID eID);		// ����� ��ģ ������Ʈ�� ȸ���ϴ� �Լ�
	CGameObject*	Reuse_Obj(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir);	// ���� Ǯ�� �ִ� ������Ʈ�� �����ϴ� �Լ�
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
	queue<int>					m_IdxQue[BULLET_END];	// ��밡���� ��� �ε������� �����ϰ�, ���� pop, ���� �� �ٽ� ����ִ´�.

};

