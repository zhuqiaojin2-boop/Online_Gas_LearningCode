// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RL_UW_BloodSlot.h"
#include "WidgetController/AttributeWidgetController.h"
#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
void URL_UW_BloodSlot::SetWidgetController_Implementation(UWidgetController* InWidgetController)
{
	AttributeWidgetController = Cast<UAttributeWidgetController>(InWidgetController);

    if (AttributeWidgetController)
    {
        AttributeWidgetController->OnHealthChanged.AddDynamic(this, &URL_UW_BloodSlot::OnHealthValueChanged);
        AttributeWidgetController->OnMaxHealthChanged.AddDynamic(this, &URL_UW_BloodSlot::OnMaxHealthValueChanged);
    }
}

void URL_UW_BloodSlot::UpdateBloodPercentage()
{
    CalculateBloodPercentage();
    BloodProgressBar->SetPercent(BloodPercentage);
}

void URL_UW_BloodSlot::SetPlayerIcon(UTexture2D* Texture)
{
    if (PlayerIcon && Texture)
    {
        FSlateBrush Brush = PlayerIcon->GetBrush();

        Brush.SetResourceObject(Texture);

        Brush.ImageSize = PlayerIconSize;

        PlayerIcon->SetBrush(Brush);
    }
}

void URL_UW_BloodSlot::InitializeForPlayer(FPlayerState_AttributeData AttributeData)
{
    Health = AttributeData.Health;
    MaxHealth = AttributeData.MaxHealth;
    LocalPlayerID = AttributeData.PlayerID;
    RequestAvatarLoad(AttributeData.AvatarID, AttributeData.HeroDataTable);
    UpdateBloodPercentage();
    PlayerNameBlock->SetText(FText::FromName(AttributeData.PlayerName));
}

void URL_UW_BloodSlot::UpdateAvatar(FName InAvatarID)
{
    
}

void URL_UW_BloodSlot::RequestAvatarLoad(FName InAvatarID, UDataTable* InHeroDataTable)
{
    // --- ��ȫ��� 1: ��������Ƿ���Ч ---
   // ���ID��Ч������û���ṩDataTable������UI�ؼ��������ڣ����޷�������
    if (InAvatarID.IsNone() || !InHeroDataTable || !PlayerIcon)
    {
        // ����������£�ֱ������һ��Ĭ�ϵġ��ʺš���հ�ͷ��Ȼ�󷵻ء�
        SetPlayerIcon(DefaultPlayerIcon);
        return;
    }

    // --- �������� ---
    const FString ContextString(TEXT("AvatarLookup for BloodSlot"));
    // ��DataTable�в�����IDƥ���������
    FHeroData* HeroDataRow = InHeroDataTable->FindRow<FHeroData>(InAvatarID, ContextString);

    // --- ��ȫ��� 2: ����Ƿ��ҵ��������� ---
    if (!HeroDataRow)
    {
        // �����DataTable���Ҳ������ID��Ҳ����Ĭ��ͷ��
        SetPlayerIcon(DefaultPlayerIcon);
        return;
    }

    // --- �����߼� ---
    // ���������л�ȡͷ�����ָ�롣��ʱ���ʲ�����δ���ص��ڴ��С�
    TSoftObjectPtr<UTexture2D> AvatarTexturePtr = HeroDataRow->InGameAvatar;

    // **�Ż�**: ���һ�������ָ���Ƿ��Ѿ����ع��ˡ�
    // ������ѡ���˺ͱ���һ����Ӣ�ۣ������������Ѿ��������ˡ�
    if (AvatarTexturePtr.IsValid())
    {
        // ����Ѿ����أ���û��Ҫ���첽�����ˣ�ֱ��ʹ�á�
        SetPlayerIcon(AvatarTexturePtr.Get());
        return;
    }

    // **�����첽����**: ����ʲ���δ���أ����������첽��������
    // �ⲻ��������Ϸ�̣߳���Ϸ������������С�
    FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
    Streamable.RequestAsyncLoad(AvatarTexturePtr.ToSoftObjectPath(),

        // C++ Lambda: �ṩһ�����ص�������������������ڼ������ʱ�����á�
        [this, AvatarTexturePtr]()
        {
            // --- ��ȫ��� 3: �ڻص������ڲ��ٴμ�� ---
            // �첽���ؿ�����Ҫ��֡��������ʱ�䡣�����ڼ䣬��ҿ����Ѿ��˳�����Ϸ��
            // ���BloodSlot�ؼ������Ѿ��������ˡ����Ա�����м�飡
            if (!this || !IsValid(this))
            {
                // ����ؼ��Ѿ�ʧЧ��ʲô��������ֱ�ӷ��ء�
                return;
            }

            // �ٴμ����ָ���ڼ��غ��Ƿ���Ч
            if (AvatarTexturePtr.IsValid())
            {
                // ���سɹ������ڿ��԰�ȫ������ͼ���ˡ�
                SetPlayerIcon(AvatarTexturePtr.Get());
            }
            else
            {
                // �����ΪĳЩԭ����·�����󣩼���ʧ�ܣ�Ҳ����Ĭ��ͷ��
                SetPlayerIcon(DefaultPlayerIcon);
            }
        }
    );
}

// �����еĸ����������������ñ�ˢ
void URL_UW_BloodSlot::SetPlayerIcon(UTexture2D* Texture)
{
    if (PlayerIcon)
    {
        if (Texture)
        {
            PlayerIcon->SetBrushFromTexture(Texture);
            PlayerIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            // ��������Texture�ǿյģ�����DefaultPlayerIconû�����ã���������ͼ��
            PlayerIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void URL_UW_BloodSlot::OnHealthValueChanged(int32 PlayerId, float NewValue)
{
    if(LocalPlayerID == PlayerId)
    {   
        Health = NewValue;
        UpdateBloodPercentage();
    }
}

void URL_UW_BloodSlot::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void URL_UW_BloodSlot::OnMaxHealthValueChanged(int32 PlayerId, float NewValue)
{
    if (LocalPlayerID == PlayerId)
    {
       MaxHealth = NewValue;
       UpdateBloodPercentage();
    }
}

void URL_UW_BloodSlot::CalculateBloodPercentage()
{
    if (MaxHealth > 0.0f)
    {
        BloodPercentage = Health / MaxHealth;
    }
    else
    {
        BloodPercentage = 0.0f;
    }
}
