cbuffer cbWVP : register(b0)
{
	matrix mTM;
	matrix mView;
	matrix mWV;
	matrix mProj;
};

cbuffer ConstBuffer : register(b1)
{
	float4 col;
};


float4 VS_Main(float4 pos : POSITION) : SV_POSITION
{
	pos.w = 1;
	pos = mul(pos, mTM);
	pos = mul(pos, mView);
	pos = mul(pos, mProj);
	
	return pos;
}


float4 PS_Main() : SV_TARGET
{
	return col;
}