// Fill out your copyright notice in the Description page of Project Settings.


#include "TransformationAccuracy.h"
#include "MathUtil.h"

FText FTransformationAccuracy::ToFullText(double HAccuracyMeters, double VAccuracyMeters, bool IsGridBased)
{
	FNumberFormattingOptions NumberFormatOptions = FNumberFormattingOptions::DefaultNoGrouping();
	FFormatNamedArguments Args;

	// Use struct members if parameters have default values, otherwise use parameters
	double HorizAccuracy = (HAccuracyMeters == 0) ? HorizontalAccuracyMeters : HAccuracyMeters;
	double VertAccuracy = (VAccuracyMeters == 0) ? VerticalAccuracyMeters : VAccuracyMeters;
	bool GridBased = (HAccuracyMeters == 0 && VAccuracyMeters == 0) ? bIsGridBased : IsGridBased;

	// Set decimal precision for accuracy values
	NumberFormatOptions.MinimumFractionalDigits = 2;
	NumberFormatOptions.MaximumFractionalDigits = 3;
	
	// Format accuracy values
	if (HorizAccuracy < 0)
	{
		Args.Add(TEXT("HAccuracyMeters"), NSLOCTEXT("GeoReferencing", "UnknownAccuracy", "Unknown"));
	}
	else
	{
		Args.Add(TEXT("HAccuracyMeters"), FText::Format(NSLOCTEXT("GeoReferencing", "AccuracyMeters", "{0}m"), FText::AsNumber(HorizAccuracy, &NumberFormatOptions)));
	}
	
	if (VertAccuracy < 0)
	{
		Args.Add(TEXT("VAccuracyMeters"), NSLOCTEXT("GeoReferencing", "UnknownAccuracy", "Unknown"));
	}
	else
	{
		Args.Add(TEXT("VAccuracyMeters"), FText::Format(NSLOCTEXT("GeoReferencing", "AccuracyMeters", "{0}m"), FText::AsNumber(VertAccuracy, &NumberFormatOptions)));
	}
	
	// Format grid-based flag as text
	Args.Add(TEXT("IsGridBased"), GridBased ? NSLOCTEXT("GeoReferencing", "GridBased", "Yes") : NSLOCTEXT("GeoReferencing", "NotGridBased", "No"));

	return FText::Format(NSLOCTEXT("GeoReferencing", "TransformationAccuracyFullText", 
		"Horizontal Accuracy: {HAccuracyMeters}, Vertical Accuracy: {VAccuracyMeters}, Grid-Based: {IsGridBased}"), Args);
}

FText FTransformationAccuracy::ToCompactText(double HAccuracyMeters, double VAccuracyMeters, bool IsGridBased)
{
	FNumberFormattingOptions NumberFormatOptions = FNumberFormattingOptions::DefaultNoGrouping();
	FFormatNamedArguments Args;

	// Use struct members if parameters have default values, otherwise use parameters
	double HorizAccuracy = (HAccuracyMeters == 0) ? HorizontalAccuracyMeters : HAccuracyMeters;
	double VertAccuracy = (VAccuracyMeters == 0) ? VerticalAccuracyMeters : VAccuracyMeters;
	bool GridBased = (HAccuracyMeters == 0 && VAccuracyMeters == 0) ? bIsGridBased : IsGridBased;

	// Set decimal precision for accuracy values
	NumberFormatOptions.MinimumFractionalDigits = 2;
	NumberFormatOptions.MaximumFractionalDigits = 3;
	
	// Format accuracy values
	if (HorizAccuracy < 0)
	{
		Args.Add(TEXT("HAccuracyMeters"), NSLOCTEXT("GeoReferencing", "UnknownCompact", "?"));
	}
	else
	{
		Args.Add(TEXT("HAccuracyMeters"), FText::AsNumber(HorizAccuracy, &NumberFormatOptions));
	}
	
	if (VertAccuracy < 0)
	{
		Args.Add(TEXT("VAccuracyMeters"), NSLOCTEXT("GeoReferencing", "UnknownCompact", "?"));
	}
	else
	{
		Args.Add(TEXT("VAccuracyMeters"), FText::AsNumber(VertAccuracy, &NumberFormatOptions));
	}
	
	// Format grid-based flag as text
	Args.Add(TEXT("GridBasedIndicator"), GridBased ? NSLOCTEXT("GeoReferencing", "GridBasedCompact", "G") : NSLOCTEXT("GeoReferencing", "DirectCompact", "D"));

	return FText::Format(NSLOCTEXT("GeoReferencing", "TransformationAccuracyCompactText", "Acc: ({HAccuracyMeters}m, {VAccuracyMeters}m) [{GridBasedIndicator}]"), Args);
}