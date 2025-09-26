// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "MotionRow.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMotionRow : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName sign_num; // 퀘스쳔넘버 // 0,1,2,

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName seq_Id ;  // 0, 1

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName seq_count  ;  // 1,2 // 시퀀스 몇개 인지

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString sign_text; // display word
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* signMontage; // 실행 모션
};