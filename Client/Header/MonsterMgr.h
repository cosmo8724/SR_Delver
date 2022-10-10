#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CItem;
class CMonster;
END

class CMonsterMgr : public CBase
{
	DECLARE_SINGLETON(CMonsterMgr)

private:
	CMonsterMgr();
	~CMonsterMgr();

public:
	void			Ready_ItemMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	// ������ Proto ������Ʈ ����(�ַ� �ؽ���)
	HRESULT			Ready_Proto();

	// ���� ����(�ʿ��� ��� ���)
	HRESULT			Ready_Clone(CLayer* pLayer);

	// ������ ���� �Ҵ��(������������ Layer ���� �� Add GameObject�� �̸� ���ԵǴ� ���)
	HRESULT			Add_GameObject(CLayer* pLayer, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos);

	// ������ ���� �Ҵ��
	HRESULT			Add_GameObject(const _tchar* pLayerTag, const _tchar* objTag, ITEMTYPE eType, _vec3 vPos);

	// ������ ���� �Ҵ�� (�κ��丮 �̹��������� ��)
	CGameObject*	Add_GameObject(const _tchar* pLayerTag, wstring texTag, CItem* pObj);



public:
	inline virtual void Free(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	vector<TCHAR*>		m_vecItemObjTags[ITEM_END];	// �������� ���������� �� ������Ʈ �±� �����
	vector<TCHAR*>		m_vecItemTextureTags;		// �κ��丮�� ���� ������ InvImg �� �ؽ��� ���۳�Ʈ �±� �����

	vector<CGameObject*>	m_vecItemPool;

};

