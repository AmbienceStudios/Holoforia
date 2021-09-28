// Copyright © 2021++ Wolf3D

#include "ReadyPlayerMeWebLink.h"


void UReadyPlayerMeWebLink::UrlGenerated(FString url) {
	UReadyPlayerMeWebLink::WebBrowserResponse.Execute(url);
	lastAvatarUrl = url;
}

void UReadyPlayerMeWebLink::SetAvatarUrlCallback(FReadyPlayerWebBrowserResponse webBrowserCallback)
{
	UReadyPlayerMeWebLink::WebBrowserResponse = webBrowserCallback;
}

FString UReadyPlayerMeWebLink::GetLastAvatarUrl()
{
	return UReadyPlayerMeWebLink::lastAvatarUrl;
}
