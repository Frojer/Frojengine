#define LIGHT_SIZE 5

// 상수 버퍼
cbuffer cbWVP : register(b0)
{
	matrix mTM;     // 월드 행렬. 
	matrix mView;   // 뷰 변환 행렬. 
	matrix mWV;     // 월드-뷰 변환 행렬. 
	matrix mProj;   // 투영 변환 행렬.
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
	float mtrlPower;
};


//텍스처 객체 변수: 엔진에서 공급됨.
//Texture2D texDiffuse;
//Texture2D texWinterMask;
//Texture2D texDiffuse2;

//레지스터 직접 지정. (기본값은 t0)
Texture2D texDiffuse : register(t0);

//텍스처 셈플러. (엔진지정)
SamplerState smpLinear;



//VS 출력 구조체.
struct v2p
{
	float4 pos : SV_POSITION;		// [입력] (보간된) 픽셀별 좌표. (Screen, 2D)
	float4 col : COLOR0;			// [입력] (보간된) 픽셀별 색상. (Pixel Color : "Diffuse")
	float4 pos3d : TEXCOORD0;		
	float4 nrm3d : TEXCOORD1;		
	float2 uv  : TEXCOORD2;			// [입력] 텍스처 좌표
};

////////////////////////////////////////////////////////////////////////////// 
//
// 조명 계산 : 램버트 라이팅 모델 적용. Lambert Lighting Model
//          : 뷰 공간 View Space 기준 처리.
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

			//뷰공간으로 정보를 변환.
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
// 정반사광 조명 계산 : 블린퐁 모델 적용. Blinn-Phong Lighting Model
//                  : 뷰 공간 View Space 기준 처리.
float4 SpecLight(float4 pos, float4 nrm)
{
	float4 N = nrm;
	float4 L;
	float4 E;
	float4 H;
	float4 spec = 0;

	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		L = float4(light[i].direction, 0);

		// 시선백터 계산.
		E = normalize(-pos);

		// 하프벡터 계산.
		H = normalize(L + E);

		// 조명 계산 
		spec += pow(max(dot(N, H), 0), mtrlPower) * light[i].specular * mtrlSpecular;
	}

	spec.w = 1;

	return spec;
}




////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

v2p VS_Main(
	float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
	float4 nrm : NORMAL,      //[입력] 노멀 normal ★
	float2 uv : TEXCOORD0    //[입력] 텍스처 좌표 Texture Coordiates.
)
{
	v2p o = (v2p)0;
	float4 diff = mtrlDiffuse;

	pos.w = 1;
	nrm.w = 0;

	// 월드 변환.(World Transform) 
	pos = mul(pos, mTM);        // pos = pos * mTM

	// 시야-뷰 변환 (View Transform)
	pos = mul(pos, mView);

	o.pos3d = pos;
	o.nrm3d = mul(nrm, mWV);
	o.nrm3d = normalize(o.nrm3d);

	// 조명 계산.(Lighting)
	//diff = LightCalc(nrm, pos);
	//diff = SpecLight(pos, nrm);
	//diff = LightCalc(o.nrm3d, o.pos3d);

	// 원근 투영 변환 (Projection Transform)
	pos = mul(pos, mProj);

	// 정보 출력.
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
// Pixel Shader 전역 데이터.
//

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(v2p i) : SV_TARGET               //[출력] 색상.(필수), "렌더타겟" 으로 출력합니다.
{
	float4 tex = texDiffuse.Sample(smpLinear, i.uv);

	float4 diff = tex * i.col;

	diff = LightCalc(i.nrm3d, i.pos3d);

	diff += SpecLight(i.pos3d, i.nrm3d);
	diff *= tex;

	clip(diff.a < 0.5f ? -1 : 1);

	return diff;
}