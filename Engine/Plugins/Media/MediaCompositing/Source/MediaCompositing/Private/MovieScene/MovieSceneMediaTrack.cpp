// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MovieSceneMediaTrack.h"

#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MovieScene.h"
#include "MovieSceneMediaSection.h"
#include "MovieSceneMediaTemplate.h"
#include "UObject/Package.h"
#include "UObject/UObjectGlobals.h"


#define LOCTEXT_NAMESPACE "MovieSceneMediaTrack"


/* UMovieSceneMediaTrack interface
 *****************************************************************************/

UMovieSceneMediaTrack::UMovieSceneMediaTrack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EvalOptions.bCanEvaluateNearestSection = false;
	EvalOptions.bEvalNearestSection = false;
	EvalOptions.bEvaluateInPreroll = true;
	EvalOptions.bEvaluateInPostroll = true;

#if WITH_EDITORONLY_DATA
	TrackTint = FColor(0, 0, 0, 200);
#endif
}


/* UMovieSceneMediaTrack interface
 *****************************************************************************/

UMovieSceneSection* UMovieSceneMediaTrack::AddNewMediaSourceOnRow(UMediaSource& MediaSource, FFrameNumber Time, int32 RowIndex)
{
	const float DefaultMediaSectionDuration = 1.0f;
	FFrameRate FrameRate = GetTypedOuter<UMovieScene>()->GetFrameResolution();

	FFrameTime DurationToUse = DefaultMediaSectionDuration * FrameRate;

	// add the section
	UMovieSceneMediaSection* NewSection = NewObject<UMovieSceneMediaSection>(this);

	NewSection->InitialPlacementOnRow(MediaSections, Time, DurationToUse.FrameNumber.Value, RowIndex);
	NewSection->SetMediaSource(&MediaSource);

	MediaSections.Add(NewSection);

	return NewSection;
}


/* UMovieScenePropertyTrack interface
 *****************************************************************************/

void UMovieSceneMediaTrack::AddSection(UMovieSceneSection& Section)
{
	MediaSections.Add(&Section);
}


UMovieSceneSection* UMovieSceneMediaTrack::CreateNewSection()
{
	return NewObject<UMovieSceneMediaSection>(this, NAME_None, RF_Transactional);
}


const TArray<UMovieSceneSection*>& UMovieSceneMediaTrack::GetAllSections() const
{
	return MediaSections;
}


bool UMovieSceneMediaTrack::HasSection(const UMovieSceneSection& Section) const
{
	return MediaSections.Contains(&Section);
}


bool UMovieSceneMediaTrack::IsEmpty() const
{
	return MediaSections.Num() == 0;
}


void UMovieSceneMediaTrack::RemoveSection(UMovieSceneSection& Section)
{
	MediaSections.Remove(&Section);
}


FMovieSceneEvalTemplatePtr UMovieSceneMediaTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FMovieSceneMediaSectionTemplate(*CastChecked<const UMovieSceneMediaSection>(&InSection), *this);
}


#undef LOCTEXT_NAMESPACE
