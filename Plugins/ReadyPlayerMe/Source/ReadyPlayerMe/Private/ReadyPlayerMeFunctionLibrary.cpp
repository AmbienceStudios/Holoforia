// Copyright © 2021++ Wolf3D


#include "ReadyPlayerMeFunctionLibrary.h"
#include "glTFRuntimeParser.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/UObjectGlobals.h"
//required for Android builds
#include "Components/SkeletalMeshComponent.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const FString UReadyPlayerMeFunctionLibrary::SHORTCODE_URL_PREFIX = "https://readyplayer.me/api/avatar/";
const FString UReadyPlayerMeFunctionLibrary::URL_PATTERN = "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,4}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)";
const FString UReadyPlayerMeFunctionLibrary::FULLBODY_BONE_NODE = "Armature";
const FString UReadyPlayerMeFunctionLibrary::HALFBODY_BONE_NODE = "AvatarRoot";
const float UReadyPlayerMeFunctionLibrary::LEGACY_BOUNDS_SCALE = 100.0f;
const FString UReadyPlayerMeFunctionLibrary::GENDER_FEMININE = "Feminine";
const FString UReadyPlayerMeFunctionLibrary::GENDER_MASCULINE = "Masculine";
const FString UReadyPlayerMeFunctionLibrary::TYPE_FULLBODY = "Fullbody";
const FString UReadyPlayerMeFunctionLibrary::TYPE_HALFBODY = "Halfbody";
const FName UReadyPlayerMeFunctionLibrary::SKELETAL_MESH_NAME = "Skeletal Mesh";
const TCHAR * UReadyPlayerMeFunctionLibrary::SUFFIX_GLB = TEXT(".glb");
const TCHAR * UReadyPlayerMeFunctionLibrary::SUFFIX_JSON = TEXT(".json");
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_OUTFIT = "outfitVersion";
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_BODY = "bodyType";
const FString UReadyPlayerMeFunctionLibrary::JSON_FIELD_GENDER = "outfitGender";

USkeletalMeshComponent* UReadyPlayerMeFunctionLibrary::SetupSkeleton(AActor* target, USkeletalMesh* skeletalMesh)
{
	USkeletalMeshComponent* skeletalMeshComponent = NewObject<USkeletalMeshComponent>(target, SKELETAL_MESH_NAME);
	skeletalMeshComponent->SetupAttachment(target->GetRootComponent());
	skeletalMeshComponent->RegisterComponent();
	skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);

	return skeletalMeshComponent;
}

void UReadyPlayerMeFunctionLibrary::SetAnimationBlueprint(USkeletalMeshComponent* skeletalMeshComponent, UClass* animInstance)
{
	skeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	skeletalMeshComponent->SetAnimInstanceClass(animInstance);
}

FString UReadyPlayerMeFunctionLibrary::GetUrlFromShortcode(const FString& shortcode) 
{
	return SHORTCODE_URL_PREFIX + shortcode;
}

bool UReadyPlayerMeFunctionLibrary::IsUrl(const FString& url)
{
	FRegexPattern regexPattern = URL_PATTERN;
	FRegexMatcher regexMatcher = FRegexMatcher(regexPattern, url);
	return regexMatcher.FindNext();
}

FglTFRuntimeSkeletonConfig UReadyPlayerMeFunctionLibrary::GetRPMSkeletonConfig(const FString & rootBoneName, USkeleton* skeleton)
{
	FglTFRuntimeSkeletonConfig skeletonConfig;
	skeletonConfig.CopyRotationsFrom = skeleton;
	skeletonConfig.bAddRootBone = true;
	skeletonConfig.RootBoneName = rootBoneName;
	return skeletonConfig;
}

FglTFRuntimeMaterialsConfig UReadyPlayerMeFunctionLibrary::GetRPMMaterialsConfig()
{
	FglTFRuntimeMaterialsConfig materialsConfig;
	materialsConfig.bMergeSectionsByMaterial = true;
	return materialsConfig;
}

FglTFRuntimeSkeletalMeshConfig UReadyPlayerMeFunctionLibrary::GetRPMSkeletalMeshConfig(const FAvatarMetadata & avatarMetaData, USkeleton* skeleton)
{
	FglTFRuntimeSkeletalMeshConfig skeletonMeshConfig;
	skeletonMeshConfig.SkeletonConfig = GetRPMSkeletonConfig(GetRootBoneName(avatarMetaData.bodyType), skeleton);

	if(avatarMetaData.bodyType == AvatarBodyType::Fullbody)
	{
		skeletonMeshConfig.Skeleton = skeleton;
		skeletonMeshConfig.MaterialsConfig = GetRPMMaterialsConfig();
	}

	return skeletonMeshConfig;
}

FglTFRuntimeConfig UReadyPlayerMeFunctionLibrary::GetRPMRuntimeConfig()
{
	FglTFRuntimeConfig runtimeConfig;
	runtimeConfig.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
	return runtimeConfig;
}

AvatarBodyType UReadyPlayerMeFunctionLibrary::GetLegacyBodyType(const UglTFRuntimeAsset* gltfRuntimeAsset)
{
	
	if (const_cast<UglTFRuntimeAsset*>(gltfRuntimeAsset)->GetNodes().Num() > MAX_HALFBODY_NODES)
	{
		return AvatarBodyType::Fullbody;
	}

	return AvatarBodyType::Halfbody;
}

FString UReadyPlayerMeFunctionLibrary::GetRootBoneName(const AvatarBodyType & avatarBodyType)
{
	if (avatarBodyType == AvatarBodyType::Fullbody)
	{
		return FULLBODY_BONE_NODE;
	}

	return HALFBODY_BONE_NODE;
}

void UReadyPlayerMeFunctionLibrary::LoadAvatarMetaData(const FString & url, const FReadyPlayerMeHttpResponse& completed)
{
	if (url.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ready Player Me Avatar Url is empty!"));
		return;
	}
	const FString newUrl = url.Replace(SUFFIX_GLB,SUFFIX_JSON);

#if ENGINE_MINOR_VERSION > 25
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif
	httpRequest->SetURL(newUrl);

	httpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr requestPtr, FHttpResponsePtr responsePtr, bool bSuccess, FReadyPlayerMeHttpResponse Completed)
		{
			FAvatarMetadata metaData = FAvatarMetadata();
			if (bSuccess)
			{
				metaData = OnResponseReceived(responsePtr->GetContentAsString());
			}
			Completed.ExecuteIfBound(metaData);
		}, completed);

	httpRequest->ProcessRequest();
}


FAvatarMetadata UReadyPlayerMeFunctionLibrary::GetAvatarMetaDataLegacy(UglTFRuntimeAsset* gltfRuntimeAsset)
{
	FAvatarMetadata metaData = FAvatarMetadata();
	metaData.bodyType = GetLegacyBodyType(gltfRuntimeAsset);
	return metaData;
}

FAvatarMetadata UReadyPlayerMeFunctionLibrary::OnResponseReceived(const FString & jsonString)
{
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);

	FAvatarMetadata metaData = FAvatarMetadata();

	if (FJsonSerializer::Deserialize(Reader, jsonObject))
	{
		if (jsonObject->HasField(JSON_FIELD_BODY)) {
			int32 recievedInt = jsonObject->GetIntegerField(JSON_FIELD_OUTFIT);
			metaData.outfitVersion = recievedInt;
		}
		if (jsonObject->HasField(JSON_FIELD_BODY) && jsonObject->GetStringField(JSON_FIELD_BODY).Contains(TYPE_HALFBODY.ToLower()))
		{
			metaData.bodyType = AvatarBodyType::Halfbody;
		}
		if (jsonObject->HasField(JSON_FIELD_GENDER) && jsonObject->GetStringField(JSON_FIELD_GENDER).Contains(GENDER_FEMININE.ToLower()))
		{
			metaData.outfitGender = AvatarGender::Feminine;
		}
	}

	return metaData;
}

void UReadyPlayerMeFunctionLibrary::SetMorphTargets(USkeletalMeshComponent* skeletalMeshComponent, const TMap<FString, float>& morphTargetMap)
{
	for (const TPair<FString, float>& morphTargetPair : morphTargetMap)
	{
		SetMorphTarget(skeletalMeshComponent, morphTargetPair.Key, morphTargetPair.Value);
	}
}

void UReadyPlayerMeFunctionLibrary::SetMorphTarget(USkeletalMeshComponent* skeletalMeshComponent,const FString & name, float value)
{
	FName morphTargetName = FName(name);
	UMorphTarget* morphTarget = skeletalMeshComponent->FindMorphTarget(morphTargetName);
	if (morphTarget != nullptr)
	{
		skeletalMeshComponent->SetMorphTarget(morphTargetName, value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Morph target %s not found!!"), *name);
	}
}
