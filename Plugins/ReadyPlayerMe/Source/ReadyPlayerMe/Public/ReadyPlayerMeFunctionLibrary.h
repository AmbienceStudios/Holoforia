// Copyright (c) 2021 Wolf3D

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "glTFRuntimeAsset.h"
#include "HttpModule.h"
#include "Internationalization/Regex.h"
#include "Templates/SharedPointer.h"
#include "ReadyPlayerMeFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum AvatarGender
{
	Masculine UMETA(DisplayName = GENDER_MASCULINE),
	Feminine UMETA(DisplayName = GENDER_FEMININE)
};

UENUM(BlueprintType)
enum AvatarBodyType
{
	Fullbody UMETA(DisplayName = TYPE_FULLBODY),
	Halfbody UMETA(DisplayName = TYPE_HALFBODY)
};


USTRUCT(BlueprintType)
struct FAvatarMetadata
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		TEnumAsByte<AvatarBodyType> bodyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		TEnumAsByte<AvatarGender> outfitGender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadyPlayerMe")
		int32 outfitVersion;

	FAvatarMetadata()
	{
		bodyType = AvatarBodyType::Fullbody;
		outfitGender = AvatarGender::Masculine;
		outfitVersion = 1;
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FReadyPlayerMeHttpResponse, const FAvatarMetadata&, AvataMetaData);

UCLASS()
class READYPLAYERME_API UReadyPlayerMeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

		UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target", DisplayName = "RPM Setup Skeleton"), Category = "Ready Player Me")
		static USkeletalMeshComponent* SetupSkeleton(AActor* target, USkeletalMesh* skeletalMesh);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Set Full Body Animation Blueprint"), Category = "Ready Player Me")
		static void SetAnimationBlueprint(USkeletalMeshComponent* skeletalMeshComponent, UClass* animInstance);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Url From Shortcode"), Category = "Ready Player Me")
		static FString GetUrlFromShortcode(const FString& shortcode);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Check Is Url"), Category = "Ready Player Me")
		static bool IsUrl(const FString& url);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Skeleton Config"), Category = "Ready Player Me")
		static FglTFRuntimeSkeletonConfig GetRPMSkeletonConfig(const FString& rootBoneName, USkeleton* skeleton);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Materials Config"), Category = "Ready Player Me")
		static FglTFRuntimeMaterialsConfig GetRPMMaterialsConfig();

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Skeletal Mesh Config"), Category = "Ready Player Me")
		static FglTFRuntimeSkeletalMeshConfig GetRPMSkeletalMeshConfig(const FAvatarMetadata& avatarMetaData, USkeleton* skeleton = nullptr);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Runtime Config"), Category = "Ready Player Me")
		static FglTFRuntimeConfig GetRPMRuntimeConfig();

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Body Type (Legacy)"), Category = "Ready Player Me")
		static AvatarBodyType GetLegacyBodyType(const UglTFRuntimeAsset* glTFRuntimeAsset);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Root Bone Name"), Category = "Ready Player Me")
		static FString GetRootBoneName(const AvatarBodyType& avatarBodyType);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Load Avatar Meta Data"), Category = "Ready Player Me")
		static void LoadAvatarMetaData(const FString& url, const FReadyPlayerMeHttpResponse& completed);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Get Avatar Meta Data (Legacy)"), Category = "Ready Player Me")
		static FAvatarMetadata GetAvatarMetaDataLegacy(UglTFRuntimeAsset* gltfRuntimeAsset);

		static FAvatarMetadata OnResponseReceived(const FString& jsonString);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Set Avatar Morph targets"), Category = CATEGORY)
		static void SetMorphTargets(USkeletalMeshComponent* skeletalMeshComponent, const TMap<FString, float>& morphTargetMap);

		UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPM Set Avatar Morph target"), Category = CATEGORY)
		static void SetMorphTarget(USkeletalMeshComponent* skeletalMeshComponent, const FString & name, float value);

		static const FString SHORTCODE_URL_PREFIX;
		static const FString URL_PATTERN;
		static const int MAX_HALFBODY_NODES=60;
		static const FString FULLBODY_BONE_NODE;
		static const FString HALFBODY_BONE_NODE;
		static const float LEGACY_BOUNDS_SCALE;
		static const int SCENE_SCALE=1;
		static const FString GENDER_FEMININE;
		static const FString GENDER_MASCULINE;
		static const FString TYPE_HALFBODY;
		static const FString TYPE_FULLBODY;
		static const FName SKELETAL_MESH_NAME;
		static const TCHAR * SUFFIX_GLB;
		static const TCHAR * SUFFIX_JSON;
		static const FString JSON_FIELD_OUTFIT;
		static const FString JSON_FIELD_BODY;
		static const FString JSON_FIELD_GENDER;
};
