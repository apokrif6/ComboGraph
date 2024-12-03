// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Components/ComboHandlerComponent.h"

#include "ComboGraph.h"
#include "ComboGraphRuntimeLog.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayMontageCallbackProxy.h"
#include "GameFramework/Character.h"
#include "Nodes/ComboGraphOpenerNode.h"

UComboHandlerComponent::UComboHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UComboHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!ComboInputMappingContext)
	{
		UE_LOG(ComboGraphRuntimeLog, Error, TEXT("[%s - %hs]: have invalid ComboInputMappingContext"),
		       *GetOwner()->GetName(), __FUNCTION__)
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(ComboGraphRuntimeLog, Error, TEXT("[%s - %hs]: can't process combos for non-character owner"),
		       *GetOwner()->GetName(), __FUNCTION__)
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetInstigatorController());
	if (!PlayerController)
	{
		UE_LOG(ComboGraphRuntimeLog, Error, TEXT("[%s - %hs]: can't bind inputs to owner without PlayerController"),
		       *OwnerCharacter->GetName(), __FUNCTION__)
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!EnhancedInputLocalPlayerSubsystem)
	{
		UE_LOG(ComboGraphRuntimeLog, Error,
		       TEXT("[%s - %hs]: can't bind inputs to owner without EnhancedInputLocalPlayerSubsystem"),
		       *OwnerCharacter->GetName(), __FUNCTION__)
		return;
	}

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(ComboInputMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(ComboGraphRuntimeLog, Error,
		       TEXT("[%s - %hs]: can't bind inputs to owner without UEnhancedInputComponent"), *GetOwner()->GetName(),
		       __FUNCTION__)
		return;
	}

	//prevent duplicated mappings (e.g. AttackOne for gamepad and keyboard should be treated as one input)
	TSet<const UInputAction*> BoundActions;
	for (const FEnhancedActionKeyMapping& Mapping : ComboInputMappingContext->GetMappings())
	{
		if (Mapping.Action && !BoundActions.Contains(Mapping.Action))
		{
			BoundActions.Add(Mapping.Action);

			EnhancedInputComponent->BindAction(Mapping.Action, TriggerEventToProcessCombo, this,
			                                   &ThisClass::OnAnyInputActionTriggered);
		}
	}
}

void UComboHandlerComponent::OnAnyInputActionTriggered(const FInputActionInstance& ActionInstance)
{
	ProcessComboAttack(ActionInstance.GetSourceAction());
}

void UComboHandlerComponent::ProcessComboAttack(const UInputAction* InputAction)
{
	if (bIsComboActive)
	{
		if (!bCanContinueCombo)
		{
			UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: can't continue combo"),
			       *GetOwner()->GetName(), __FUNCTION__)
			return;
		}

		CurrentComboGraphAttack = FindNextAttackForCombo(CurrentComboGraphAttack, InputAction);
	}
	else
	{
		CurrentComboGraphAttack = FindOpenerForInputAction(InputAction);
	}

	if (!CurrentComboGraphAttack)
	{
		UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: could not suitable CurrentComboGraphAttack"),
		       *GetOwner()->GetName(), __FUNCTION__)
		return;
	}

	PlayComboMontage(CurrentComboGraphAttack);
}

UComboGraphNode* UComboHandlerComponent::FindOpenerForInputAction(const UInputAction* InputAction) const
{
	if (!ComboGraph)
	{
		UE_LOG(ComboGraphRuntimeLog, Error, TEXT("[%s - %hs]: have invalid ComboGraph"), *GetOwner()->GetName(),
		       __FUNCTION__)
		return nullptr;
	}

	for (UComboGraphNode* ComboGraphNode : ComboGraph->AllNodes)
	{
		if (ComboGraphNode->IsA<UComboGraphOpenerNode>() && ComboGraphNode->InputAction == InputAction)
		{
			return ComboGraphNode;
		}
	}

	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: can't find opener with %s InputAction"),
	       *GetOwner()->GetName(), __FUNCTION__, *InputAction->GetName())
	return nullptr;
}

UComboGraphNode* UComboHandlerComponent::FindNextAttackForCombo(const UComboGraphNode* CurrentComboGraphNode,
                                                                const UInputAction* InputAction) const
{
	if (!CurrentComboGraphNode)
	{
		UE_LOG(ComboGraphRuntimeLog, Warning, TEXT("[%s - %hs]: get invalid CurrentComboGraphNode"),
		       *GetOwner()->GetName(), __FUNCTION__)
		return nullptr;
	}

	for (UComboGraphNode* ComboGraphNode : CurrentComboGraphNode->ChildrenNodes)
	{
		if (ComboGraphNode->InputAction == InputAction)
		{
			return ComboGraphNode;
		}
	}

	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: can't find next combo sequence attack"),
	       *GetOwner()->GetName(),
	       __FUNCTION__)
	return nullptr;
}

void UComboHandlerComponent::PlayComboMontage(const UComboGraphNode* ComboGraphNode)
{
	if (!ComboGraphNode)
	{
		UE_LOG(ComboGraphRuntimeLog, Warning, TEXT("[%s - %hs]: get invalid ComboGraphNode"), *GetOwner()->GetName(),
		       __FUNCTION__)
		return;
	}

	if (!ComboGraphNode->ComboMontage)
	{
		UE_LOG(ComboGraphRuntimeLog, Warning, TEXT("[%s - %hs]: get ComboGraphNode %s with invalid ComboMontage"),
		       *GetOwner()->GetName(), __FUNCTION__, *ComboGraphNode->GetName())
		return;
	}

	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		OwnerCharacter->GetMesh(), ComboGraphNode->ComboMontage);
	PlayMontageCallbackProxy->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);

	bIsComboActive |= true;

	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: play combo montage %s"), *GetOwner()->GetName(),
	       __FUNCTION__, *ComboGraphNode->ComboMontage->GetName())
}

void UComboHandlerComponent::OnMontageBlendOut(FName NotifyName)
{
	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: combo sequence get montage blend out"),
	       *GetOwner()->GetName(), __FUNCTION__)
}

void UComboHandlerComponent::OnMontageInterrupted(FName NotifyName)
{
	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: combo sequence get montage interrupted"),
	       *GetOwner()->GetName(), __FUNCTION__)

	bIsComboActive = false;
}

void UComboHandlerComponent::OnMontageCompleted(FName NotifyName)
{
	UE_LOG(ComboGraphRuntimeLog, Display, TEXT("[%s - %hs]: combo sequence get montage completed"),
	       *GetOwner()->GetName(), __FUNCTION__)

	bIsComboActive = false;
}
