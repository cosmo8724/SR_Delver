#include "..\..\Header\ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}


CProtoMgr::~CProtoMgr()
{
	Free();
}

HRESULT CProtoMgr::Ready_Proto(const _tchar * pProtoTag, CComponent * pComponent)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapProto.insert({ pProtoTag, pComponent });

	return S_OK;
}

CComponent * CProtoMgr::Clone_Proto(const _tchar * pProtoTag)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (pPrototype == nullptr) // 확인 필요
		return nullptr;

	return pPrototype->Clone();
}

HRESULT CProtoMgr::Delete_Proto(const _tchar * pProtoTag)
{
	auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return E_FAIL;

	Safe_Release(iter->second);

	iter = m_mapProto.erase(iter);

	return S_OK;
}

CComponent * CProtoMgr::Find_Proto(const _tchar * pProtoTag)
{
	auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;
	
	return iter->second;
}

void Engine::CProtoMgr::Free(void)
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}
