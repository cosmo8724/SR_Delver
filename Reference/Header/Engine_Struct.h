#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	typedef struct tagVertexTex
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTex
	{
		_vec3		vPos;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);		// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT �� 3���� ũ�⸸ŭ�̸�, ��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ�ó UV ���� �������� �� �� �ִµ� ���� 0��° ���� ���� �����ϰڴٴ� �ǹ��̴�.

	typedef	struct tagIndex16
	{
		_ushort			_0;
		_ushort			_1;
		_ushort			_2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	typedef struct tagTile
	{
		tagVertexTex*	pVertex = nullptr;
		tagIndex32*		pIndex= nullptr;
		BYTE				byOption = 0;
		BYTE				byDrawID = 0;
		_ulong				dwIndex = 0;
	}TILE;

	// ��ƼŬ
	typedef struct Particle
	{
		_vec3		vPosition;	// ��ġ
		D3DCOLOR	tColor;		// �÷�
								//_float			_size;		// ũ��
	}PARTICLE;
	const _ulong		FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE; // D3DFVF_PSIZE : ũ�������� : �׷���ī�忡 ���� ���������� ������.
																	//D3DFVF_XYZ : ��ȯ���� ���� ������ ��ġ�� ���Ե�.
																	//D3DFVF_DIFFUSE : Ȯ��� ������Ұ� ���Ե�.

																	// ��ƼŬ �Ӽ�
	typedef struct Attribute
	{
		_vec3		_position;		// ���彺���̽� �� ��ƼŬ ��ġ
		_vec3		_velocity;		// ��ƼŬ �ӵ�(���� /s)
		_vec3		_acceleration;	// ��ƼŬ�� ����(���� /s)
		_float		_lifeTime;		// ��ƼŬ �Ҹ���� �����Ǵ� �ð�.
		_float		_age;			// ��ƼŬ�� ���� ����
		D3DXCOLOR	_color;			// ��ƼŬ�� �÷�
		D3DXCOLOR	_colorFade;		// ��ƼŬ�� �÷��� �ð��� �v�� ���� ����ϴ� ���
		_bool		_isAlive;		// ��ƼŬ�� ������ ��� True, �Ҹ��� ��� False

	}ATTRIBUTE;


	// ��� �ڽ�
	typedef struct tBoundingBox
	{
		_vec3	vMin;
		_vec3	vMax;

		_bool isPointInside(_vec3 p)
		{
			// ������ ����Ʈ�� ������ ���� �ִ°�?
			if (p.x >= vMin.x && p.y >= vMin.y && p.z >= vMin.z &&
				p.x <= vMax.x && p.y <= vMax.y && p.z <= vMax.z)
				return true;
			else
				return false;
		}
	}BDBOX, *LPBDBOX;

	// ��� ��
	typedef struct tBoundingSphere
	{
		_vec3	vCenter;
		_float	fRadius;
	}BDSPHERE, *LPBDSPHERE;

	struct TexturePath
	{
		PDIRECT3DTEXTURE9 pTexture;
		_tchar*   wszFilePathName;
	};

	typedef struct tAttributeInput
	{
		tAttributeInput()
		{
			fLifeTime = 0.f;
			fAge = 0.f;
			bIsAlive = true;
		}

		_vec3			vPosition;			// ���彺���̽� �� ��ƼŬ ��ġ
		_vec3			vVelocity;			// ��ƼŬ�� �ӵ� (per second)
		_vec3			vAcceleration;	// ��ƼŬ�� ���ӵ� (per second)

		_float			fLifeTime;			// ��ƼŬ �Ҹ���� �����Ǵ� �ð�
		_float			fAge;				// ��ƼŬ�� ���� ����

		D3DXCOLOR	tColor;			// ��ƼŬ�� ����
		D3DXCOLOR	tColorFade;	// ��ƼŬ�� ������ �ð��� �帧�� ���� ����ϴ� ���

		_bool			bIsAlive;			// ��ƼŬ�� ���� ����
	} ATTINFO;

	typedef struct tParticleInfo // tool�� ����ü
	{
		_vec3			vOrigin;			// ��ƼŬ�� ���۵Ǵ� ��
		BDBOX			tBdBox;			// ��ƼŬ�� �̵��� �� �ִ� ���� ����
		_float			fEmitRate;		// �ý��ۿ� ���ο� ��ƼŬ�� �߰��Ǵ� ����. (����/s)
		_float			fSize;				// �ý��� �� ��� ��ƼŬ�� ũ�� 
		_int			iMaxParticles;		// �־��� �ð����� �ý����� ���� �� �ִ� �ִ� ��ƼŬ ��
		_ulong			dwVbSize;			// ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ ��
											// ���� ��ƼŬ �ý��� �� ��ƼŬ ���ʹ� ������
		_ulong			dwVbOffset;		// �������� �̿��
										// ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������		
										// ����Ʈ�� �ƴ� ��ƼŬ �����̴�.
										// ex) �ܰ� 1�� 0���� 499���, �ܰ�2���� �������� 500
		_ulong			dwVbBatchSize;	// �������� �̿��
										// �ܰ� �ϳ��� ���ǵ� ��ƼŬ ��

	} PINFO;

	typedef struct tagCreatureInfo
	{
		_int		iHp;
		_int		iAttack;

	}MONTSERINFO;

	typedef struct tPlayerInfo
	{
		_int		iHp;
		_int		iHpMax;
		_int		iLevel;
		_int		iExp;
		_int		iExpMax;
		_int		iHunger;
		_int		iAtk;
		_int		iDef;
		_float		fSpeed;

		// ����
		_bool		bStun;
		_bool		bSlow;
		_float		fSlowSpeed;
	}PLAYERINFO;

	typedef struct tItemInfo
	{
		_int		iHp;	// ������
		_int		iLevel;	// ����
		_int		iExp;

		_int		iHpHeal = 0;	// �������� ������ �ִ� �ɷ�ġ
		_int		iDef = 0;
		_float		fSpeed = 0.f;
		_int		iHunger = 0;
		_int		iAtk = 0;
	}ITEMINFO;
}


#endif // Engine_Struct_h__
