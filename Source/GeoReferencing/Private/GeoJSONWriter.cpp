// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeoJSONWriter.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"

bool UGeoJSONWriter::SaveGeoJSONFile(
	const FString& FilePath,
	const TArray<FGeographicCoordinates>& Points,
	const TArray<FString>& Properties)
{
	FString GeoJSONString = ExportToGeoJSONString(Points, Properties);
	
	if (GeoJSONString.IsEmpty())
	{
		UE_LOG(LogGeoReferencing, Error, TEXT("Failed to generate GeoJSON content"));
		return false;
	}

	if (!FFileHelper::SaveStringToFile(GeoJSONString, *FilePath))
	{
		UE_LOG(LogGeoReferencing, Error, TEXT("Failed to write GeoJSON file: %s"), *FilePath);
		return false;
	}

	return true;
}

FString UGeoJSONWriter::ExportToGeoJSONString(
	const TArray<FGeographicCoordinates>& Points,
	const TArray<FString>& Properties)
{
	if (Points.Num() == 0)
	{
		UE_LOG(LogGeoReferencing, Warning, TEXT("No points to export to GeoJSON"));
		return FString();
	}

	// Create FeatureCollection
	TSharedPtr<FJsonObject> FeatureCollection = MakeShareable(new FJsonObject());
	FeatureCollection->SetStringField(TEXT("type"), TEXT("FeatureCollection"));

	// Create features array
	TArray<TSharedPtr<FJsonValue>> FeaturesArray;

	for (int32 i = 0; i < Points.Num(); ++i)
	{
		const FGeographicCoordinates& Point = Points[i];

		// Create feature
		TSharedPtr<FJsonObject> Feature = MakeShareable(new FJsonObject());
		Feature->SetStringField(TEXT("type"), TEXT("Feature"));

		// Create geometry
		TSharedPtr<FJsonObject> Geometry = MakeShareable(new FJsonObject());
		Geometry->SetStringField(TEXT("type"), TEXT("Point"));

		// Create coordinates array [longitude, latitude, altitude]
		TArray<TSharedPtr<FJsonValue>> CoordinatesArray;
		CoordinatesArray.Add(MakeShareable(new FJsonValueNumber(Point.Longitude)));
		CoordinatesArray.Add(MakeShareable(new FJsonValueNumber(Point.Latitude)));
		CoordinatesArray.Add(MakeShareable(new FJsonValueNumber(Point.Altitude)));

		Geometry->SetArrayField(TEXT("coordinates"), CoordinatesArray);
		Feature->SetObjectField(TEXT("geometry"), Geometry);

		// Add properties if available
		if (Properties.IsValidIndex(i) && !Properties[i].IsEmpty())
		{
			TSharedPtr<FJsonObject> PropertiesObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Properties[i]);
			
			if (FJsonSerializer::Deserialize(Reader, PropertiesObject) && PropertiesObject.IsValid())
			{
				Feature->SetObjectField(TEXT("properties"), PropertiesObject);
			}
			else
			{
				// If property parsing fails, use empty object
				Feature->SetObjectField(TEXT("properties"), MakeShareable(new FJsonObject()));
			}
		}
		else
		{
			// Empty properties object
			Feature->SetObjectField(TEXT("properties"), MakeShareable(new FJsonObject()));
		}

		FeaturesArray.Add(MakeShareable(new FJsonValueObject(Feature)));
	}

	FeatureCollection->SetArrayField(TEXT("features"), FeaturesArray);

	// Serialize to string
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	
	if (!FJsonSerializer::Serialize(FeatureCollection.ToSharedRef(), Writer))
	{
		UE_LOG(LogGeoReferencing, Error, TEXT("Failed to serialize GeoJSON"));
		return FString();
	}

	return OutputString;
}
