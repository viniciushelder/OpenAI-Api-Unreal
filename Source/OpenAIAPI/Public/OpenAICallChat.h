// Copyright Kellan Mythen 2023. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OpenAIDefinitions.h"
#include "HttpModule.h"
#include "OpenAICallChat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResponseRecievedPin, const FChatCompletion, message, const FString&, errorMessage, bool, Success);
/**
 * 
 */
UCLASS()
class OPENAIAPI_API UOpenAICallChat : public UBlueprintAsyncActionBase
{
public:
	GENERATED_BODY()

public:
	UOpenAICallChat();
	~UOpenAICallChat();

	FChatSettings chatSettings;

	UPROPERTY(BlueprintAssignable, Category = "OpenAI")
	FOnResponseRecievedPin Finished;

private:
	UPROPERTY(meta = (BlueprintReadWrite = false))
	FString customModelOverride;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Tooltip = "This calls the chat completion api from OpenAI - https://api.openai.com/v1/chat/completions"), Category = "OpenAI")
		static UOpenAICallChat* OpenAICallChat(FChatSettings chatSettings, FString customModel);

	virtual void Activate() override;
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
	
};