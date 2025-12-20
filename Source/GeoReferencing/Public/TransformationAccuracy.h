// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TransformationAccuracy.generated.h"

/**
 * @class UTransformationAccuracy
 * @brief A utility class providing blueprint-accessible functions for evaluating transformation accuracy.
 *
 * This class extends UBlueprintFunctionLibrary and serves as a blueprint-accessible library
 * for functions related to assessing the accuracy of georeferencing or spatial transformations.
 *
 * @note This class is part of the GIS-related functionalities provided by the GeoReferencing module.
 */
/**
 * Structure containing information about the accuracy of a coordinate transformation
 */
USTRUCT(BlueprintType)
struct GEOREFERENCING_API FTransformationAccuracy
{
	GENERATED_BODY()
public:
	/** Horizontal accuracy of the transformation in meters */
	UPROPERTY(BlueprintReadOnly, Category = "GeoReferencing")
	double HorizontalAccuracyMeters = -1.0;

	/** Vertical accuracy of the transformation in meters */
	UPROPERTY(BlueprintReadOnly, Category = "GeoReferencing")
	double VerticalAccuracyMeters = -1.0;

	/** Whether the transformation uses a grid-based method */
	UPROPERTY(BlueprintReadOnly, Category = "GeoReferencing")
	bool bIsGridBased = false;

	/** Description of the transformation method used */
	UPROPERTY(BlueprintReadOnly, Category = "GeoReferencing")
	FString TransformationMethod;
	
	FTransformationAccuracy()
		: HorizontalAccuracyMeters(-1.0)
		, VerticalAccuracyMeters(-1.0)
		, bIsGridBased(false)
	{
	}

	FText ToCompactText(double HAccuracyMeters = 0, double VAccuracyMeters = 0, bool IsGridBased = false);
	FText ToFullText(double HAccuracyMeters = 0, double VAccuracyMeters = 0, bool IsGridBased = false);

};

UCLASS()
class GEOREFERENCING_API UTransformationAccuracyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/**
	 * Converts a GeographicCoordinates value to localized formatted text, in the form 'X= Y= Z='
	 **/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFullText", AdvancedDisplay = "1", BlueprintAutocast), Category = "GeoReferencing")
	static inline FText ToFullText(UPARAM(ref) FTransformationAccuracy& TransformationAccuracy, double HorizontalAccuracyMeters = -1.0, double VerticalAccuracyMeters = -1.0, bool bIsGridBased = false)
	{
		return TransformationAccuracy.ToFullText(HorizontalAccuracyMeters, VerticalAccuracyMeters, bIsGridBased);
	}

	/**
	 * Converts a GeographicCoordinates value to formatted text, in the form '(X, Y, Z)'
	 **/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToCompactText", AdvancedDisplay = "1", BlueprintAutocast), Category = "GeoReferencing")
	static inline FText ToCompactText(UPARAM(ref) FTransformationAccuracy& TransformationAccuracy, double HorizontalAccuracyMeters = -1.0, double VerticalAccuracyMeters = -1.0, bool bIsGridBased = false)
	{
		return TransformationAccuracy.ToCompactText(HorizontalAccuracyMeters, VerticalAccuracyMeters, bIsGridBased);
	}
};

