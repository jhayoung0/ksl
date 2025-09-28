// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameUI.generated.h"

/**
 * 
 */
UCLASS()
class KSLPROJECT_API UEndGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Txt_changed;

	UFUNCTION()
	void SetUIText(const FText& InText);

};
