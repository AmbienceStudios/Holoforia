// Copyright © 2021++ Wolf3D

#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "ReadyPlayerMeWebLink.h"
#include "WebBrowser.h"
#include "SWebBrowser.h"
#include "ReadyPlayerMeWebBrowser.generated.h"

UCLASS()
class READYPLAYERME_API UReadyPlayerMeWebBrowser : public UWebBrowser
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPB Setup Browser"), Category = "Ready Player Me|Web Browser")
		void SetupBrowser(FReadyPlayerWebBrowserResponse response);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPB Bind Browser To Object"), Category = "Ready Player Me|Web Browser")
		UReadyPlayerMeWebLink* BindBrowserToObject();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Add Event Listener"), Category = "Ready Player Me|Web Browser")
	void AddEventListener();

private:
	UReadyPlayerMeWebLink* webLinkObject;

	const FString linkObjectName = TEXT("linkobject");
	const FString eventListenerSnippet = TEXT("window.addEventListener('message', function(event){ document.querySelector('.content').remove(); setTimeout(() => {window.ue.linkobject.urlgenerated(event.data);}, 1000) }, false);");
};
