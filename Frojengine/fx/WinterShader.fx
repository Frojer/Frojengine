#define LIGHT_SIZE 5

// ��� ����
cbuffer cbWVP : register(b0)
{
    matrix mTM; // ���� ���. 
    matrix mView; // �� ��ȯ ���. 
    matrix mWV; // ����-�� ��ȯ ���. 
    matrix mProj; // ���� ��ȯ ���.
};

struct Light
{
    float range;
    uint lightType;
    bool useLight;
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 position;
    float3 direction;
};

cbuffer cbLight : register(b1)
{
    Light light[LIGHT_SIZE];
};

cbuffer ConstBuffer : register(b2)
{
    float4 mtrlDiffuse;
    float4 mtrlAmbient;
    float4 mtrlSpecular;
    float4 autumnColor;
    float4 fogColor;
    float mtrlPower;
    int seasonIndex;
    float fogDepthMin;
    float fogDepthMax;
};


//�ؽ�ó ��ü ����: �������� ���޵�.
//Texture2D texDiffuse;
//Texture2D texWinterMask;
//Texture2D texDiffuse2;

//�������� ���� ����. (�⺻���� t0)
Texture2D texDiffuse : register(t0);
Texture2D texWinterMask : register(t1);

//�ؽ�ó ���÷�. (��������)
SamplerState smpLinear;



//VS ��� ����ü.
struct v2p
{
    float4 pos : SV_POSITION; // [�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
    float4 col : COLOR0; // [�Է�] (������) �ȼ��� ����. (Pixel Color : "Diffuse")
    float4 pos3d : TEXCOORD0;
    float4 nrm3d : TEXCOORD1;
    float2 uv : TEXCOORD2; // [�Է�] �ؽ�ó ��ǥ
};

////////////////////////////////////////////////////////////////////////////// 
//
// ���� ��� : ����Ʈ ������ �� ����. Lambert Lighting Model
//          : �� ���� View Space ���� ó��.
float4 LightCalc(float4 nrm, float4 pos)
{
    float4 N = nrm;
    float4 L;
    float4 lightPos;
    float4 dir;
    float4 diff = 0;
    float dist;

    for (int i = 0; i < LIGHT_SIZE; i++)
    {
        if (light[i].useLight)
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
// ���ݻ籤 ���� ��� : ���� �� ����. Blinn-Phong Lighting Model
//                  : �� ���� View Space ���� ó��.
float4 SpecLight(float4 pos, float4 nrm)
{
    float4 N = nrm;
    float4 L;
    float4 E;
    float4 H;
    float4 spec = 0;

    for (int i = 0; i < LIGHT_SIZE; i++)
    {
        if (light[i].lightType != 0)
            continue;

        L = float4(light[i].direction, 0);
        L = mul(L, mView);

		// �ü����� ���.
        E = normalize(-pos);

		// �������� ���.
        H = normalize(L + E);

		// ���� ��� 
        spec += pow(max(dot(N, H), 0), mtrlPower) * light[i].specular * mtrlSpecular;
    }

    spec.w = 1;

    return spec;
}


float4 Mask(float4 a, float4 b, float4 mask)
{
    return (a * (1 - mask)) + (b * mask);
}



////////////////////////////////////////////////////////////////////////////// 
//
// ���ݻ籤 ���� ��� : ���� �� ����. Blinn-Phong Lighting Model
//                   : �� ���� View Space ���� ó��.
float4 FogCalc(float depth)
{
    float fogWidth = fogDepthMax - fogDepthMin;
    float fog = (fogWidth - (depth - fogDepthMin)) / fogWidth;

    if (fog < 0)        return 0;
    else if (fog > 1)   return 1;
	
    return fog;
}




////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : ���� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

v2p VS_Main(
	float4 pos : POSITION, //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
	float4 nrm : NORMAL, //[�Է�] ��� normal ��
	float2 uv : TEXCOORD0 //[�Է�] �ؽ�ó ��ǥ Texture Coordiates.
)
{
    v2p o = (v2p) 0;
    float4 diff = mtrlDiffuse;

    pos.w = 1;
    nrm.w = 0;

	// ���� ��ȯ.(World Transform) 
    pos = mul(pos, mTM); // pos = pos * mTM

	// �þ�-�� ��ȯ (View Transform)
    pos = mul(pos, mView);

    o.pos3d = pos;
    o.nrm3d = mul(nrm, mWV);
    o.nrm3d = normalize(o.nrm3d);

	// ���� ���.(Lighting)
	//diff = LightCalc(nrm, pos);
	//diff = SpecLight(pos, nrm);
	//diff = LightCalc(o.nrm3d, o.pos3d);

	// ���� ���� ��ȯ (Projection Transform)
    pos = mul(pos, mProj);

	// ���� ���.
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

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : �ȼ� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(v2p i) : SV_TARGET //[���] ����.(�ʼ�), "����Ÿ��" ���� ����մϴ�.
{
    float4 tex = texDiffuse.Sample(smpLinear, i.uv);
    float4 winterMask = texWinterMask.Sample(smpLinear, i.uv);
    float4 diff;
    
    switch (seasonIndex)
    {
        case 0:
            diff = tex;
            break;
	
        case 1:
            diff = tex * autumnColor;
            diff *= 1.5f;
            break;

        case 2:
            diff = ((tex * (1 - winterMask)) + winterMask);
            break;
    }


    diff *= i.col * LightCalc(i.nrm3d, i.pos3d);
    diff += SpecLight(i.pos3d, i.nrm3d);
    
    diff.a = tex.a;

    clip(diff.a < 0.5f ? -1 : 1);

    float f = FogCalc(distance(0, i.pos3d));

    return (f * diff) + ((1 - f) * fogColor);
}