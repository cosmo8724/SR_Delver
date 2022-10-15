#include "..\..\Header\PtBuffer.h"

USING(Engine)

CPtBuffer::CPtBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_vbSize(2048)
	, m_vbOffset(0)
	, m_vbBatchSize(512)
	, m_pParticles(nullptr)

{
}

CPtBuffer::CPtBuffer(const CPtBuffer & rhs)
	: CComponent(rhs)
	, m_vbSize(rhs.m_vbSize)
	, m_vbOffset(rhs.m_vbOffset)
	, m_vbBatchSize(rhs.m_vbBatchSize)
	, m_pParticles(nullptr)
{
	// D3D��ġ ������ �ʱ�ȭ �۾��� ó��
	m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(PARTICLE),	// ���ؽ� ���� ũ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : ���� ���ؽ����۸� ����Ѵ�.
		// _POINTS	: ���ؽ� ���۰� ����Ʈ ��������Ʈ�� ������ ������ ����
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0);
}

CPtBuffer::~CPtBuffer()
{
}

HRESULT CPtBuffer::Ready_Buffer()
{
	// D3D��ġ ������ �ʱ�ȭ �۾��� ó��
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(PARTICLE),	// ���ؽ� ���� ũ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		// _DYNAMIC : ���� ���ؽ����۸� ����Ѵ�.
		// _POINTS	: ���ؽ� ���۰� ����Ʈ ��������Ʈ�� ������ ������ ����
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0), E_FAIL);

	return S_OK;
}

void CPtBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(Particle));
	m_pGraphicDev->SetFVF(FVF_PARTICLE);


	// ���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
	if (m_vbOffset >= m_vbSize)
		m_vbOffset = 0;

	Particle* v = 0;

	m_pVB->Lock(
		m_vbOffset * sizeof(PARTICLE),
		m_vbBatchSize * sizeof(PARTICLE),
		(void**)&v,
		m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	DWORD numParticlesInBatch = 0;

	// ��� ��ƼŬ�� ������ �ɶ�����
	for (auto iter = m_pParticles->begin(); iter != m_pParticles->end(); ++iter)
	{
		if (iter->bIsAlive)
		{
			// �Ѵܰ��� ������ ��ƼŬ�� ���� ���ؽ� ���� ���׸�Ʈ�� ����
			v->vPosition = iter->vPosition;
			v->tColor = (D3DCOLOR)iter->tColor;
			v++; // next element;

			numParticlesInBatch++; // �ܰ� ī���͸� ������Ų��.

								   // ���� �ܰ谡 ��� ä�����ִ°�?
			if (numParticlesInBatch == m_vbBatchSize)
			{
				// ���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ���� �׸���.
				m_pVB->Unlock();

				m_pGraphicDev->DrawPrimitive(
					D3DPT_POINTLIST,
					m_vbOffset,
					m_vbBatchSize);

				// �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.

				// ���� �ܰ��� ó�� ���������� �̵��Ѵ�.
				m_vbOffset += m_vbBatchSize;

				// ���ؽ� ������ ��踦 �Ѵ� �޸𸮷� �������� �������� �����´�.
				// ��踦 ���� ��� ó������ ����
				if (m_vbOffset >= m_vbSize)
					m_vbOffset = 0;

				m_pVB->Lock(
					m_vbOffset * sizeof(PARTICLE),
					m_vbBatchSize * sizeof(PARTICLE),
					(void**)&v,
					m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

				numParticlesInBatch = 0; // �����ܰ踦 ���� ����
			}
		}
	}

	m_pVB->Unlock();

	// numParticleInBatch == vbBatchSize ������ �������� �ʾ�
	// �������ܰ谡 ���������� �ʴ� ��찡 �߻��� �� ����.
	// �Ϻθ� ä���� �ܰ�� �̰����� ��������.
	if (numParticlesInBatch)
	{
		m_pGraphicDev->DrawPrimitive(
			D3DPT_POINTLIST,
			m_vbOffset,
			numParticlesInBatch);
	}

	// �������
	m_vbOffset += m_vbBatchSize;
}

void CPtBuffer::Free()
{
	Safe_Release(m_pVB);
	m_pParticles = nullptr;
	CComponent::Free();
}

CPtBuffer * CPtBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPtBuffer* pInstance = new CPtBuffer(pGraphicDev);
	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CPtBuffer::Clone()
{
	return new CPtBuffer(*this);
}
