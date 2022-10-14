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

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);		// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT 형 3개의 크기만큼이며, 괄호산의 숫자 0의 의미는 본래 버텍스에 텍스처 UV 값이 여러개가 올 수 있는데 그중 0번째 것이 값을 지정하겠다는 의미이다.

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

	// 파티클
	typedef struct Particle
	{
		_vec3		vPosition;	// 위치
		D3DCOLOR	tColor;		// 컬러
								//_float			_size;		// 크기
	}PARTICLE;
	const _ulong		FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE; // D3DFVF_PSIZE : 크기제어요소 : 그래픽카드에 따라 지원유무가 갈린다.
																	//D3DFVF_XYZ : 변환되지 않은 꼭짓점 위치가 포함됨.
																	//D3DFVF_DIFFUSE : 확산색 구성요소가 포함됨.

																	// 파티클 속성
	typedef struct Attribute
	{
		_vec3		_position;		// 월드스페이스 내 파티클 위치
		_vec3		_velocity;		// 파티클 속도(보통 /s)
		_vec3		_acceleration;	// 파티클의 가속(보통 /s)
		_float		_lifeTime;		// 파티클 소멸까지 유지되는 시간.
		_float		_age;			// 파티클의 현재 나이
		D3DXCOLOR	_color;			// 파티클의 컬러
		D3DXCOLOR	_colorFade;		// 파티클의 컬러가 시간의 흚에 따라 퇴색하는 방법
		_bool		_isAlive;		// 파티클이 생존한 경우 True, 소멸한 경우 False

	}ATTRIBUTE;


	// 경계 박스
	typedef struct tBoundingBox
	{
		_vec3	vMin;
		_vec3	vMax;

		_bool isPointInside(_vec3 p)
		{
			// 지정한 포인트가 경계상자 내에 있는가?
			if (p.x >= vMin.x && p.y >= vMin.y && p.z >= vMin.z &&
				p.x <= vMax.x && p.y <= vMax.y && p.z <= vMax.z)
				return true;
			else
				return false;
		}
	}BDBOX, *LPBDBOX;

	// 경계 구
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

		_vec3			vPosition;			// 월드스페이스 내 파티클 위치
		_vec3			vVelocity;			// 파티클의 속도 (per second)
		_vec3			vAcceleration;	// 파티클의 가속도 (per second)

		_float			fLifeTime;			// 파티클 소멸까지 유지되는 시간
		_float			fAge;				// 파티클의 현재 나이

		D3DXCOLOR	tColor;			// 파티클의 색상
		D3DXCOLOR	tColorFade;	// 파티클의 색상이 시간의 흐름에 따라 퇴색하는 방법

		_bool			bIsAlive;			// 파티클의 생존 유무
	} ATTINFO;

	typedef struct tParticleInfo // tool용 구조체
	{
		_vec3			vOrigin;			// 파티클이 시작되는 곳
		BDBOX			tBdBox;			// 파티클이 이동할 수 있는 부피 제한
		_float			fEmitRate;		// 시스템에 새로운 파티클이 추가되는 비율. (개수/s)
		_float			fSize;				// 시스템 내 모든 파티클의 크기 
		_int			iMaxParticles;		// 주어진 시간동안 시스템이 가질 수 있는 최대 파티클 수
		_ulong			dwVbSize;			// 버텍스 버퍼가 보관할 수 있는 파티클 수
											// 실제 파티클 시스템 내 파티클 수와는 독립적
		_ulong			dwVbOffset;		// 렌더링에 이용됨
											// 버텍스 버퍼에서 복사를 시작할 파티클 내 다음 단계로의 오프셋		
											// 바이트가 아닌 파티클 단위이다.
											// ex) 단계 1이 0부터 499라면, 단계2로의 오프셋은 500
		_ulong			dwVbBatchSize;	// 렌더링에 이용됨
											// 단계 하나에 정의된 파티클 수

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

		// 상태
		_bool		bStun;
		_bool		bSlow;
	}PLAYERINFO;

	typedef struct tItemInfo
	{
		_int		iHp;	// 내구도
		_int		iLevel;	// 레벨
		_int		iExp;
		_int		iAbility;	// 아이템이 가지고 있는 능력치
	}ITEMINFO;
}


#endif // Engine_Struct_h__
