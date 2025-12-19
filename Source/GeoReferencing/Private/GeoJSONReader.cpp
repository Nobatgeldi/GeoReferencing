// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeoJSONReader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

bool UGeoJSONReader::LoadGeoJSONFile(
	const FString& FilePath,
	TArray<FGeographicCoordinates>& OutPoints,
	TArray<FString>& OutProperties)
{
	// Read file content
	FString FileContent;
	if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		UE_LOG(LogGeoReferencing, Error, TEXT("Failed to read GeoJSON file: %s"), *FilePath);
		return false;
	}

	return LoadGeoJSONString(FileContent, OutPoints, OutProperties);
}

bool UGeoJSONReader::LoadGeoJSONString(
	const FString& GeoJSONContent,
	TArray<FGeographicCoordinates>& OutPoints,
	TArray<FString>& OutProperties)
{
	// Parse JSON
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(GeoJSONContent);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogGeoReferencing, Error, TEXT("Failed to parse GeoJSON content"));
		return false;
	}

	// Clear output arrays
	OutPoints.Empty();
	OutProperties.Empty();

	// Check GeoJSON type
	FString Type = JsonObject->GetStringField(TEXT("type"));

	if (Type == TEXT("FeatureCollection"))
	{
		// Process feature collection
		const TArray<TSharedPtr<FJsonValue>>* FeaturesArray;
		if (!JsonObject->TryGetArrayField(TEXT("features"), FeaturesArray))
		{
			UE_LOG(LogGeoReferencing, Error, TEXT("FeatureCollection missing 'features' array"));
			return false;
		}

		for (const TSharedPtr<FJsonValue>& FeatureValue : *FeaturesArray)
		{
			TSharedPtr<FJsonObject> FeatureObject = FeatureValue->AsObject();
			if (!FeatureObject.IsValid())
			{
				continue;
			}

			// Extract geometry
			TSharedPtr<FJsonObject> GeometryObject = FeatureObject->GetObjectField(TEXT("geometry"));
			if (GeometryObject.IsValid())
			{
				ParseGeometry(GeometryObject, OutPoints);
			}

			// Extract properties as JSON string
			TSharedPtr<FJsonObject> PropertiesObject = FeatureObject->GetObjectField(TEXT("properties"));
			if (PropertiesObject.IsValid())
			{
				FString PropertiesString;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesString);
				FJsonSerializer::Serialize(PropertiesObject.ToSharedRef(), Writer);
				OutProperties.Add(PropertiesString);
			}
			else
			{
				OutProperties.Add(TEXT("{}"));
			}
		}
	}
	else if (Type == TEXT("Feature"))
	{
		// Process single feature
		TSharedPtr<FJsonObject> GeometryObject = JsonObject->GetObjectField(TEXT("geometry"));
		if (GeometryObject.IsValid())
		{
			ParseGeometry(GeometryObject, OutPoints);
		}

		// Extract properties
		TSharedPtr<FJsonObject> PropertiesObject = JsonObject->GetObjectField(TEXT("properties"));
		if (PropertiesObject.IsValid())
		{
			FString PropertiesString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesString);
			FJsonSerializer::Serialize(PropertiesObject.ToSharedRef(), Writer);
			OutProperties.Add(PropertiesString);
		}
		else
		{
			OutProperties.Add(TEXT("{}"));
		}
	}
	else
	{
		// Direct geometry object
		ParseGeometry(JsonObject, OutPoints);
		OutProperties.Add(TEXT("{}"));
	}

	return OutPoints.Num() > 0;
}

bool UGeoJSONReader::ParseGeometry(
	const TSharedPtr<FJsonObject>& GeometryObject,
	TArray<FGeographicCoordinates>& OutPoints)
{
	if (!GeometryObject.IsValid())
	{
		return false;
	}

	FString GeometryType = GeometryObject->GetStringField(TEXT("type"));
	const TArray<TSharedPtr<FJsonValue>>* CoordinatesArray;

	if (!GeometryObject->TryGetArrayField(TEXT("coordinates"), CoordinatesArray))
	{
		return false;
	}

	if (GeometryType == TEXT("Point"))
	{
		// Single coordinate
		TArray<TSharedPtr<FJsonValue>> CoordArray;
		for (const TSharedPtr<FJsonValue>& Value : *CoordinatesArray)
		{
			CoordArray.Add(Value);
		}
		OutPoints.Add(ExtractCoordinate(CoordArray));
	}
	else if (GeometryType == TEXT("LineString") || GeometryType == TEXT("MultiPoint"))
	{
		// Array of coordinates
		for (const TSharedPtr<FJsonValue>& CoordValue : *CoordinatesArray)
		{
			const TArray<TSharedPtr<FJsonValue>>* CoordArray = nullptr;
			if (CoordValue->TryGetArray(CoordArray))
			{
				OutPoints.Add(ExtractCoordinate(*CoordArray));
			}
		}
	}
	else if (GeometryType == TEXT("Polygon") || GeometryType == TEXT("MultiLineString"))
	{
		// Array of arrays of coordinates
		for (const TSharedPtr<FJsonValue>& RingValue : *CoordinatesArray)
		{
			const TArray<TSharedPtr<FJsonValue>>* RingArray = nullptr;
			if (RingValue->TryGetArray(RingArray))
			{
				for (const TSharedPtr<FJsonValue>& CoordValue : *RingArray)
				{
					const TArray<TSharedPtr<FJsonValue>>* CoordArray = nullptr;
					if (CoordValue->TryGetArray(CoordArray))
					{
						OutPoints.Add(ExtractCoordinate(*CoordArray));
					}
				}
			}
		}
	}
	else if (GeometryType == TEXT("MultiPolygon"))
	{
		// Array of arrays of arrays of coordinates
		for (const TSharedPtr<FJsonValue>& PolygonValue : *CoordinatesArray)
		{
			const TArray<TSharedPtr<FJsonValue>>* PolygonArray = nullptr;
			if (PolygonValue->TryGetArray(PolygonArray))
			{
				for (const TSharedPtr<FJsonValue>& RingValue : *PolygonArray)
				{
					const TArray<TSharedPtr<FJsonValue>>* RingArray = nullptr;
					if (RingValue->TryGetArray(RingArray))
					{
						for (const TSharedPtr<FJsonValue>& CoordValue : *RingArray)
						{
							const TArray<TSharedPtr<FJsonValue>>* CoordArray = nullptr;
							if (CoordValue->TryGetArray(CoordArray))
							{
								OutPoints.Add(ExtractCoordinate(*CoordArray));
							}
						}
					}
				}
			}
		}
	}

	return true;
}

FGeographicCoordinates UGeoJSONReader::ExtractCoordinate(const TArray<TSharedPtr<FJsonValue>>& CoordinateArray)
{
	FGeographicCoordinates Coordinate;

	if (CoordinateArray.Num() >= 2)
	{
		// GeoJSON format: [longitude, latitude, altitude (optional)]
		Coordinate.Longitude = CoordinateArray[0]->AsNumber();
		Coordinate.Latitude = CoordinateArray[1]->AsNumber();
		
		if (CoordinateArray.Num() >= 3)
		{
			Coordinate.Altitude = CoordinateArray[2]->AsNumber();
		}
		else
		{
			Coordinate.Altitude = 0.0;
		}
	}

	return Coordinate;
}
