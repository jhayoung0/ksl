// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JudgeResult.h"
#include "MainCharacter.h"
#include "ProblemUI.h"
#include "GameFramework/PlayerController.h"
#include "SignPlayerController.generated.h"

/**
 * 
 */
 
 // 게임 플로우
 UENUM(BlueprintType)
 enum class GamePlayState : uint8
 {
     MainMenu,		// 메인메뉴
     TopicSelect,	// 주제 선택
     LessonIntro,	// 문제 시작하기 전 애니메이션 및 카운트다운?
     PlayingOneSequence, // 문제가 주어짐 (캐릭터 모션)
 	 PlayingTwoSequence, // 두번째 시퀀스 있는 경우
     WaitingJudge,	// 정답 판정 기다리기
     Feedback,		// 정답 여부 표시
     TopicComplete, // 해당 주제 끝 
     ResultMenu		// 종료 화면
 };




UCLASS()
class KSLPROJECT_API ASignPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	// 현재 상태 가져오기
	UPROPERTY(EditAnywhere)
	GamePlayState CurrentState;
	GamePlayState PreviousState;
	
	// 전환함수
	UFUNCTION(BlueprintCallable)
	void SetGameState(GamePlayState NewState);
	
	// 토픽 (메인ui에서 버튼 클릭에 따라 저장됨)
	UPROPERTY()
	int32 topicId;

	// 실행될 첫번재 문제
	UPROPERTY()
	FString startquestionid;

	// 현재 판정 결과
	UPROPERTY(BlueprintReadOnly, Category="Judge")
	FJudgeResult JudgeResult;
	
	
	// 현재 진행되는 문제
	UPROPERTY()
	FString label = TEXT("-1");
	
	// 메인 캐릭터
	UPROPERTY()
	AMainCharacter* MainChar; 
	
	// 전환될때 실행될 함수
	UFUNCTION(BlueprintCallable) // 문제 시작
	void BeginLesson();
	
	UFUNCTION(BlueprintCallable)
	void PlayOneSeqMotion();

	UFUNCTION(BlueprintCallable)
	void PlayTwoSeqMotion();
	
	UFUNCTION(BlueprintCallable) // 정답 처리
	void OnJudgeDone(bool bCorrect);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sign")
	UDataTable* MotionTable = nullptr;
	

	/////////////////////////////위젯 관련/////////////////////////////////////
	UFUNCTION()
	void ShowWidgetForState(GamePlayState State); // 위젯 스위칭(옵션)

	// 현재 위젯
	UPROPERTY(EditAnywhere)
	UUserWidget* CurrentWidget;


	UPROPERTY()
	UProblemUI* PlayingUI = nullptr;

	
	// 위젯들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> PlayingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> FeedbackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> TopicCompleteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> ResultClass;
	
};
