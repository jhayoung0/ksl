// Fill out your copyright notice in the Description page of Project Settings.

#include "SignPlayerController.h"
#include "MotionRow.h"
#include "NetworkMessage.h"
#include "ProblemUI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void ASignPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("player"), FoundActors);

	if (FoundActors.Num() > 0)
	{
		MainChar = Cast<AMainCharacter>(FoundActors[0]);
		if (!MainChar)
		{
			//UE_LOG(LogTemp, Warning, TEXT("No Main char"));
		}
	}
	
	SetGameState(GamePlayState::MainMenu);

	// 이미지 보내지 않기
	label = TEXT("-1");

}

void ASignPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentState: %s"), *EnumName);
}

void ASignPlayerController::SetGameState(GamePlayState NewState)
{
	PreviousState = CurrentState;
	CurrentState  = NewState;

	switch (CurrentState)
	{
	case GamePlayState::MainMenu:
		ShowWidgetForState(GamePlayState::MainMenu);
		SetShowMouseCursor(true);
		break;
	case GamePlayState::TopicSelect:
		SelectTopic();
		break;
	case GamePlayState::LessonIntro:
		BeginLesson(); // 1회 호출
		break;
	case GamePlayState::Playing:
		ShowWidgetForState(GamePlayState::Playing); // 위젯 먼저 띄워주기
		PlayMotion();
		break;
	case GamePlayState::WaitingJudge:
		break;
	case GamePlayState::Feedback:
		break;
	case GamePlayState::TopicComplete:
		ShowWidgetForState(GamePlayState::TopicComplete); // 위젯 먼저 띄워주기
		MainChar->TopicComplete();
		break;
	case GamePlayState::ResultMenu:
		ShowWidgetForState(GamePlayState::ResultMenu); // 위젯 먼저 띄워주기
		break;
	}
}


// 토픽별로 문제 집합 생성
void ASignPlayerController::BeginLesson()
{

	// 이미지 보내지 않기
	label = TEXT("-1");
	
	// 문제 초기화
	QuestionOrder.Reset();

	if (topicId == 0)
	{
		// 0~10
		for (int32 i = 0; i <= 10; ++i)
		{
			QuestionOrder.Add(FString::FromInt(i));
		}
	}
	else if (topicId == 1)
	{
		// 11~24
		for (int32 i = 11; i <= 24; ++i)
		{
			QuestionOrder.Add(FString::FromInt(i));
		}
		
	}
	else if (topicId == 2) // 문제 단위(시퀀스 묶음)로 10문제 추출
	{
		TArray<TArray<FString>> Groups;
		int32 i = 0;

		while (i <= 24)
		{
			FName Key = *FString::FromInt(i);
			const FMotionRow* Row = MotionTable ? MotionTable->FindRow<FMotionRow>(Key, TEXT("BeginLesson")) : nullptr;

			int32 SeqCnt = 1;
			if (Row)
			{
				SeqCnt = FMath::Max(1, FCString::Atoi(*Row->seq_count.ToString()));
			}

			int32 Last = FMath::Min(i + SeqCnt - 1, 24);

			TArray<FString> Group;
			for (int32 k = i; k <= Last; ++k)
			{
				Group.Add(FString::FromInt(k));
			}
			Groups.Add(MoveTemp(Group));

			i = Last + 1;
		}

		// 랜덤 셔플
		for (int32 j = 0; j < Groups.Num(); ++j)
		{
			int32 SwapIdx = FMath::RandRange(j, Groups.Num() - 1);
			Groups.Swap(j, SwapIdx);
		}

		// 앞에서 10문제만 선택
		const int32 Take = FMath::Min(10, Groups.Num());
		for (int32 gi = 0; gi < Take; ++gi)
		{
			for (const FString& L : Groups[gi])
			{
				QuestionOrder.Add(L);
			}
		}
	}
	CurIdx = 0; // 현재 문제 번호 0번으로 초기화 해주기 
	SetGameState(GamePlayState::Playing);  // 플레이 스테이트로 변경
}

void ASignPlayerController::SelectTopic()
{
	SetGameState(GamePlayState::MainMenu);
	
}


// 모션 노출
void ASignPlayerController::PlayMotion()
{
	// 만약 그 주제의 마지막 문제면 주제 마지막으로 간다. 
	if (CurIdx >= QuestionOrder.Num())
	{
		SetGameState(GamePlayState::TopicComplete);
		return;
	}

	// 1) Row 조회
	if (!MotionTable) { return; }
	
	// 문제 진행
	CurLabel = QuestionOrder[CurIdx];     
	FName RowKey = FName(*CurLabel);         // DataTable의 RowName
	
	const FMotionRow* Row = MotionTable->FindRow<FMotionRow>(RowKey, TEXT("PlayOneSeqMotion"));
	if (!Row){return;}
	if (Row)
	{
		// 2) UI 단어 갱신 
		if (UProblemUI* PlayingUI = Cast<UProblemUI>(CurrentWidget))
		{
			PlayingUI->SetSignWord(FText::FromString(Row->sign_text));
		}
	}

	// 3) 몽타주 재생 (루프)
	if (MainChar)
	{
		// 애니메이션 재생
		MainChar->PlaySignAnimByKey(RowKey);
		//MainChar->PlaySignMontageByKey(RowKey);
	}

	// 4) 현재 문제 업데이트 (이 문제 가지고 서버에 판정 요청) 
	label = QuestionOrder[CurIdx]; 
}


// 함수 : 다음 문제로 넘어가기 (정답일 때 호출해줄 함수)
// 정답이면 다음 문제 (or 시퀀스로 넘기기)
// 오답이면 그 문제 다시 띄우기
void ASignPlayerController::JudgeNextStep(bool IsCorrect)
{
	SetGameState(GamePlayState::WaitingJudge); 
	// 서버 판정 결과를 가지고 이 함수를 호출해주면.
	label = TEXT("-1"); // 다시 초기화 해주기 (이미지 보내지 않게 하려고)
	
	// 몽타주 실행 (정답 몽타주, 오답 몽타주) // 제외함. 
	//MainChar->PlayResultMontage(IsCorrect);
	
	if (IsCorrect) // 정답이면 다음 문제로 넘어가기 
	{
		if (TrueSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), TrueSound);
		}
		 
		CurIdx++;
	}

	if (FalseSound){UGameplayStatics::PlaySound2D(GetWorld(), FalseSound); }
	SetGameState(GamePlayState::Playing); 
}




//////////////////////////////위젯//////////////////////////////////
void ASignPlayerController::ShowWidgetForState(GamePlayState State)
{
	if (CurrentState!=GamePlayState::MainMenu && PreviousState == CurrentState)
	{
		return;
	}
	
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	TSubclassOf<UUserWidget> WidgetClass = nullptr;

	switch (State)
	{
	case GamePlayState::MainMenu:
		WidgetClass = MainMenuClass;
		break;
	case GamePlayState::Playing:
		WidgetClass = PlayingClass;
		break;
	case GamePlayState::Feedback:
		WidgetClass = FeedbackClass;
		break;
	case GamePlayState::TopicComplete:
		WidgetClass = TopicCompleteClass;
		break;
	case GamePlayState::ResultMenu:
		WidgetClass = ResultClass;
		break;
	default:
		break;
	}

	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(0);
			
		}
	}
}
