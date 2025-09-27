// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ProblemUI.generated.h"

/**
 * 
 */
UCLASS()
class KSLPROJECT_API UProblemUI : public UUserWidget
{
	GENERATED_BODY()

public: // display word
	UPROPERTY(meta = (BindWidget))  // bind하려고
	class UTextBlock* txt_signword;


	// 단어 갱신용 
	UFUNCTION()
	void SetSignWord(const FText& InText);
	
};
