#pragma once
#include "EditorUI.h"
#include <Engine/CTexture.h>

// ��������Ʈ ������ UI Ŭ����
// - �ؽ�ó ��Ʋ�󽺿��� ��������Ʈ�� �����ϰ� �����ϴ� ����� ����
class SpriteEditorUI
    : public EditorUI
{
private:
    // =========================================
    // �ؽ�ó ���� ��� ����
    // =========================================
    Ptr<CTexture>   m_Atlas;                            // ���� �������� ��Ʋ�� �ؽ�ó
    ImVec2          m_TextureDisplayPos;                // �ؽ�ó�� ������ ǥ�õǴ� ȭ����� ��ġ
    float           m_ZoomLevel;                        // ���� �� ���� (1.0f�� �⺻��)

    // =========================================
    // ���� ���� ���� ��� ����
    // =========================================
    Vec2            m_LeftTop;                          // ���õ� ������ �»�� ��ǥ (�ȼ�)
    Vec2            m_Slice;                            // ���õ� ������ ũ�� (�ȼ�)
    Vec2            m_Background;                       // ��� ���� ũ�� (�ȼ�)
    Vec2            m_Offset;                           // ������ �� (�ȼ�)
    bool            m_HasSelection;                     // ���� ������ �ִ��� ����

    // =========================================
    // �巡�� ���� ��� ����
    // =========================================
    bool            m_bDragging;                        // �巡�� ������ ����
    Vec2            m_DragStart;                        // �巡�� ���� ��ġ (�ؽ�ó ��ǥ)
    Vec2            m_DragCurrent;                      // ���� �巡�� ��ġ (�ؽ�ó ��ǥ)

    // =========================================
    // �д�(�̹��� �̵�) ���� ��� ����
    // =========================================
    bool            m_bPanning;                         // �д� ������ ����
    Vec2            m_PanOffset;                        // ���� �д� ������
    ImVec2          m_LastMousePos;                     // ���� ���콺 ��ġ

    // =========================================
    // UI ���� ��� ����
    // =========================================
    ImVec2          m_ImageMin;                         // �̹��� ǥ�� ������ �»�� (��ũ�� ��ǥ)
    ImVec2          m_ImageMax;                         // �̹��� ǥ�� ������ ���ϴ� (��ũ�� ��ǥ)
    ImVec2          m_PreviewSize;                      // ������ �̹��� ũ��
    bool            m_MouseOverAtlas;                   // ���콺�� Atlas ���� ���� �ִ���

    // =========================================
    // ��������Ʈ �̸� ���� ��� ����
    // =========================================
    static char    m_SpriteNameBuf[50];                 // ��������Ʈ �̸� �Է� ����
    bool           m_ShowNameError;                     // �̸� ���� �˾� ǥ�� ����

public:
    // ������/�Ҹ���
    SpriteEditorUI();
    ~SpriteEditorUI();

    // EditorUI �����Լ� �������̵�
    virtual void Tick() override {};                    // �� ������ ����
    virtual void Render() override;                     // UI ������
    virtual void Render_Update() override {};           // ������� ����

private:
    // =========================================
    // UI ������ ���� �Լ�
    // =========================================
    void RenderAtlasView();                             // ��Ʋ�� �ؽ�ó �� ������
    void RenderControlPanel();                          // ���� �г� ������
    void RenderPreview();                               // ������ ���� ������
    void RenderSelectionRect();                         // ���� ���� ������
    void RenderNameErrorPopup();                        // �̸� ���� �˾� ������

    // =========================================
    // �Է� ó�� ���� �Լ�
    // =========================================
    void HandleDragging();                              // �巡�� ó��
    void HandlePanning();                               // �д�(��Ŭ�� �巡��) ó��

    // =========================================
    // ���� ���� ���� �Լ�
    // =========================================
    void UpdateSelectionFromDrag();                     // �巡�� ������ ���� ���� ������Ʈ
    void UpdateSelectionRect();                         // ���� ���� ���� �� ������Ʈ

    // =========================================
    // ��������Ʈ ���� ���� �Լ�
    // =========================================
    void CreateSprite(const wstring& _Name);            // ��������Ʈ ����
    wstring GetUniqueSpriteNameFromInput() const;       // �Էµ� �̸����� ������ ��������Ʈ �̸� ����

    // =========================================
    // ��ƿ��Ƽ �Լ�
    // =========================================
    void LoadTextureFromFile();                         // ���Ͽ��� �ؽ�ó �ε�
    wstring OpenFileDialog(const wchar_t* filter);      // ���� ���̾�α� ǥ��
    Vec2 GetAdjustedDisplayPos() const;                 // �д��� ����� ���÷��� ��ġ ���
    Vec2 ScreenToTextureSpace(const ImVec2& screenPos); // ȭ�� ��ǥ�� �ؽ�ó ��ǥ�� ��ȯ
    bool IsInImageBounds(const ImVec2& point);          // ��ǥ�� �̹��� ���� ���� �ִ��� Ȯ��


};