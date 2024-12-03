// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComboNodeType.h"
#include "UObject/Object.h"
#include "ComboGraphNode.generated.h"

enum class EComboNodeAnimationInterruptionType : uint8;
class UComboGraphEdge;
class UInputAction;
class UComboGraph;

/**
 * Parent for all combo sequence nodes
 */
UCLASS(Blueprintable, Abstract)
class COMBOGRAPHRUNTIME_API UComboGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UComboGraphNode();
	virtual ~UComboGraphNode() override;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboMontage = nullptr;

	UPROPERTY()
	TObjectPtr<UComboGraph> Graph = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UComboGraphNode>> ParentNodes;

	UPROPERTY()
	TArray<TObjectPtr<UComboGraphNode>> ChildrenNodes;

	UPROPERTY()
	TMap<TObjectPtr<UComboGraphNode>, TObjectPtr<UComboGraphEdge>> Edges;

	UFUNCTION(BlueprintCallable, Category = "ComboGraphNode")
	virtual UComboGraphEdge* GetEdge(UComboGraphNode* ChildNode);

	UFUNCTION(BlueprintCallable, Category = "ComboGraphNode")
	UComboGraph* GetGraph() const { return Graph; }

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FText NodeTitle;

	UPROPERTY()
	FLinearColor BackgroundColor;

	UPROPERTY()
	TSubclassOf<UComboGraph> CompatibleGraphType;

	UPROPERTY()
	FText ContextMenuName;
#endif

#if WITH_EDITOR
	virtual FLinearColor GetBackgroundColor() const { return BackgroundColor; }

	virtual bool IsNameEditable() const { return true; }

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle) { NodeTitle = NewTitle; }

	virtual bool CanCreateConnection(UComboGraphNode* Other, FText& ErrorMessage) { return true; }

	virtual bool CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);

	virtual bool CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
#endif

	EComboNodeType GetComboNodeType() const { return ComboNodeType; }

protected:
	EComboNodeType ComboNodeType = EComboNodeType::None;
};
