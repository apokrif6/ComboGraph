// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Components/ActorComponent.h"
#include "ComboHandlerComponent.generated.h"

enum class ETriggerEvent : uint8;
struct FInputActionInstance;
class UInputMappingContext;
class UPlayMontageCallbackProxy;
class UComboGraphNode;
class UInputAction;
class UComboGraph;
/**
 * Main component to store and handle ComboGraph related data and logic
 */
UCLASS(ClassGroup=("Combo Graph"), Within=Character, meta=(BlueprintSpawnableComponent))
class COMBOGRAPHRUNTIME_API UComboHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UComboHandlerComponent();

	virtual void BeginPlay() override;

	bool GetCanContinueCombo() const { return bCanContinueCombo; }

	void SetCanContinueCombo(const bool InCanContinueCombo) { bCanContinueCombo = InCanContinueCombo; }

	UComboGraph* GetComboGraph() const { return ComboGraph; }

	bool GetIsComboActive() const { return bIsComboActive; }

protected:
	/**
	 * ComboGraphAsset with defined combo sequences
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UComboGraph> ComboGraph;

	/**
	 * All inputs which need to be considered as part of combo attacks should be in this mapping context
	 * Component binds on every Input Action inside it
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UInputMappingContext> ComboInputMappingContext;

	/**
	 * Component binds to this event at Input Actions to process combo
	 * By default it is Completed, but if you want to handle all InputActions by yourself, you can change it to Triggered
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	ETriggerEvent TriggerEventToProcessCombo = ETriggerEvent::Completed;

private:
	/**
	 * Main method for handling the Input Action
	 * Finds and plays suitable opener if no combo is active
	 * Or finds the corresponding attack in current combo sequence and continue combo
	 */
	void ProcessComboAttack(const UInputAction* InputAction);

	/**
	 * Bound to EnhancedInputComponent and called for every Input Action for specified mapping context
	 */
	void OnAnyInputActionTriggered(const FInputActionInstance& ActionInstance);

	/**
	 * Looking for openers with specific Input Action
	 */
	UComboGraphNode* FindOpenerForInputAction(const UInputAction* InputAction) const;

	/**
	 * Looking for suitable attack in combo sequence for specific Input Action
	 */
	UComboGraphNode* FindNextAttackForCombo(const UComboGraphNode* CurrentComboGraphNode,
	                                        const UInputAction* InputAction) const;

	/**
	 * Create MontageCallbackProxy and play montage
	 */
	void PlayComboMontage(const UComboGraphNode* ComboGraphNode);

	/**
	 * Cached owner character
	 */
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

	/**
	 * Ongoing node in current combo sequence
	 */
	UPROPERTY()
	UComboGraphNode* CurrentComboGraphAttack = nullptr;

	/**
	 * Callback for ongoing attack montage
	 */
	UPROPERTY()
	UPlayMontageCallbackProxy* PlayMontageCallbackProxy = nullptr;

	/**
	 * If any combo is active
	 */
	bool bIsComboActive = false;

	/**
	 * Can we continue ongoing combo sequence
	 */
	bool bCanContinueCombo = false;

#pragma region Delegates
	UFUNCTION()
	void OnMontageBlendOut(FName NotifyName);

	UFUNCTION()
	void OnMontageInterrupted(FName NotifyName);

	UFUNCTION()
	void OnMontageCompleted(FName NotifyName);
#pragma endregion Delegates
};
