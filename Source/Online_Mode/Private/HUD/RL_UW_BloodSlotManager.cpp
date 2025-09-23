// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RL_UW_BloodSlotManager.h"
#include "WidgetController/WidgetController.h"
#include "HUD/RL_UW_BloodSlot.h"
#include "PS/RL_PS_Base.h"
#include "AS/RL_AS_Player.h"
void URL_UW_BloodSlotManager::SetWidgetController_Implementation(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
}

void URL_UW_BloodSlotManager::NativeOnInitialized()
{
	
}

void URL_UW_BloodSlotManager::OnPlayerStateAdded(APlayerState* NewPlayerState)
{
	
}

void URL_UW_BloodSlotManager::OnPlayerStateRemoved(APlayerState* RemovedPlayerState)
{
	
}

void URL_UW_BloodSlotManager::MakeInitDataFromPS(APlayerState* PS, FPlayerState_AttributeData& InitData)
{
    ARL_PS_Base* LocalPS = Cast<ARL_PS_Base>(PS);
    if (!LocalPS)
    {
        // 如果转换失败（例如，这是一个观战者或其他类型的PlayerState），
        // 清空InitData并返回，防止UI显示错误信息。
        InitData = FPlayerState_AttributeData();
        return;
    }

    // 2. 从PlayerState填充简单的数据
    InitData.PlayerID = LocalPS->GetPlayerId(); // APlayerState的内置函数，获取唯一ID
    InitData.PlayerName = LocalPS->GetPlayerName(); // APlayerState的内置函数
    InitData.IsLeader = LocalPS->IsLeader(); // 直接访问您在PS中定义的属性 (或者通过Getter)

    // 3. 从PlayerState的AttributeSet中获取属性值
    URL_AS_Player* AttributeSet = LocalPS->GetAttributeSet();
    if (AttributeSet)
    {
        InitData.Health = AttributeSet->GetHealth();
        InitData.MaxHealth = AttributeSet->GetMaxHealth();
    }
    else
    {
        // 如果AttributeSet还没初始化，提供安全的默认值
        InitData.Health = 0.f;
        InitData.MaxHealth = 1.f; // 设为1.0f以防止UI计算百分比时除以0
    }
    InitData.AvatarID = LocalPS->GetAvatarID();
    InitData.HeroDataTable = this->AvatarDataTable; // 将DataTable的引用传递过去

}

void URL_UW_BloodSlotManager::CreatePlayerBloodSlots()
{
    if (!WidgetController) return;

    // 从WidgetController获取所有PlayerState的数组
    const TArray<APlayerState*>& AllPlayerStates = WidgetController->GetPlayerStates(); // 你需要在WC中添加一个Getter

    for (APlayerState* PS : AllPlayerStates)
    {
        // 1. 创建 BloodSlot
        URL_UW_BloodSlot* NewBloodSlot = CreateWidget<URL_UW_BloodSlot>(this, BloodSlotClass);
        FPlayerState_AttributeData InitData;
        MakeInitDataFromPS(PS, InitData);

        if (NewBloodSlot)
        {
            // 2. **最重要的步骤**: 调用初始化函数，注入身份和初始数据
            NewBloodSlot->InitializeForPlayer(InitData);

            // 3. 将 WidgetController 设置给它，让它去绑定未来的更新事件
            NewBloodSlot->SetWidgetController(WidgetController);

            // 4. 添加到UI面板
            BloodSlotBox->AddChild(NewBloodSlot);
        }
    }
}
