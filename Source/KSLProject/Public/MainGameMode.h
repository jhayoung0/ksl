// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JudgeResult.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */




UCLASS()
class KSLPROJECT_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Judge")
	FJudgeResult LastJudge;

	// 판정 결과 갱신
	UFUNCTION(BlueprintCallable, Category="Judge")
	void UpdateJudgeResult(FString label, bool bInIsCorrect);

	// 특정 문제/시퀀스 판정 조회
	UFUNCTION(BlueprintCallable, Category="Judge")
	bool GetIsCorrectFor(FString label, bool& bOutIsCorrect) const;
	
	
};
