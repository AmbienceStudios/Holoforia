// Copyright © 2021++ Wolf3D

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReadyPlayerMeWebLink.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FReadyPlayerWebBrowserResponse, const FString&, AvatarUrl);

UCLASS(Blueprintable, BlueprintType)
class READYPLAYERME_API UReadyPlayerMeWebLink : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ready Player Me|Web Link")
	void UrlGenerated(FString url);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Set Callback"), Category = "Ready Player Me|Web Link")
	void SetAvatarUrlCallback(FReadyPlayerWebBrowserResponse webBrowserCallback);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Last Avatar Url"), Category = "Ready Player Me|Web Link")
	FString GetLastAvatarUrl();

private:

	FReadyPlayerWebBrowserResponse WebBrowserResponse;
	FString lastAvatarUrl;
};
