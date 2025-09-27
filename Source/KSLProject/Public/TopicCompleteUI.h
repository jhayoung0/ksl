// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopicCompleteUI.generated.h"

class ASignPlayerController;
/**
 * 
 */
UCLASS()
class KSLPROJECT_API UTopicCompleteUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_othertopic;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_exit;

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void clickOthertopic();

	UFUNCTION()
	void clickExit();

	UPROPERTY()
	ASignPlayerController* pc;
};
