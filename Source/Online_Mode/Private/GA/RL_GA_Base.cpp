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
            //    **�ؼ�����**: �������Լ�������ContextHandle����ֵ��Spec�ڲ���Context��
            //    ������Ҫͨ�� SpecHandle.Data.Get() ����ȡ�� FGameplayEffectSpec �Ŀ�дָ�롣
            if (TempHandle.Data)
            {
                TempHandle.Data->SetContext(ContextHandle);
            }

            // 4. �����úõ�SpecHandle��ӵ����������С�
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
//Ĭ���˺����㹫ʽ
void URL_GA_Base::CalculateFinalDamage(float &BaseDamgage,float Strength)
{
    //�ɸ��ӻ�
    BaseDamgage = Strength * 1.25f;
}
float URL_GA_Base::GetAttribute(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAttribute Attribute) const
{
    return ActorInfo->AbilitySystemComponent.Get()->GetNumericAttribute(Attribute);
}


