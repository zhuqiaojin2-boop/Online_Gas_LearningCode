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
    if (WidgetTree) // WidgetTree����������ؼ��������ӿؼ�
    {
        // WidgetTree->ForEachWidget ��һ�����Ա������в㼶�ӿؼ��ĺ���
        WidgetTree->ForEachWidget([this](UWidget* Widget)
            {
                // ����ӿؼ��Ƿ�ʵ�������ǵĽӿ�
                IWidgetControllerInterface* WCInterface = Cast<IWidgetControllerInterface>(Widget);
                if (WCInterface)
                {
                    // ���ʵ���ˣ��͵��ýӿں����������ǵ�Controller���ݸ���
                    WCInterface->Execute_SetWidgetController(Widget, WidgetController);
                }
            });
    }
}
	