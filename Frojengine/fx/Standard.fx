#define LIGHT_SIZE 5

// 상수 버퍼
cbuffer cbWVP
{
	matrix mTM;     // 월드 행렬. 
	matrix mView;   // 뷰 변환 행렬. 
	matrix mWV;     // 월드-뷰 변환 행렬. 
	matrix mProj;   // 투영 변환 행렬.
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

//VS 출력 구조체.
struct v2p
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////// 
//
// 조명 계산 : 램버트 라이팅 모델 적용. Lambert Lighting Model
//          : 뷰 공간 View Space 기준 처리.
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
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

v2p VS_Main(
	float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
	float4 nrm : NORMAL,      //[입력] 노멀 normal ★
	float2 uv : TEXCOORD0    //[입력] 텍스처 좌표 Texture Coordiates.
)
{
	float4 diff = mtrlDiffuse;

	//* 아래의 테스트를 수행하기 전에  
	//* VS 에 상수 버퍼가 설정되어 있어야 합니다.    
	pos.w = 1;

	//월드 변환.(World Transform) 
	pos = mul(pos, mTM);        //pos = pos * mTM

								//시야-뷰 변환 (View Transform)
	pos = mul(pos, mView);

	//조명 계산.(Lighting)
	diff = LightCalc(nrm, pos);

	//원근 투영 변환 (Projection Transform)
	pos = mul(pos, mProj);

	//정보 출력.
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
// Pixel Shader 전역 데이터.
//

//텍스처 객체 변수: 엔진에서 공급됨.
//Texture2D texDiffuse;
//Texture2D texWinterMask;
//Texture2D texDiffuse2;

//레지스터 직접 지정. (기본값은 t0)
Texture2D texDiffuse : register(t0);

//텍스처 셈플러. (엔진지정)
SamplerState smpLinear;

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(
	float4 pos : SV_POSITION,   //[입력] (보간된) 픽셀별 좌표. (Screen, 2D)
	float4 col : COLOR0,        //[입력] (보간된) 픽셀별 색상. (Pixel Color : "Diffuse")
	float2 uv : TEXCOORD0		//[입력] 텍스처 좌표
) : SV_TARGET               //[출력] 색상.(필수), "렌더타겟" 으로 출력합니다.
{
	float4 tex = texDiffuse.Sample(smpLinear, uv);

	float4 diff = tex * col;

	clip(diff.a < 0.5f ? -1 : 1);

	return diff;
}