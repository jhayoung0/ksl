// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JudgeResult.h"
#include "LevelSequencePlayer.h"
#include "MainCharacter.h"
#include "MainUI.h"
#include "ProblemUI.h"
#include "NiagaraFunctionLibrary.h"
#include "WebSocketActor.h"
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
     Playing, // 문제가 주어짐 (캐릭터 모션)
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

	// 웹소켓 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWebSocketActor* WebSocketActorRef = nullptr;
	
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
	
	
	// 현재 진행되는 문제 (대기중일때 -1 해찬이거)
	UPROPERTY()
	FString label = TEXT("-1");

	// 현재 진행 문제 (내 거) 
	UPROPERTY()
	FString CurLabel;
	
	// 문제 순서 인덱스
	TArray<FString> QuestionOrder;

	// 현재 몇번째 문제인지
	int32 CurIdx = 0;

	
	// 메인 캐릭터
	UPROPERTY()
	AMainCharacter* MainChar; 
	
	// 전환될때 실행될 함수
	UFUNCTION(BlueprintCallable) // 문제 시작
	void BeginLesson();

	UFUNCTION(BlueprintCallable)
	void SelectTopic();
	
	UFUNCTION(BlueprintCallable)
	void PlayMotion();

	UFUNCTION(BlueprintCallable)
	void JudgeNextStep(bool bInIsCorrect);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sign")
	UDataTable* MotionTable = nullptr;
	
	UPROPERTY()
	bool bCorrect;
	
	/////////////////////////////위젯 관련/////////////////////////////////////
	UFUNCTION()
	void ShowWidgetForState(GamePlayState State); // 위젯 스위칭(옵션)

	// 현재 위젯
	UPROPERTY(EditAnywhere)
	UUserWidget* CurrentWidget;
	
	// 위젯들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UMainUI> MainMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UProblemUI> PlayingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> FeedbackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> TopicCompleteClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|UI")
	TSubclassOf<UUserWidget> ResultClass;


	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|Sound")
	class USoundBase* TrueSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|Sound")
	class USoundBase* FalseSound;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|Sound")
	class USoundBase* NextLevelSound;

	// vfx
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flow|VFX")
	UNiagaraSystem* NS_TrueSystem;
	
	
	// 레벨 시퀀스
	UPROPERTY(EditAnywhere)
	ULevelSequencePlayer* LevelEndPlayer;
	
	UFUNCTION()
	void PlayTopicSelectSeq();
	
	UFUNCTION()
	void PlayLevelEndSeq();

	UFUNCTION()
	void OnPlayTopicSelectSeqFinished();
	
	UFUNCTION()
	void OnLevelEndSequenceDelayed();

	
	UFUNCTION()
    void OnLevelEndSequenceFinished();
        
	
};
