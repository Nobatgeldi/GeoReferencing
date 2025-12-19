// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GeographicCoordinates.h"
#include "GeoJSONWriter.generated.h"

/**
 * Utility class for writing GeoJSON files (RFC 7946)
 * Supports Point FeatureCollection export
 */
UCLASS()
class GEOREFERENCING_API UGeoJSONWriter : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Save geographic coordinates to a GeoJSON file
	 * @param FilePath Path where the GeoJSON file will be saved
	 * @param Points Array of geographic coordinates to export
	 * @param Properties Array of property JSON strings for each point (optional)
	 * @return True if successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
	static bool SaveGeoJSONFile(
		const FString& FilePath,
		const TArray<FGeographicCoordinates>& Points,
		const TArray<FString>& Properties);

	/**
	 * Export geographic coordinates to a GeoJSON string
	 * @param Points Array of geographic coordinates to export
	 * @param Properties Array of property JSON strings for each point (optional)
	 * @return GeoJSON string representation
	 */
	UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
	static FString ExportToGeoJSONString(
		const TArray<FGeographicCoordinates>& Points,
		const TArray<FString>& Properties);
};
