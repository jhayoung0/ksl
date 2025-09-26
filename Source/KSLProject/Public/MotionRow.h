// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "MotionRow.generated.h"

class UAnimMontage;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMotionRow : public FTableRowBase
{
	GENERATED_BODY();

	FMotionRow();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName topic; // 주제 (0,1,2) 0: 숫자, 1: 의사소통 , 2: 랜덤
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName label; // 퀘스쳔넘버 // 0,1,2, (unique_id)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName seq_count  ;  // 1,2 // 시퀀스 몇개 인지
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString sign_text; // display word
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* signMontage; // 실행 모션
};
