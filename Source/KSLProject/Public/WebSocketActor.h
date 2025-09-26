// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWebSocket.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetRenderer.h"
#include "WebSocketActor.generated.h"

USTRUCT(BlueprintType)
struct FServerResponseData
{
	GENERATED_BODY()

	// 블루프린트에서 읽기 전용으로 노출시킵니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WebSocket Response")
	bool bIsSuccess = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WebSocket Response")
	FString SignId;
};

class UMediaPlayer;

UCLASS()
class KSLPROJECT_API AWebSocketActor : public AActor
{
	GENERATED_BODY()

public:
	AWebSocketActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WebSocket")
	FServerResponseData LastServerResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WebSocket")
	bool bShouldSendFrames = false;

	// 블루프린트에서 호출할 수 있는 함수를 선언합니다.
	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	void SetFrameSending(bool bShouldSend);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category="WebSocket")
	FString ServerURL = TEXT("ws://172.16.20.85:8000/ws/test");

	UPROPERTY(EditAnywhere, Category="WebSocket")
	FString Protocol = TEXT("");

	UPROPERTY(EditAnywhere, Category="Capture")
	TSubclassOf<UUserWidget> TargetWidgetClass;

	UPROPERTY(Transient)
	UUserWidget* TargetWidgetInstance = nullptr;
	
	// 에디터에서 할당할 미디어 플레이어 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture Settings")
	UMediaPlayer* MediaPlayer;

	UPROPERTY(Transient)
	UTextureRenderTarget2D* RenderTarget = nullptr;
	UPROPERTY(EditAnywhere)
	FString camurl;

	FTimerHandle FrameSendTimerHandle;

	void SendFrameData();
	bool CaptureWidgetToRenderTarget();

private:
	void InitializeWebSocket();
	void BindEvents();
	void CloseWebSocket();

	void SendTestData();
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessageReceived(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void StartSendingFrames();
	void StopSendingFrames();

	TSharedPtr<IWebSocket> WebSocket;

	UPROPERTY(EditAnywhere, Category = "Capture Settings")
	int32 CaptureWidth = 240;

	UPROPERTY(EditAnywhere, Category = "Capture Settings")
	int32 CaptureHeight = 290;

private:
	TSharedPtr<FWidgetRenderer> WidgetRenderer;

	UPROPERTY(meta = (BindWidget))
	UWidget* ClippingAreaWidget;
};
