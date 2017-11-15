// ��� ����
cbuffer cbDEFAULT : register(b0)
{
	matrix mTM;     //���� ���. 
	matrix mView;   //�� ��ȯ ���. 
	matrix mWV;     //����-�� ��ȯ ���. 
	matrix mProj;   //���� ��ȯ ���. 
};


//���� ������ ������ۡ�
cbuffer cbLIGHT : register(b1)
{
	float3 LitPosition;
	float3 LitDirection; //���� ����.
	float4 LitDiffuse;   //�� ���� : Ȯ�걤 Diffuse Light.
	float4 LitAmbient;   //���� ���� : �ֺ��� Ambient Light.
	float  LitRange;     //�� ���� �Ÿ�.
	bool   LitOn;        //���� ���뿩��.
};

//���� ������ ������ۡ�
cbuffer cbMATERIAL : register(b2)
{
	float4 MtrlDiffuse;  //�� ����(Ȯ�걤) �� �ݻ���(%) 
	float4 MtrlAmbient;  //���� ����(�ֺ���) �� �ݻ���(%) 
};


//���� ������ ������ۡ�
cbuffer cbPointLIGHT : register(b3)
{
	float3 LitPointPosition;
	float3 LitPointDirection; //���� ����.
	float4 LitPointDiffuse;   //�� ���� : Ȯ�걤 Diffuse Light.
	float4 LitPointAmbient;   //���� ���� : �ֺ��� Ambient Light.
	float  LitPointRange;     //�� ���� �Ÿ�.
	bool   LitPointOn;        //���� ���뿩��.
};




//VS ��� ����ü.
struct VSOutput
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

VSOutput VS_Main(
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
	float4 diff = 1;
	//if (LitOn)
	//{
	//	diff = Light(nrm, pos);
	//}

	//���� ���� ��ȯ (Projection Transform)
	pos = mul(pos, mProj);

	//���� ���.
	VSOutput o = (VSOutput)0;
	o.pos = pos;
	o.col = diff;
	o.uv = uv;

	return o;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ��� : ����Ʈ ������ �� ����. Lambert Lighting Model
//          : �� ���� View Space ���� ó��.
float4 Light(float4 nrm, float4 pos)
{
	float4 N = nrm;    N.w = 0;
	float4 L = float4(LitDirection, 0);

	//��������� ������ ��ȯ.
	N = mul(N, mWV);
	L = mul(L, mView);

	N = normalize(N);

	//���� ��� 
	float4 diff = max(dot(N, L), 0) * LitDiffuse * MtrlDiffuse;
	float4 amb = LitAmbient * MtrlAmbient;
	float4 ptDiff = 0;

	// ����Ʈ ����Ʈ ���
	if (LitPointOn)
	{
		float4 pointPos = mul(float4(LitPointPosition, 1), mView);

		float4 dir = normalize(pointPos - pos);
		float dist = distance(pos, pointPos);

		if (dist < LitPointRange)
		{
			ptDiff = max(dot(N, dir), 0) * abs((dist / LitPointRange) - 1) * LitPointDiffuse * MtrlDiffuse;
			ptDiff += LitPointAmbient * MtrlAmbient * abs((dist / LitPointRange) - 1);
		}
	}

	return diff + amb + ptDiff;
}
//*/



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


cbuffer AlphaTest : register(b0)
{
	float alphaTestValue;
}


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
	/*float4 tex = texDiffuse.Sample(smpLinear, uv);

	float4 diff = tex * col;

	clip(diff.a < 0.5f ? -1 : 1);
*/
	return 1;
}