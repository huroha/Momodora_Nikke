#include "pch.h"
#include "RigidBodyUI.h"

#include <Engine/CRigidBody2D.h>
#include "CImGuiMgr.h"
#include "Inspector.h"

RigidBodyUI::RigidBodyUI()
    : ComponentUI("RigidBodyUI", COMPONENT_TYPE::RIGIDBODY2D)
{

}

RigidBodyUI::~RigidBodyUI()
{

}


void RigidBodyUI::Render_Update()
{
    ComponentTitle("RigidBody2D");

    CRigidBody2D* pRigidBody = GetTargetObject()->RigidBody2D();

    ImGui::SameLine(350);
    if (ImGui::Button("X"))
    {
        DeleteComponentTask(pRigidBody);

        Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
        pInspector->DeleteComponentCheck(GetType());
    }

    Vec3    vVelocity = pRigidBody->GetVelocity();
    float   Mass = pRigidBody->GetMass();
    float   MaxSpeed = pRigidBody->GetMaxSpeed();
    float   GravityAccel = pRigidBody->GetGravityAccel();
    float   GravityMaxSpeed = pRigidBody->GetGravityMaxSpeed();
    float   JumpSpeed = pRigidBody->GetJumpSpeed();
    float   Friction = pRigidBody->GetFriction();
    bool    UseGravity = pRigidBody->IsUseGravity();
    


    ImGui::Text("RigidBodyMode");
    ImGui::SameLine(100);
    const char* szRigidMode[2] = { "TOPVIEW", "PLATFOMER" };
    int eMode = (int)pRigidBody->GetMode();

    if (ImGui::Combo("##comboSpawnShape", &eMode, szRigidMode, 2))
    {
        pRigidBody->SetMode((RIGIDBODY_MODE)eMode);
    }


    ImGui::Text("Velocity");
    ImGui::SameLine(100);
    if (ImGui::DragFloat3("##Velocity", vVelocity))
    {
        pRigidBody->SetVelocity(vVelocity);
    }

    ImGui::Text("Mass");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##Mass", &Mass))
    {
        pRigidBody->SetMass(Mass);
    }

    ImGui::Text("MaxSpeed");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##MaxSpeed", &MaxSpeed))
    {
        pRigidBody->SetMaxSpeed(MaxSpeed);
    }
    
    ImGui::Text("GravityAccel");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##GravityAccel", &GravityAccel))
    {
        pRigidBody->SetGravityAccelScale(GravityAccel);
    }

    ImGui::Text("GravityMaxSpeed");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##GravityMaxSpeed", &GravityMaxSpeed))
    {
        pRigidBody->SetMaxGravitySpeed(GravityMaxSpeed);
    }

    ImGui::Text("JumpSpeed");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##JumpSpeed", &JumpSpeed))
    {
        pRigidBody->SetJumpSpeed(JumpSpeed);
    }

    ImGui::Text("Friction");
    ImGui::SameLine(100);
    if (ImGui::DragFloat("##Friction", &Friction))
    {
        pRigidBody->SetFriction(Friction);
    }

    ImGui::Text("UseGravity");
    ImGui::SameLine(100);
    if (ImGui::Checkbox("##BustRepeat", &UseGravity))
    {
        pRigidBody->UseGravity(UseGravity);
    }

}
