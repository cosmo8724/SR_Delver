
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4		g_WorldLightPosition;
float4		g_WorldCameraPosition;

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
};

struct VS_OUT
{
	float4		vPosition	: POSITION;	
	float3		fDiffuse	: TEXCOORD1;
	float3		fViewDir	: TEXCOORD2;
	float3		fReflection : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = mul(In.vPosition, g_WorldMatrix);

	float3 lightDir = Out.vPosition.xyz - g_WorldLightPosition.xyz;
	lightDir = normalize(lightDir);

	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjMatrix);

	float3 worldNormal = mul(In.vNormal, (float3x3)g_WorldMatrix);
	worldNormal = normalize(worldNormal);

	Out.fDiffuse = dot(-lightDir, worldNormal);
	Out.fReflection = reflect(lightDir, worldNormal);

	return Out;		
}

struct PS_IN
{
	float4		vPosition	: POSITION;
	float3		fDiffuse	: TEXCOORD1;
	float3		fViewDir	: TEXCOORD2;
	float3		fReflection : TEXCOORD3;
};

struct PS_OUT
{
	vector		vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 diffuse = saturate(In.fDiffuse);

	float3 reflection = normalize(In.fReflection);
	float3 viewDir = normalize(In.fViewDir);
	float3 specular = 0;
	
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	Out.vColor = (ambient + diffuse + +specular, 1);

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


