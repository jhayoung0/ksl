// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProblemUI.generated.h"

/**
 * 
 */
UCLASS()
class KSLPROJECT_API UProblemUI : public UUserWidget
{
	GENERATED_BODY()

public: // display word
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_signword;
		
};
