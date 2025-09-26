// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JudgeResult.generated.h"

/**
 * 서버에서 받은 데이터
 */

USTRUCT(BlueprintType)
struct KSLPROJECT_API FJudgeResult
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString label;			// 문제 unique id

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCorrect = false; // 정답 여부
};
