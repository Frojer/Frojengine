//
//
// Demo.fx : 기본 셰이더 소스.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

 

////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main
//
float4 VS_Main( float4 pos : POSITION ) : SV_POSITION
{
    return pos;
}





////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main( float4 pos : SV_POSITION ) : SV_Target
{
	float4 col = {1, 1, 1, 1};

	return col;
}



/**************** end of file "Demo.fx" ***********************/
