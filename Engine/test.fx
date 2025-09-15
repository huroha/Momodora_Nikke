#ifndef _TEST
#define _TEST

#include "value.fx"


#define WIDTH           g_int_0
#define HEIGHT          g_int_1
#define CLEAR_COLOR     g_vec4_0


// UnorderedAccess
// Read Write
RWTexture2D<float4> g_Target : register(u0);

// Group �� ������ ���� ����
// - HLSL 5.0 ���� �׷�� �ִ� 1024���� ������ ���� ����
[numthreads(32, 32, 1)]

// ComputeShader Semantic
// SV_GroupID           : �����尡 ���� �׷��� ID(�׷� �ε���)
// SV_GroupThreadID     : �����尡 ���� �׷� ������ �ڽ��� ��ġ�� 3���� �ε����� ǥ��
// SV_GroupIndex        : SV_GroupThreadID �� 1���� �ε����� ��ȯ
// SV_DispatchThreadID  : ��ü ��� �׷��� ��Ʋ� ��� ������ �� �ش� �������� ������ ID

void CS_ClearTexture(int3 _id : SV_DispatchThreadID)
{
    if (WIDTH <= _id.x || HEIGHT <= _id.y)
    {
        return;
    }
    
    g_Target[_id.xy] = CLEAR_COLOR;
}




#endif

