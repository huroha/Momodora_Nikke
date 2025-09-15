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

	// 위치정보
	Transform()->Binding();


	// 원래는 Mtrl 과 Mesh를 멤버로 들고있엇지만, RenderComponent에서 받아와야함.
	// 사용할 쉐이더
	GetMaterial()->Binding();

	// 렌더링
	GetMesh()->Render();

	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Clear();
	}

}

