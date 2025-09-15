#ifndef _SHADER
#define _SHADER

cbuffer Transform : register(b0)
{
    // 행 우선
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
}

Texture2D g_tex : register(t0);

// Vertex Shader 
struct VS_IN
{
    // semantic
    float3 vPos : POSITION;     // 원래 위치 정보 (원본 데이터)   
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};



struct VS_OUT
{
    float4 vPosition : SV_Position; // System Value 의 약자로, 변환된 최종 위치 Position으로 입력받은 3D 좌표를 변환한다. EX) MVP변환 등이 적용되어 3D 좌표가 2D 화면에 표시되는 이유
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vLocalPos = _in.vPos;
    
    //=======================================================
    // -Matrix를 쓰기전 Transform에 vec4 position과 scale이 있었을 때 사용했음.
    //float3 vPos = (_in.vPos * vObjectScale.xyz) + vObjectPos.xyz;
    //output.vPosition = float4(vPos,1.f);
    
    //-월드 , 투영, 변환을 적용하기 전
    // Local -> World 변환
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    //=======================================================
    
    
    // Local -> World
    // LocalPos의 동차좌표가 1인건 여기가 이동파트임.
    float4 vWorldPos = mul(float4(vLocalPos, 1.f), g_matWorld);
    
    // World -> View
    float4 vViewPos = mul(vWorldPos, g_matView);
    
    // View -> Projection
    float4 vProjPos = mul(vViewPos, g_matProj);
    
    
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

SamplerState g_sam_0 : register(s0);

// Pixel Shader
float4 PS_Test(VS_OUT _in) : SV_Target
{
    // UV 적용 전
    //float4 vColor = g_tex.Sample(g_sam_0, float2(0.5f, 0.5f));
    //return _in.vColor;
    
    
    
    // UV 들어간 후
    float4 vColor = g_tex.Sample(g_sam_0, _in.vUV);
    return vColor;

}


#endif