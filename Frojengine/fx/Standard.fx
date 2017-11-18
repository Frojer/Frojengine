// ��� ����
cbuffer cbWVP
{
	matrix mTM;     // ���� ���. 
	matrix mView;   // �� ��ȯ ���. 
	matrix mWV;     // ����-�� ��ȯ ���. 
	matrix mProj;   // ���� ��ȯ ���.
};

cbuffer ConstBuffer
{
	float4 mtrlDiffuse;
	float4 mtrlAmbient;
	float4 mtrlSpecular;
	float mtrlPower;
};

//VS ��� ����ü.
struct v2p
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};


///*
// ���� ó�� �Լ�.��
float4 Light(float4 nrm, float4 pos);




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
	//* �Ʒ��� �׽�Ʈ�� �����ϱ� ����  
	//* VS �� ��� ���۰� �����Ǿ� �־�� �մϴ�.    
	pos.w = 1;

	//���� ��ȯ.(World Transform) 
	pos = mul(pos, mTM);        //pos = pos * mTM

								//�þ�-�� ��ȯ (View Transform)
	pos = mul(pos, mView);

	////���� ���.(Lighting)
	float4 diff = mtrlDiffuse;

	//���� ���� ��ȯ (Projection Transform)
	pos = mul(pos, mProj);

	//���� ���.
	v2p o = (v2p)0;
	o.pos = pos;
	o.col = diff;
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

//�ؽ�ó ��ü ����: �������� ���޵�.
//Texture2D texDiffuse;
//Texture2D texWinterMask;
//Texture2D texDiffuse2;

//�������� ���� ����. (�⺻���� t0)
Texture2D texDiffuse : register(t0);

//�ؽ�ó ���÷�. (��������)
SamplerState smpLinear;

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : �ȼ� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(
	float4 pos : SV_POSITION,   //[�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
	float4 col : COLOR0,        //[�Է�] (������) �ȼ��� ����. (Pixel Color : "Diffuse")
	float2 uv : TEXCOORD0		//[�Է�] �ؽ�ó ��ǥ
) : SV_TARGET               //[���] ����.(�ʼ�), "����Ÿ��" ���� ����մϴ�.
{
	float4 tex = texDiffuse.Sample(smpLinear, uv);

	float4 diff = tex * col;

	clip(diff.a < 0.5f ? -1 : 1);

	return diff;
}