// Fill out your copyright notice in the Description page of Project Settings.


#include "TransformationAccuracy.h"
#include "MathUtil.h"

FText FTransformationAccuracy::ToFullText(double HAccuracyMeters, double VAccuracyMeters, bool IsGridBased)
{
	FNumberFormattingOptions NumberFormatOptions = FNumberFormattingOptions::DefaultNoGrouping();
	FFormatNamedArguments Args;

	// HorizontalAccuracyMeters VerticalAccuracyMeters
	NumberFormatOptions.MinimumFractionalDigits = HAccuracyMeters;
	NumberFormatOptions.MaximumFractionalDigits = VAccuracyMeters;
	Args.Add(TEXT("HAccuracyMeters"), FText::AsNumber(HAccuracyMeters, &NumberFormatOptions));
	Args.Add(TEXT("VAccuracyMeters"), FText::AsNumber(VAccuracyMeters, &NumberFormatOptions));
	Args.Add(TEXT("IsGridBased"), FText::AsNumber(IsGridBased, &NumberFormatOptions));

	return FText::Format(NSLOCTEXT("GeoReferencing", "TransformationAccuracyFullText", 
		"HorizontalAccuracyMeters={HAccuracyMeters} VerticalAccuracyMeters={VAccuracyMeters} Is Grid Based={IsGridBased}m"), Args);
}

FText FTransformationAccuracy::ToCompactText(double HAccuracyMeters, double VAccuracyMeters, bool IsGridBased)
{
	FNumberFormattingOptions NumberFormatOptions = FNumberFormattingOptions::DefaultNoGrouping();
	FFormatNamedArguments Args;

	// HorizontalAccuracyMeters VerticalAccuracyMeters
	NumberFormatOptions.MinimumFractionalDigits = HAccuracyMeters;
	NumberFormatOptions.MaximumFractionalDigits = VAccuracyMeters;
	Args.Add(TEXT("HAccuracyMeters"), FText::AsNumber(HAccuracyMeters, &NumberFormatOptions));
	Args.Add(TEXT("VAccuracyMeters"), FText::AsNumber(VAccuracyMeters, &NumberFormatOptions));
	Args.Add(TEXT("IsGridBased"), FText::AsNumber(IsGridBased, &NumberFormatOptions));

	return FText::Format(NSLOCTEXT("GeoReferencing", "TransformationAccuracyCompactText", "({HAccuracyMeters}, {VAccuracyMeters})  {IsGridBased}m"), Args);
}