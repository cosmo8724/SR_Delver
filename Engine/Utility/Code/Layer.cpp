#include "..\..\Header\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_mapObject.insert({ pObjTag, pInstance });

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return E_FAIL;

	CGameObject * pInstance = iter->second;

	Safe_Release<CGameObject*>(pInstance);

	iter = m_mapObject.erase(iter);

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iResult = 0;

	//for (auto& iter : m_mapObject)
	//{
	//	iResult = iter.second->Update_Object(fTimeDelta);

	//	if (iResult & 0x80000000) //0x80000000 == -1
	//		return iResult;
	//}
	map<const _tchar*, CGameObject*>::iterator iter;
	for (iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{
		iResult = iter->second->Update_Object(fTimeDelta);

		if (iResult & 0x80000000) //0x80000000 == -1
			return iResult;

		if (OBJ_TEST == iResult)
		{
			Safe_Release(iter->second);
			m_mapObject.erase(iter++);
		}
		else
		{
			++iter;
		}

	}

	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_Object();
}

CLayer* CLayer::Create(void)
{
	CLayer*	pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
		Safe_Release(pLayer);

	return pLayer;
}

void CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
