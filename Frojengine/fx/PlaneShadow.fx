// ��� ����
cbuffer cbWVP : register(b0)
{
	matrix mTM;     // ���� ���. 
	matrix mView;   // �� ��ȯ ���. 
	matrix mWV;     // ����-�� ��ȯ ���. 
	matrix mProj;   // ���� ��ȯ ���.
};

cbuffer ConstBuffer : register(b1)
{
	float4 fogColor;
	float fogDepthMin;
	float fogDepthMax;
};


Texture2D texDiffuse : register(t0);

SamplerState smpLinear;



////////////////////////////////////////////////////////////////////////////// 
//
// ���ݻ籤 ���� ��� : ���� �� ����. Blinn-Phong Lighting Model
//                   : �� ���� View Space ���� ó��.
float4 FogCalc(float depth)
{
	float fogWidth = fogDepthMax - fogDepthMin;
	float fog = (fogWidth - (depth - fogDepthMin)) / fogWidth;

	if (fog < 0)
		return 0;
	else if (fog > 1)
		return 1;

	return fog;
}



//VS ��� ����ü.
struct v2p
{
	float4 pos : SV_POSITION;		// [�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
    float2 uv : TEXCOORD0;
	float4 pos3d : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : ���� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

v2p VS_Main(
	float4 pos : POSITION,    //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
	float4 nrm : NORMAL,      //[�Է�] ��� normal ��
	float2 uv : TEXCOORD0    //[�Է�] �ؽ�ó ��ǥ Texture Coordiates.
)
{
	v2p o = (v2p)0;

	// ���� ��ȯ.(World Transform) 
	pos = mul(pos, mTM);        // pos = pos * mTM

	// �þ�-�� ��ȯ (View Transform)
	pos = mul(pos, mView);

	o.pos3d = pos;
    
	// ���� ���� ��ȯ (Projection Transform)
	pos = mul(pos, mProj);

	// ���� ���.
	o.pos = pos;
	o.uv = uv;
	
    return o;
}



////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader ���� ������.
//

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : �ȼ� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 tex = texDiffuse.Sample(smpLinear, i.uv);
	
    clip(tex.a < 0.25f ? -1 : 1);

	float f = FogCalc(distance(0, i.pos3d));

	return (f * float4(0.0f, 0.0f, 0.0f, 0.6f * tex.a)) + ((1 - f) * fogColor);
}