// 상수 버퍼
cbuffer cbWVP
{
	matrix mTM;     // 월드 행렬. 
	matrix mView;   // 뷰 변환 행렬. 
	matrix mWV;     // 월드-뷰 변환 행렬. 
	matrix mProj;   // 투영 변환 행렬.
};

//VS 출력 구조체.
struct v2p
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};


///*
// 조명 처리 함수.★
float4 Light(float4 nrm, float4 pos);




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
	//* 아래의 테스트를 수행하기 전에  
	//* VS 에 상수 버퍼가 설정되어 있어야 합니다.    
	pos.w = 1;

	//월드 변환.(World Transform) 
	pos = mul(pos, mTM);        //pos = pos * mTM

								//시야-뷰 변환 (View Transform)
	pos = mul(pos, mView);

	////조명 계산.(Lighting)
	float4 diff = 1;

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