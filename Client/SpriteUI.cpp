#include "pch.h"
#include "SpriteUI.h"
#include <Engine/CPathMgr.h>

SpriteUI::SpriteUI()
	: AssetUI("Sprite", ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Render_Update()
{
	AssetTitle();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();



	// 재질을 파일로 저장하기
	if (ImGui::Button("SAVE"))
	{
		Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
		assert(pMtrl.Get());

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pMtrl->GetKey();
		pMtrl->Save(strFilePath);
	}
}