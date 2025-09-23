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
    // --- 安全检查 1: 检查输入是否有效 ---
   // 如果ID无效，或者没有提供DataTable，或者UI控件本身不存在，就无法继续。
    if (InAvatarID.IsNone() || !InHeroDataTable || !PlayerIcon)
    {
        // 在这种情况下，直接设置一个默认的“问号”或空白头像，然后返回。
        SetPlayerIcon(DefaultPlayerIcon);
        return;
    }

    // --- 查找数据 ---
    const FString ContextString(TEXT("AvatarLookup for BloodSlot"));
    // 在DataTable中查找与ID匹配的数据行
    FHeroData* HeroDataRow = InHeroDataTable->FindRow<FHeroData>(InAvatarID, ContextString);

    // --- 安全检查 2: 检查是否找到了数据行 ---
    if (!HeroDataRow)
    {
        // 如果在DataTable中找不到这个ID，也设置默认头像。
        SetPlayerIcon(DefaultPlayerIcon);
        return;
    }

    // --- 加载逻辑 ---
    // 从数据行中获取头像的软指针。此时，资产还并未加载到内存中。
    TSoftObjectPtr<UTexture2D> AvatarTexturePtr = HeroDataRow->InGameAvatar;

    // **优化**: 检查一下这个软指针是否已经加载过了。
    // 如果玩家选择了和别人一样的英雄，这个纹理可能已经被加载了。
    if (AvatarTexturePtr.IsValid())
    {
        // 如果已经加载，就没必要再异步请求了，直接使用。
        SetPlayerIcon(AvatarTexturePtr.Get());
        return;
    }

    // **核心异步加载**: 如果资产尚未加载，我们请求异步加载它。
    // 这不会阻塞游戏线程，游戏会继续流畅运行。
    FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
    Streamable.RequestAsyncLoad(AvatarTexturePtr.ToSoftObjectPath(),

        // C++ Lambda: 提供一个“回调”函数。这个函数会在加载完成时被调用。
        [this, AvatarTexturePtr]()
        {
            // --- 安全检查 3: 在回调函数内部再次检查 ---
            // 异步加载可能需要几帧甚至更长时间。在这期间，玩家可能已经退出了游戏，
            // 这个BloodSlot控件可能已经被销毁了。所以必须进行检查！
            if (!this || !IsValid(this))
            {
                // 如果控件已经失效，什么都不做，直接返回。
                return;
            }

            // 再次检查软指针在加载后是否有效
            if (AvatarTexturePtr.IsValid())
            {
                // 加载成功！现在可以安全地设置图标了。
                SetPlayerIcon(AvatarTexturePtr.Get());
            }
            else
            {
                // 如果因为某些原因（如路径错误）加载失败，也设置默认头像。
                SetPlayerIcon(DefaultPlayerIcon);
            }
        }
    );
}

// 您已有的辅助函数，用于设置笔刷
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
            // 如果传入的Texture是空的（例如DefaultPlayerIcon没有设置），则隐藏图标
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
