#ifndef _SHADER
#define _SHADER

cbuffer Transform : register(b0)
{
    // �� �켱
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
}

Texture2D g_tex : register(t0);

// Vertex Shader 
struct VS_IN
{
    // semantic
    float3 vPos : POSITION;     // ���� ��ġ ���� (���� ������)   
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};



struct VS_OUT
{
    float4 vPosition : SV_Position; // System Value �� ���ڷ�, ��ȯ�� ���� ��ġ Position���� �Է¹��� 3D ��ǥ�� ��ȯ�Ѵ�. EX) MVP��ȯ ���� ����Ǿ� 3D ��ǥ�� 2D ȭ�鿡 ǥ�õǴ� ����
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float3 vLocalPos = _in.vPos;
    
    //=======================================================
    // -Matrix�� ������ Transform�� vec4 position�� scale�� �־��� �� �������.
    //float3 vPos = (_in.vPos * vObjectScale.xyz) + vObjectPos.xyz;
    //output.vPosition = float4(vPos,1.f);
    
    //-���� , ����, ��ȯ�� �����ϱ� ��
    // Local -> World ��ȯ
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    //=======================================================
    
    
    // Local -> World
    // LocalPos�� ������ǥ�� 1�ΰ� ���Ⱑ �̵���Ʈ��.
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
    // UV ���� ��
    //float4 vColor = g_tex.Sample(g_sam_0, float2(0.5f, 0.5f));
    //return _in.vColor;
    
    
    
    // UV �� ��
    float4 vColor = g_tex.Sample(g_sam_0, _in.vUV);
    return vColor;

}


#endif