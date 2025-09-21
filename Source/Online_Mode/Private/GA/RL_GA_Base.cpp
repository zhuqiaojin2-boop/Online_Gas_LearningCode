// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/RL_GA_Base.h"
#include <Character/RL_Character_Base.h>
#include "AbilitySystemComponent.h"


FGameplayEffectHandleSpecContainer URL_GA_Base::ConstructHandleSpecsFromContainer(FGameplayEffectContainer Container)
{
	FGameplayEffectHandleSpecContainer ReturnSpec;

    FGameplayEffectContextHandle ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());

    for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.EffectClasses)
    {
        if (!EffectClass)
        {
            continue;
        }

        FGameplayEffectSpecHandle TempHandle = MakeOutgoingGameplayEffectSpec(EffectClass, GetAbilityLevel());

        if (TempHandle.IsValid())
        {
            //    **关键步骤**: 将我们自己创建的ContextHandle，赋值给Spec内部的Context。
            //    我们需要通过 SpecHandle.Data.Get() 来获取到 FGameplayEffectSpec 的可写指针。
            if (TempHandle.Data)
            {
                TempHandle.Data->SetContext(ContextHandle);
            }

            // 4. 将配置好的SpecHandle添加到返回数组中。
            ReturnSpec.EffectSpecHandles.Add(TempHandle);
        }
    }

    return ReturnSpec;
}
TArray<FActiveGameplayEffectHandle> URL_GA_Base::ApplyEffectForEachSpecHandles(FGameplayEffectHandleSpecContainer& HandleSpecContainer)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : HandleSpecContainer.EffectSpecHandles)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, HandleSpecContainer.TargetData));
	}
	return AllEffects;

}
//默认伤害计算公式
void URL_GA_Base::CalculateFinalDamage(float &BaseDamgage,float Strength)
{
    //可复杂化
    BaseDamgage = Strength * 1.25f;
}
float URL_GA_Base::GetAttribute(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAttribute Attribute) const
{
    return ActorInfo->AbilitySystemComponent.Get()->GetNumericAttribute(Attribute);
}


