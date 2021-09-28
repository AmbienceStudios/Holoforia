// Copyright © 2021++ Wolf3D


#include "ReadyPlayerMeWebBrowser.h"

void UReadyPlayerMeWebBrowser::SetupBrowser(FReadyPlayerWebBrowserResponse webBrowserCallback)
{
	UReadyPlayerMeWebBrowser::BindBrowserToObject();
	webLinkObject->SetAvatarUrlCallback(webBrowserCallback);
	UReadyPlayerMeWebBrowser::AddEventListener();
}

UReadyPlayerMeWebLink* UReadyPlayerMeWebBrowser::BindBrowserToObject()
{
	if (!UReadyPlayerMeWebBrowser::webLinkObject) {
		UReadyPlayerMeWebBrowser::webLinkObject = NewObject<UReadyPlayerMeWebLink>(UReadyPlayerMeWebLink::StaticClass());
		webLinkObject->Rename(*UReadyPlayerMeWebBrowser::linkObjectName);
	}
	WebBrowserWidget->BindUObject(UReadyPlayerMeWebBrowser::linkObjectName, webLinkObject, true);
	return webLinkObject;
}

void UReadyPlayerMeWebBrowser::AddEventListener()
{
	this->ExecuteJavascript(UReadyPlayerMeWebBrowser::eventListenerSnippet);
}
