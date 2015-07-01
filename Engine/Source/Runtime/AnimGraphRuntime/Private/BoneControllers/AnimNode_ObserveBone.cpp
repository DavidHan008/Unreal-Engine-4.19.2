// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AnimGraphRuntimePrivatePCH.h"
#include "BoneControllers/AnimNode_ObserveBone.h"
#include "AnimationRuntime.h"

/////////////////////////////////////////////////////
// FAnimNode_ObserveBone

FAnimNode_ObserveBone::FAnimNode_ObserveBone()
	: DisplaySpace(BCS_ComponentSpace)
	, Translation(FVector::ZeroVector)
	, Rotation(FRotator::ZeroRotator)
	, Scale(FVector(1.0f))
{
}

void FAnimNode_ObserveBone::GatherDebugData(FNodeDebugData& DebugData)
{
	const FString DebugLine = FString::Printf(TEXT("(Bone: %s has T(%s), R(%s), S(%s))"), *BoneToObserve.BoneName.ToString(), *Translation.ToString(), *Rotation.Euler().ToString(), *Scale.ToString());

	DebugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(DebugData);
}

void FAnimNode_ObserveBone::EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms)
{
	const FBoneContainer BoneContainer = MeshBases.GetPose().GetBoneContainer();

	const FCompactPoseBoneIndex BoneIndex = BoneToObserve.GetCompactPoseIndex(BoneContainer);
	FTransform BoneTM = MeshBases.GetComponentSpaceTransform(BoneIndex);
	
	// Convert to the specific display space if necessary
	FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp, MeshBases, BoneTM, BoneIndex, DisplaySpace);

	Translation = BoneTM.GetTranslation();
	Rotation = BoneTM.GetRotation().Rotator();
	Scale = BoneTM.GetScale3D();
}

bool FAnimNode_ObserveBone::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return (BoneToObserve.IsValid(RequiredBones));
}

void FAnimNode_ObserveBone::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	BoneToObserve.Initialize(RequiredBones);
}