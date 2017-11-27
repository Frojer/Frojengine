#define LIGHT_SIZE 5

// ��� ����
cbuffer cbWVP
{
	matrix mTM;     // ���� ���. 
	matrix mView;   // �� ��ȯ ���. 
	matrix mWV;     // ����-�� ��ȯ ���. 
	matrix mProj;   // ���� ��ȯ ���.
};

struct Light
{
	float4 diffuse;
	float4 ambient;
	float3 position;
	float3 direction;
	float range;
	uint lightType;
	bool useLight;
};

cbuffer cbLight
{
	Light light[LIGHT_SIZE];
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

////////////////////////////////////////////////////////////////////////////// 
//
// ���� ��� : ����Ʈ ������ �� ����. Lambert Lighting Model
//          : �� ���� View Space ���� ó��.
float4 LightCalc(float4 nrm, float4 pos)
{
	float4 N = nrm;    N.w = 0;
	float4 L;
	float4 lightPos;
	float4 dir;
	float4 diff;
	float dist;

	N = mul(N, mWV);
	N = normalize(N);

	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		//if (light[i].useLight)
		{
			L = float4(light[i].direction, 0);

			//��������� ������ ��ȯ.
			L = mul(L, mView);

			switch (light[i].lightType)
			{
			// Direction Light
			case 0:
				diff += max(dot(N, L), 0) * light[i].diffuse * mtrlDiffuse;
				diff += light[i].ambient * mtrlAmbient;
				break;
			
			// Point Light
			case 1:
				lightPos = mul(float4(light[i].position, 1), mView);
				dir = normalize(lightPos - pos);
				dist = distance(pos, lightPos);
				
				if (dist < light[i].range)
				{
					diff += max(dot(N, dir), 0) * abs((dist / light[i].range) - 1) * light[i].diffuse * mtrlDiffuse;
					diff += light[i].ambient * mtrlAmbient * abs((dist / light[i].range) - 1);
				}
				break;
			}
		}
	}
	
	diff.a = mtrlDiffuse.a;

	return diff;
}




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
	float4 diff = mtrlDiffuse;

	//* �Ʒ��� �׽�Ʈ�� �����ϱ� ����  
	//* VS �� ��� ���۰� �����Ǿ� �־�� �մϴ�.    
	pos.w = 1;

	//���� ��ȯ.(World Transform) 
	pos = mul(pos, mTM);        //pos = pos * mTM

								//�þ�-�� ��ȯ (View Transform)
	pos = mul(pos, mView);

	//���� ���.(Lighting)
	diff = LightCalc(nrm, pos);

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