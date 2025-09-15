// FlipbookEditorUI.h
#pragma once
#include "EditorUI.h"
#include <Engine/CFlipbook.h>
#include <Engine/CSprite.h>

class FlipbookEditorUI 
	: public EditorUI
{
private:
	// ���� �������� �ø��� ����
	Ptr<CFlipbook>          m_EditFlipbook;					// �������� �ø���
	int                     m_SelectedIdx;					// ���õ� ��������Ʈ �ε���

	// �÷��̾� ������ ����
	bool                    m_bPlaying;						// ��� ������ ����
	float                   m_AccTime;						// ���� ������ ���� �ð�
	float                   m_FPS;							// ��� �ӵ� (������/��)
	bool                    m_Repeat;						// �ݺ� ��� ����
	float					m_PreviewZoom;


	// UI ����
	bool                    m_ShowNameError;				// �̸� ���� �˾� ǥ�� ����
	char                    m_NameBuf[50];					// �ø��� �̸� �Է� ����

	int						m_InsertIndex;					// ��������Ʈ ���� ��ġ (-1: ���� �߰�)

public:
	// EditorUI �����Լ� ������
	virtual void Tick() override;							// ������ ������Ʈ
	virtual void Render() override;							// UI ������
	virtual void Render_Update() override {};				// ������� ����

private:
	// Flipbook �ҷ����� ���� �Լ���
	void ShowFlipbookSelectUI();							// Flipbook ���� UI ǥ��
	void LoadFlipbook(Ptr<CFlipbook> _pFlipbook);			// ���õ� Flipbook �ε�

	// === Flipbook ���� ���� UI ===
	void RenderSpriteList();								// ��������Ʈ ���
	void RenderSpriteProperties();							// ���õ� ��������Ʈ �Ӽ�

	// === �÷��̾� ������ UI ===
	void RenderPlayerControls();							// �÷��̾� ����
	void RenderPreviewDisplay();

	// === ���� UI ===
	void RenderNameErrorPopup();							// �̸� ���� �˾�

	// === ��������Ʈ ���� ===
	void ShowSpriteSelectUI();								// ��������Ʈ ���� UI ǥ��
	void SelectSprite(void* _ptr);							// ��������Ʈ ���� �ݹ�
	void AddSprite(Ptr<CSprite> _pSprite);					// ��������Ʈ �߰�
	void RemoveSprite(int _idx);							// ��������Ʈ ����
	void MoveSprite(int _from, int _to);					// ��������Ʈ ���� ����

	// === ����/�ҷ����� ===
	void SaveFlipbook();									// �ø��� ����
	wstring GetUniqueFlipbookName() const;					// ������ �ø��� �̸� ����

	// === ��ƿ��Ƽ ===
	void UpdatePreview();									// ������ ������Ʈ
	void PlayPreview();										// ������ ���
	void StopPreview();										// ������ ����
	wstring OpenFileDialog(const wchar_t* filter);


public:
	FlipbookEditorUI();
	~FlipbookEditorUI();
};