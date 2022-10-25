
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4		g_WorldLightPosition;
float4		g_WorldCameraPosition;

float		g_fTime;

texture		g_DefaultTexture;

float		g_WaveHeight=0.5f;
float		g_Speed=1.f;
float		g_WaveFrequency=29.6f;
float		g_UVSpeed=0.8f;


sampler		DefaultSampler = sampler_state 
{
	texture = g_DefaultTexture;
};

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV		: TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition	: POSITION;	
	float2		vTexUV : TEXCOORD0;
	float3		fDiffuse : TEXCOORD1;
	float3		fViewDir : TEXCOORD2;
	float3		fReflection : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	float cosTime = g_WaveHeight * cos(g_fTime * g_Speed + In.vTexUV.x * g_WaveFrequency);
	In.vPosition.y += cosTime;

	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	
	float3 lightDir = Out.vPosition.xyz - g_WorldLightPosition.xyz;
	lightDir = normalize(lightDir);

	float3 viewDir = normalize(Out.vPosition.xyz - g_WorldCameraPosition.xyz);
	Out.fViewDir = viewDir;

	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	float3 worldNormal = mul(In.vNormal, (float3x3)g_WorldMatrix);
	worldNormal = normalize(worldNormal);

	Out.fDiffuse = dot(-lightDir, worldNormal);
	Out.fReflection = reflect(lightDir, worldNormal);

	Out.vPosition = vPosition;

	//Out.vTexUV = In.vTexUV + float2(g_fTime * g_UVSpeed, 0);
	Out.vTexUV = In.vTexUV + float2(0, g_fTime * g_UVSpeed);

	return Out;		
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector		vColor = (vector)0.f;

	Out.vColor = tex2D(DefaultSampler, In.vTexUV);
	Out.vColor.a = 0.5;

	return Out;
}

technique DefaultTechnique
{
	pass Default
	{
		Lighting = true;
		cullmode = none;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		BlendOp = Add;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}


