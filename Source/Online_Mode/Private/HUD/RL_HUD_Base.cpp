// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RL_HUD_Base.h"
#include "HUD/RL_UW_BloodSlot.h"
#include "Blueprint/WidgetTree.h"
#include "WidgetController/WidgetController.h"

void URL_HUD_Base::NativeOnInitialized()
{
}

void URL_HUD_Base::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
}

void URL_HUD_Base::OnWidgetControllerSet()
{
    if (WidgetTree) // WidgetTree包含了这个控件的所有子控件
    {
        // WidgetTree->ForEachWidget 是一个可以遍历所有层级子控件的函数
        WidgetTree->ForEachWidget([this](UWidget* Widget)
            {
                // 检查子控件是否实现了我们的接口
                IWidgetControllerInterface* WCInterface = Cast<IWidgetControllerInterface>(Widget);
                if (WCInterface)
                {
                    // 如果实现了，就调用接口函数，将我们的Controller传递给它
                    WCInterface->Execute_SetWidgetController(Widget, WidgetController);
                }
            });
    }
}
	