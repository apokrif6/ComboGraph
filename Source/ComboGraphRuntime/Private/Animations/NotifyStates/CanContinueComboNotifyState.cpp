// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Animations/NotifyStates/CanContinueComboNotifyState.h"

#include "ComboGraphRuntimeLog.h"
#include "Components/ComboHandlerComponent.h"
#include "Interfaces/ComboHandlerInterface.h"

void UCanContinueComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UNLIKELY(!MeshComp))
	{
		UE_LOG(ComboGraphRuntimeLog, Warning, TEXT("[%hs]: get invalid MeshComp"), __FUNCTION__)
		return;
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp->GetOwner())
	{
		UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%hs]: no Owner for MeshCom"), __FUNCTION__)
		return;
	}

	if (const IComboHandlerInterface* AsComboHandlerInterface = Cast<IComboHandlerInterface>(MeshComp->GetOwner()))
	{
		AsComboHandlerInterface->GetComboHandlerComponent()->SetCanContinueCombo(true);
	}
	else
	{
		UE_LOG(ComboGraphRuntimeLog, Warning,
		       TEXT("[%hs]: used for character %s which is not implement IComboHandlerInterface"), __FUNCTION__,
		       *MeshComp->GetOwner()->GetName())
	}
}

void UCanContinueComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	if (UNLIKELY(!MeshComp))
	{
		UE_LOG(ComboGraphRuntimeLog, Warning, TEXT("[%hs]: get invalid MeshComp"), __FUNCTION__)
		return;
	}

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
	{
		UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%hs]: no Owner for MeshCom"), __FUNCTION__)
		return;
	}

	if (const IComboHandlerInterface* AsComboHandlerInterface = Cast<IComboHandlerInterface>(MeshComp->GetOwner()))
	{
		AsComboHandlerInterface->GetComboHandlerComponent()->SetCanContinueCombo(false);
	}
	else
	{
		UE_LOG(ComboGraphRuntimeLog, Warning,
		       TEXT("[%hs]: used for character %s which is not implement IComboHandlerInterface"), __FUNCTION__,
		       *MeshComp->GetOwner()->GetName())
	}
}
