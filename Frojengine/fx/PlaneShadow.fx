// 상수 버퍼
cbuffer cbWVP : register(b0)
{
	matrix mTM;     // 월드 행렬. 
	matrix mView;   // 뷰 변환 행렬. 
	matrix mWV;     // 월드-뷰 변환 행렬. 
	matrix mProj;   // 투영 변환 행렬.
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
// 정반사광 조명 계산 : 블린퐁 모델 적용. Blinn-Phong Lighting Model
//                   : 뷰 공간 View Space 기준 처리.
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



//VS 출력 구조체.
struct v2p
{
	float4 pos : SV_POSITION;		// [입력] (보간된) 픽셀별 좌표. (Screen, 2D)
    float2 uv : TEXCOORD0;
	float4 pos3d : TEXCOORD1;
};

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

	// 월드 변환.(World Transform) 
	pos = mul(pos, mTM);        // pos = pos * mTM

	// 시야-뷰 변환 (View Transform)
	pos = mul(pos, mView);

	o.pos3d = pos;
    
	// 원근 투영 변환 (Projection Transform)
	pos = mul(pos, mProj);

	// 정보 출력.
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
// Pixel Shader 전역 데이터.
//

////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main(v2p i) : SV_TARGET
{
    float4 tex = texDiffuse.Sample(smpLinear, i.uv);
	
    clip(tex.a < 0.25f ? -1 : 1);

	float f = FogCalc(distance(0, i.pos3d));

	return (f * float4(0.0f, 0.0f, 0.0f, 0.6f * tex.a)) + ((1 - f) * fogColor);
}