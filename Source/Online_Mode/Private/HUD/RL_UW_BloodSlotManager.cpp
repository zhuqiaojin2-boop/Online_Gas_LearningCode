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
        // ���ת��ʧ�ܣ����磬����һ����ս�߻��������͵�PlayerState����
        // ���InitData�����أ���ֹUI��ʾ������Ϣ��
        InitData = FPlayerState_AttributeData();
        return;
    }

    // 2. ��PlayerState���򵥵�����
    InitData.PlayerID = LocalPS->GetPlayerId(); // APlayerState�����ú�������ȡΨһID
    InitData.PlayerName = LocalPS->GetPlayerName(); // APlayerState�����ú���
    InitData.IsLeader = LocalPS->IsLeader(); // ֱ�ӷ�������PS�ж�������� (����ͨ��Getter)

    // 3. ��PlayerState��AttributeSet�л�ȡ����ֵ
    URL_AS_Player* AttributeSet = LocalPS->GetAttributeSet();
    if (AttributeSet)
    {
        InitData.Health = AttributeSet->GetHealth();
        InitData.MaxHealth = AttributeSet->GetMaxHealth();
    }
    else
    {
        // ���AttributeSet��û��ʼ�����ṩ��ȫ��Ĭ��ֵ
        InitData.Health = 0.f;
        InitData.MaxHealth = 1.f; // ��Ϊ1.0f�Է�ֹUI����ٷֱ�ʱ����0
    }
    InitData.AvatarID = LocalPS->GetAvatarID();
    InitData.HeroDataTable = this->AvatarDataTable; // ��DataTable�����ô��ݹ�ȥ

}

void URL_UW_BloodSlotManager::CreatePlayerBloodSlots()
{
    if (!WidgetController) return;

    // ��WidgetController��ȡ����PlayerState������
    const TArray<APlayerState*>& AllPlayerStates = WidgetController->GetPlayerStates(); // ����Ҫ��WC�����һ��Getter

    for (APlayerState* PS : AllPlayerStates)
    {
        // 1. ���� BloodSlot
        URL_UW_BloodSlot* NewBloodSlot = CreateWidget<URL_UW_BloodSlot>(this, BloodSlotClass);
        FPlayerState_AttributeData InitData;
        MakeInitDataFromPS(PS, InitData);

        if (NewBloodSlot)
        {
            // 2. **����Ҫ�Ĳ���**: ���ó�ʼ��������ע����ݺͳ�ʼ����
            NewBloodSlot->InitializeForPlayer(InitData);

            // 3. �� WidgetController ���ø���������ȥ��δ���ĸ����¼�
            NewBloodSlot->SetWidgetController(WidgetController);

            // 4. ��ӵ�UI���
            BloodSlotBox->AddChild(NewBloodSlot);
        }
    }
}
