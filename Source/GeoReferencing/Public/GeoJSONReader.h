// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GeographicCoordinates.h"
#include "GeoJSONReader.generated.h"

/**
 * Utility class for reading GeoJSON files (RFC 7946)
 * Supports Point, LineString, and Polygon geometries
 */
UCLASS()
class GEOREFERENCING_API UGeoJSONReader : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Load a GeoJSON file and extract point coordinates
	 * @param FilePath Path to the GeoJSON file
	 * @param OutPoints Array of geographic coordinates from the GeoJSON
	 * @param OutProperties Array of property JSON strings for each feature
	 * @return True if successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
	static bool LoadGeoJSONFile(
		const FString& FilePath,
		TArray<FGeographicCoordinates>& OutPoints,
		TArray<FString>& OutProperties);

	/**
	 * Parse a GeoJSON string and extract point coordinates
	 * @param GeoJSONContent The GeoJSON content as a string
	 * @param OutPoints Array of geographic coordinates from the GeoJSON
	 * @param OutProperties Array of property JSON strings for each feature
	 * @return True if successful, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
	static bool LoadGeoJSONString(
		const FString& GeoJSONContent,
		TArray<FGeographicCoordinates>& OutPoints,
		TArray<FString>& OutProperties);

private:
	/**
	 * Parse coordinates from a GeoJSON geometry object
	 * @param GeometryObject The JSON geometry object
	 * @param OutPoints Array to append extracted coordinates
	 * @return True if successful
	 */
	static bool ParseGeometry(
		const TSharedPtr<FJsonObject>& GeometryObject,
		TArray<FGeographicCoordinates>& OutPoints);

	/**
	 * Extract coordinates from a coordinate array
	 * @param CoordinateArray JSON array containing coordinate values
	 * @return Geographic coordinate
	 */
	static FGeographicCoordinates ExtractCoordinate(const TArray<TSharedPtr<FJsonValue>>& CoordinateArray);
};
