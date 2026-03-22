# GeoJSON Loader Documentation

**GeoReferencing Plugin – GeoJSON & GeoTIFF Loading Guide**

---

## Table of Contents

1. [What Is GeoJSON?](#what-is-geojson)
2. [GeoJSON Loader Overview](#geojson-loader-overview)
3. [Supported Geometry Types](#supported-geometry-types)
4. [Classes at a Glance](#classes-at-a-glance)
5. [C++ Examples](#c-examples)
   - [Load from a File](#load-from-a-file)
   - [Load from a String](#load-from-a-string)
   - [Export to a File](#export-to-a-file)
   - [Export to a String](#export-to-a-string)
   - [Full Import → Transform → Export Pipeline](#full-import--transform--export-pipeline)
   - [Reading Feature Properties](#reading-feature-properties)
6. [Blueprint Examples](#blueprint-examples)
   - [Load GeoJSON File and Spawn Actors](#blueprint-example-1-load-geojson-file-and-spawn-actors)
   - [Export Actor Positions to GeoJSON](#blueprint-example-2-export-actor-positions-to-geojson)
   - [Parse GeoJSON from a Web Response String](#blueprint-example-3-parse-geojson-from-a-web-response-string)
7. [GeoJSON File Format Reference](#geojson-file-format-reference)
8. [Error Handling](#error-handling)
9. [Coordinate Order Convention](#coordinate-order-convention)
10. [GeoTIFF Loading](#geotiff-loading)
11. [Frequently Asked Questions](#frequently-asked-questions)

---

## What Is GeoJSON?

[GeoJSON](https://geojson.org/) (RFC 7946) is a text-based format for encoding geographic data structures using JSON. It is widely used by GIS tools such as QGIS, ArcGIS, and Mapbox to share points, lines, and polygons with optional attribute (properties) data.

A minimal GeoJSON file looks like this:

```json
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "geometry": {
        "type": "Point",
        "coordinates": [37.6173, 55.7558, 145.0]
      },
      "properties": {
        "name": "Moscow",
        "population": 12500000
      }
    }
  ]
}
```

> **Coordinate order in GeoJSON is always `[longitude, latitude, altitude]`** – the opposite of the traditional "lat/long" pair.

---

## GeoJSON Loader Overview

The plugin provides two Blueprint-callable utility classes for GeoJSON I/O:

| Class | Purpose |
|-------|---------|
| `UGeoJSONReader` | Read GeoJSON files or strings → `TArray<FGeographicCoordinates>` |
| `UGeoJSONWriter` | Write `TArray<FGeographicCoordinates>` → GeoJSON file or string |

Both classes expose **static Blueprint-callable functions** so they work in C++ without instantiation and are directly available as nodes in the Blueprint graph under the category **GeoReferencing | Formats**.

The `FGeographicCoordinates` struct returned by the reader contains:

```cpp
double Longitude;  // degrees East of prime meridian
double Latitude;   // degrees North of equator
double Altitude;   // metres above the WGS-84 ellipsoid (0 if not provided)
```

---

## Supported Geometry Types

`UGeoJSONReader` extracts every vertex from the following geometry types:

| GeoJSON type | What is extracted |
|---|---|
| `Point` | The single coordinate |
| `MultiPoint` | Each coordinate in the array |
| `LineString` | Every vertex along the line |
| `MultiLineString` | Every vertex of every line |
| `Polygon` | Every vertex of every ring (exterior + holes) |
| `MultiPolygon` | Every vertex of every ring of every polygon |
| `Feature` | Coordinates from the embedded geometry |
| `FeatureCollection` | Coordinates from every feature; properties preserved per-feature |

---

## Classes at a Glance

### UGeoJSONReader

```cpp
// Declared in Source/GeoReferencing/Public/GeoJSONReader.h

UCLASS()
class GEOREFERENCING_API UGeoJSONReader : public UObject
{
public:
    // Load from a file on disk
    UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
    static bool LoadGeoJSONFile(
        const FString& FilePath,
        TArray<FGeographicCoordinates>& OutPoints,
        TArray<FString>& OutProperties);

    // Parse an in-memory string
    UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
    static bool LoadGeoJSONString(
        const FString& GeoJSONContent,
        TArray<FGeographicCoordinates>& OutPoints,
        TArray<FString>& OutProperties);
};
```

**Return value**: `true` if parsing succeeded and at least one coordinate was extracted; `false` otherwise (errors are logged to the Output Log).

### UGeoJSONWriter

```cpp
// Declared in Source/GeoReferencing/Public/GeoJSONWriter.h

UCLASS()
class GEOREFERENCING_API UGeoJSONWriter : public UObject
{
public:
    // Write a FeatureCollection to a file
    UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
    static bool SaveGeoJSONFile(
        const FString& FilePath,
        const TArray<FGeographicCoordinates>& Points,
        const TArray<FString>& Properties);

    // Serialize to an in-memory string (useful for HTTP payloads)
    UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Formats")
    static FString ExportToGeoJSONString(
        const TArray<FGeographicCoordinates>& Points,
        const TArray<FString>& Properties);
};
```

The writer always produces a `FeatureCollection` with one `Point` Feature per coordinate. If the `Properties` array has fewer entries than `Points`, empty property objects (`{}`) are used for the missing ones.

---

## C++ Examples

All examples below assume you have included the necessary headers:

```cpp
#include "GeoJSONReader.h"
#include "GeoJSONWriter.h"
#include "GeographicCoordinates.h"
#include "GeoReferencingSystem.h"
```

### Load from a File

```cpp
void AMyActor::LoadLocationsFromFile()
{
    TArray<FGeographicCoordinates> Points;
    TArray<FString> Properties;

    const FString FilePath = FPaths::ProjectContentDir() / TEXT("Data/locations.geojson");

    if (UGeoJSONReader::LoadGeoJSONFile(FilePath, Points, Properties))
    {
        UE_LOG(LogTemp, Log, TEXT("Loaded %d points from GeoJSON"), Points.Num());

        for (int32 i = 0; i < Points.Num(); ++i)
        {
            UE_LOG(LogTemp, Log, TEXT("  [%d] Lat=%.6f Lon=%.6f Alt=%.1f   Props=%s"),
                i,
                Points[i].Latitude,
                Points[i].Longitude,
                Points[i].Altitude,
                *Properties[i]);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load GeoJSON file: %s"), *FilePath);
    }
}
```

---

### Load from a String

Use this when you receive GeoJSON from a network request or generate it at runtime.

```cpp
void AMyActor::ParseGeoJSONString(const FString& RawGeoJSON)
{
    TArray<FGeographicCoordinates> Points;
    TArray<FString> Properties;

    if (UGeoJSONReader::LoadGeoJSONString(RawGeoJSON, Points, Properties))
    {
        // Convert each geographic coordinate to an Unreal Engine world location
        AGeoReferencingSystem* GeoSystem =
            AGeoReferencingSystem::GetGeoReferencingSystem(GetWorld());

        for (const FGeographicCoordinates& Geo : Points)
        {
            FVector WorldLocation;
            GeoSystem->GeographicToEngine(Geo, WorldLocation);

            // Spawn a marker at the world location
            GetWorld()->SpawnActor<ALocationMarker>(
                ALocationMarker::StaticClass(),
                WorldLocation,
                FRotator::ZeroRotator);
        }
    }
}
```

---

### Export to a File

```cpp
void AMyActor::SaveLocationsToFile(
    const TArray<FGeographicCoordinates>& Locations,
    const TArray<FString>& MetaData)
{
    const FString OutPath = FPaths::ProjectSavedDir() / TEXT("Export/waypoints.geojson");

    if (UGeoJSONWriter::SaveGeoJSONFile(OutPath, Locations, MetaData))
    {
        UE_LOG(LogTemp, Log, TEXT("GeoJSON saved to: %s"), *OutPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save GeoJSON to: %s"), *OutPath);
    }
}
```

---

### Export to a String

Useful for sending GeoJSON over HTTP without writing to disk.

```cpp
FString AMyActor::BuildGeoJSONPayload(const TArray<FGeographicCoordinates>& Locations)
{
    // Build property JSON for each location
    TArray<FString> Properties;
    for (int32 i = 0; i < Locations.Num(); ++i)
    {
        Properties.Add(FString::Printf(
            TEXT("{\"id\":%d,\"timestamp\":%lld}"),
            i,
            FDateTime::UtcNow().ToUnixTimestamp()));
    }

    return UGeoJSONWriter::ExportToGeoJSONString(Locations, Properties);
}
```

---

### Full Import → Transform → Export Pipeline

This is the most common workflow when working with GIS data:

1. Import geographic coordinates from a GeoJSON file.
2. Convert them to Unreal Engine world coordinates (batch for performance).
3. Process or spawn objects at those locations.
4. Optionally write results back to a new GeoJSON file.

```cpp
void AMyGameMode::ProcessGISData()
{
    AGeoReferencingSystem* GeoSystem =
        AGeoReferencingSystem::GetGeoReferencingSystem(GetWorld());
    if (!GeoSystem) return;

    // --- Step 1: Import ---
    TArray<FGeographicCoordinates> GeoPoints;
    TArray<FString> Properties;
    const FString InputFile = FPaths::ProjectContentDir() / TEXT("Data/poi.geojson");

    if (!UGeoJSONReader::LoadGeoJSONFile(InputFile, GeoPoints, Properties))
    {
        UE_LOG(LogTemp, Error, TEXT("Could not load %s"), *InputFile);
        return;
    }

    // --- Step 2: Batch transform (faster than one-by-one) ---
    TArray<FVector> WorldPoints;
    GeoSystem->GeographicToEngineBatch(GeoPoints, WorldPoints);

    // --- Step 3: Spawn actors ---
    for (int32 i = 0; i < WorldPoints.Num(); ++i)
    {
        AActor* Marker = GetWorld()->SpawnActor<APointOfInterest>(
            APointOfInterest::StaticClass(),
            WorldPoints[i],
            FRotator::ZeroRotator);

        if (Marker)
        {
            Cast<APointOfInterest>(Marker)->SetProperties(Properties[i]);
        }
    }

    // --- Step 4: Export result ---
    const FString OutputFile = FPaths::ProjectSavedDir() / TEXT("processed_poi.geojson");
    UGeoJSONWriter::SaveGeoJSONFile(OutputFile, GeoPoints, Properties);

    UE_LOG(LogTemp, Log, TEXT("Processed %d points of interest."), GeoPoints.Num());
}
```

---

### Reading Feature Properties

The `OutProperties` array returned by `LoadGeoJSONFile` / `LoadGeoJSONString` contains one JSON string per feature. Parse individual fields with Unreal's `FJsonSerializer`:

```cpp
void AMyActor::ReadProperties(const FString& PropertiesJSON)
{
    TSharedPtr<FJsonObject> Props;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(PropertiesJSON);

    if (FJsonSerializer::Deserialize(Reader, Props) && Props.IsValid())
    {
        FString Name;
        int32   Population = 0;
        double  Elevation  = 0.0;

        Props->TryGetStringField(TEXT("name"),       Name);
        Props->TryGetNumberField(TEXT("population"), Population);
        Props->TryGetNumberField(TEXT("elevation"),  Elevation);

        UE_LOG(LogTemp, Log, TEXT("Name=%s  Population=%d  Elevation=%.1f"),
            *Name, Population, Elevation);
    }
}
```

---

## Blueprint Examples

Blueprint nodes for GeoJSON I/O live under **GeoReferencing | Formats** in the function search. All node names match their C++ function names.

---

### Blueprint Example 1: Load GeoJSON File and Spawn Actors

**Goal**: Read waypoints from a GeoJSON file on disk and place an actor at every coordinate.

**Step-by-step node graph**:

```
[Event BeginPlay]
        │
        ▼
[Load Geo JSON File]──── File Path: "C:/Data/waypoints.geojson"
   │              │
   │ Out Points   │ Out Properties
   │              │
   ▼              ▼
[Geographic To Engine Batch]  ◄── (GeoReferencingSystem reference)
        │
        │ Out Engine Coords (Array of Vector)
        ▼
[For Each Loop with Break]
        │ Array Element (Vector)
        ▼
[Spawn Actor from Class]──── Class: BP_WaypointMarker
   Location: Array Element
```

**Node descriptions**:

| Node | Category | Key Inputs / Outputs |
|---|---|---|
| `Load Geo JSON File` | GeoReferencing\|Formats | **File Path** → `bool` (success), **Out Points** array, **Out Properties** array |
| `Geographic To Engine Batch` | GeoReferencing | **Geo System** reference, **Geographic Coords** → **Engine Coords** |
| `For Each Loop` | Utilities\|Array | Iterates **Engine Coords** |
| `Spawn Actor from Class` | Game | **Class**, **Location** |

**Add a Branch after `Load Geo JSON File`** to check the boolean return value and print a warning if loading failed:

```
[Load Geo JSON File]
        │ Return Value (bool)
        ▼
   [Branch]
  True ──► [For Each Loop] ──► [Spawn Actor …]
  False ──► [Print String]──── "Failed to load GeoJSON"
```

---

### Blueprint Example 2: Export Actor Positions to GeoJSON

**Goal**: Collect the geographic positions of all `BP_POI` actors in the level and save them to a GeoJSON file.

```
[Event BeginPlay]
        │
        ▼
[Get All Actors of Class]──── Actor Class: BP_POI
        │ Out Actors (Array)
        ▼
[For Each Loop]
        │ Array Element (Actor reference)
        │
        ├─► [Get Actor Location] → FVector
        │         │
        │         ▼
        │   [Engine To Geographic]──── GeoSystem reference
        │         │ Geographic Coords (FGeographicCoordinates)
        │         ▼
        │   [Add (Array)] ──► GeoPoints local array
        │
        └─► [Make Literal String] "{\"source\":\"level\"}"
              │
              ▼
        [Add (Array)] ──► Properties local array
        │
       (after loop)
        ▼
[Save Geo JSON File]
   File Path:   "C:/Export/actors.geojson"
   Points:      GeoPoints array
   Properties:  Properties array
        │ Return Value (bool)
        ▼
   [Print String]──── "Saved X actors to GeoJSON"
```

---

### Blueprint Example 3: Parse GeoJSON from a Web Response String

**Goal**: After a successful HTTP GET, parse the response body as GeoJSON.

```
[On Request Complete (HTTP)]
        │ Response Body (String)
        ▼
[Load Geo JSON String]
        │ Out Points   │ Out Properties
        ▼              ▼
[For Each Loop]──────────────────────────┐
        │ Array Element (FGeographicCoords) │
        ▼                                  │
[Geographic To Engine]──── GeoSystem ref   │
        │ Engine Coords (Vector)            │
        ▼                                  │
[Set Actor Location]                       │
  (Target: pre-spawned marker actors) ◄────┘
```

---

## GeoJSON File Format Reference

### Feature Collection (most common)

```json
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "geometry": {
        "type": "Point",
        "coordinates": [37.6173, 55.7558, 145.0]
      },
      "properties": {
        "name": "Moscow",
        "category": "city"
      }
    },
    {
      "type": "Feature",
      "geometry": {
        "type": "LineString",
        "coordinates": [
          [37.6, 55.75],
          [37.7, 55.80],
          [37.8, 55.85]
        ]
      },
      "properties": { "road": "M10" }
    }
  ]
}
```

### Bare Geometry (no Feature wrapper)

```json
{
  "type": "Point",
  "coordinates": [139.6917, 35.6895, 40.0]
}
```

### MultiPoint

```json
{
  "type": "MultiPoint",
  "coordinates": [
    [37.62, 55.75],
    [30.32, 59.93],
    [82.92, 55.03]
  ]
}
```

### Polygon

```json
{
  "type": "Polygon",
  "coordinates": [
    [
      [37.60, 55.75],
      [37.65, 55.75],
      [37.65, 55.80],
      [37.60, 55.80],
      [37.60, 55.75]
    ]
  ]
}
```

---

## Error Handling

Both `LoadGeoJSONFile` and `LoadGeoJSONString` return `false` and write a descriptive message to the Unreal Output Log (`LogGeoReferencing` category) when:

- The file path does not exist or cannot be read.
- The string content is not valid JSON.
- The `"type"` field is missing or unrecognised.
- The `"features"` array is absent from a `FeatureCollection`.
- No coordinates were extracted (zero points after parsing).

**C++ – check the return value**:

```cpp
TArray<FGeographicCoordinates> Points;
TArray<FString> Properties;

if (!UGeoJSONReader::LoadGeoJSONFile(FilePath, Points, Properties))
{
    // Inspect the Output Log for the specific error message.
    UE_LOG(LogTemp, Warning,
        TEXT("GeoJSON load failed. Check Output Log for details. Path: %s"),
        *FilePath);
    return;
}
```

**Blueprint – use the Branch node**:

After the `Load Geo JSON File` node connect the boolean return pin to a `Branch`. Route the *False* path to a `Print String` or `Log` node to surface the error to the designer.

---

## Coordinate Order Convention

> ⚠️ **GeoJSON stores coordinates as `[longitude, latitude, altitude]`**, not `[latitude, longitude]`.  
> The plugin handles this automatically – `FGeographicCoordinates` is always populated with the correct values regardless of the source format.

| Source | Array order |
|---|---|
| GeoJSON (RFC 7946) | `[longitude, latitude, altitude]` |
| `FGeographicCoordinates` struct | `.Longitude`, `.Latitude`, `.Altitude` (separate fields) |

When writing custom property strings, store coordinates as GeoJSON dictates so exported files are compatible with QGIS, ArcGIS, and Mapbox.

---

## GeoTIFF Loading

> **GeoTIFF support is planned for Phase 4 of the plugin roadmap** and is not yet implemented.

GeoTIFF (`.tif` / `.tiff`) is a raster format that stores geospatially referenced satellite imagery and Digital Elevation Models (DEMs). When implemented, the planned `UGeoTIFFReader` class will:

- Read elevation data from DEMs and expose it as an `FHeightmap`.
- Import satellite imagery as `UTexture2D` with embedded CRS metadata.
- Allow terrain mesh generation directly from elevation grids.

**Current workaround**: Export elevation data from GIS tools (QGIS, GDAL) as a point-cloud GeoJSON file and use `UGeoJSONReader` to load the elevation values via the `Altitude` field of `FGeographicCoordinates`.

Stay updated on GeoTIFF progress in [CHANGELOG.md](../CHANGELOG.md) and [IMPLEMENTATION_ROADMAP.md](IMPLEMENTATION_ROADMAP.md).

---

## Frequently Asked Questions

**Q: The loader returns `false` but my JSON looks valid. What is wrong?**

Check:
1. The file path – use an absolute path or `FPaths::ProjectContentDir()`.
2. The JSON has a `"type"` field at the top level (`"FeatureCollection"`, `"Feature"`, or a geometry type).
3. There is at least one coordinate in the data (the function returns `false` for empty data).
4. Open the Unreal **Output Log** and filter on `LogGeoReferencing` for the specific error.

---

**Q: Can I load a GeoJSON file that has only a `Polygon`, without a `Feature` wrapper?**

Yes. `UGeoJSONReader` accepts bare geometry objects as well as `Feature` and `FeatureCollection` top-level types.

---

**Q: The coordinates I get back seem swapped (latitude where I expect longitude).**

GeoJSON stores `[longitude, latitude]`. The plugin reads them in that order into `FGeographicCoordinates.Longitude` and `FGeographicCoordinates.Latitude` correctly. If you are supplying a custom JSON string, ensure your data follows the RFC 7946 convention.

---

**Q: Can I include altitude in my GeoJSON?**

Yes. Add a third value to the coordinate array: `[longitude, latitude, altitude_in_metres]`. If omitted, `FGeographicCoordinates.Altitude` defaults to `0.0`.

---

**Q: How do I open the exported GeoJSON in QGIS?**

1. Open QGIS → Layer → Add Layer → Add Vector Layer.
2. Choose "File" as the source type.
3. Browse to your `.geojson` file.
4. Click **Add**. The points will appear in the QGIS canvas.

---

**Q: What is the recommended approach for very large datasets (>100 000 points)?**

Use `GeographicToEngineBatchParallel()` instead of the standard batch function after loading. This splits the coordinate transformation across multiple CPU threads and is typically 10–15× faster for large datasets. See [USAGE_GUIDE.md](USAGE_GUIDE.md) for performance benchmarks.

---

*Last updated: March 2026 · Plugin version 1.0 · Maintained by COVISART*
