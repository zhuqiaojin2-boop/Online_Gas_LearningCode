// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/RL_HUD_Base.h"

#include "HUD/RL_UW_BloodSlot.h"

void URL_HUD_Base::NativeOnInitialized()
{
	BloodSlot = CreateWidget<URL_UW_BloodSlot>(this, BloodSlotClass);
}
	