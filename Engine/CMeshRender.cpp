#include "pch.h"
#include "CMeshRender.h"

#include "assets.h"
#include "CTransform.h"
#include "CFlipbookPlayer.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (!GetActive())
		return;

	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Binding();
	}

	// ��ġ����
	Transform()->Binding();


	// ������ Mtrl �� Mesh�� ����� ����־�����, RenderComponent���� �޾ƿ;���.
	// ����� ���̴�
	GetMaterial()->Binding();

	// ������
	GetMesh()->Render();

	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Clear();
	}

}

