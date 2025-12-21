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

	/**
	 * Converts the transformation accuracy to compact formatted text.
	 * Format: "Acc: (Xm, Ym) [G/D]" where G=Grid-based, D=Direct
	 * Unknown accuracy values (-1.0) are displayed as "?"
	 * @param HAccuracyMeters Optional override for horizontal accuracy (0 uses struct member)
	 * @param VAccuracyMeters Optional override for vertical accuracy (0 uses struct member)  
	 * @param IsGridBased Optional override for grid-based flag
	 * @return Formatted text suitable for compact UI display
	 */
	FText ToCompactText(double HAccuracyMeters = 0, double VAccuracyMeters = 0, bool IsGridBased = false);
	
	/**
	 * Converts the transformation accuracy to full formatted text.
	 * Format: "Horizontal Accuracy: Xm, Vertical Accuracy: Ym, Grid-Based: Yes/No"
	 * Unknown accuracy values (-1.0) are displayed as "Unknown"
	 * @param HAccuracyMeters Optional override for horizontal accuracy (0 uses struct member)
	 * @param VAccuracyMeters Optional override for vertical accuracy (0 uses struct member)
	 * @param IsGridBased Optional override for grid-based flag
	 * @return Formatted text with full labels
	 */
	FText ToFullText(double HAccuracyMeters = 0, double VAccuracyMeters = 0, bool IsGridBased = false);

};

UCLASS()
class GEOREFERENCING_API UTransformationAccuracyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	/**
	 * Converts a TransformationAccuracy value to localized formatted text, displaying horizontal accuracy, 
	 * vertical accuracy, and whether the transformation is grid-based.
	 * Format: "Horizontal Accuracy: Xm, Vertical Accuracy: Ym, Grid-Based: Yes/No"
	 * @param TransformationAccuracy The accuracy information to format
	 * @param HorizontalAccuracyMeters Optional override for horizontal accuracy (-1.0 to use struct value)
	 * @param VerticalAccuracyMeters Optional override for vertical accuracy (-1.0 to use struct value)
	 * @param bIsGridBased Optional override for grid-based flag
	 **/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToFullText", AdvancedDisplay = "1", BlueprintAutocast), Category = "GeoReferencing")
	static inline FText ToFullText(UPARAM(ref) FTransformationAccuracy& TransformationAccuracy, double HorizontalAccuracyMeters = -1.0, double VerticalAccuracyMeters = -1.0, bool bIsGridBased = false)
	{
		return TransformationAccuracy.ToFullText(HorizontalAccuracyMeters, VerticalAccuracyMeters, bIsGridBased);
	}

	/**
	 * Converts a TransformationAccuracy value to compact formatted text, suitable for UI display.
	 * Format: "Acc: (Xm, Ym) [G/D]" where G=Grid-based, D=Direct
	 * @param TransformationAccuracy The accuracy information to format
	 * @param HorizontalAccuracyMeters Optional override for horizontal accuracy (-1.0 to use struct value)
	 * @param VerticalAccuracyMeters Optional override for vertical accuracy (-1.0 to use struct value)
	 * @param bIsGridBased Optional override for grid-based flag
	 **/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToCompactText", AdvancedDisplay = "1", BlueprintAutocast), Category = "GeoReferencing")
	static inline FText ToCompactText(UPARAM(ref) FTransformationAccuracy& TransformationAccuracy, double HorizontalAccuracyMeters = -1.0, double VerticalAccuracyMeters = -1.0, bool bIsGridBased = false)
	{
		return TransformationAccuracy.ToCompactText(HorizontalAccuracyMeters, VerticalAccuracyMeters, bIsGridBased);
	}
};

