# GeoReferencing API Documentation

Complete API reference for developers integrating the GeoReferencing plugin.

## Table of Contents
- [Core Classes](#core-classes)
- [Structures](#structures)
- [Enumerations](#enumerations)
- [Blueprint Function Libraries](#blueprint-function-libraries)
- [Usage Patterns](#usage-patterns)

## Core Classes

### AGeoReferencingSystem

The main actor class that handles all coordinate transformations.

**Header**: `GeoReferencingSystem.h`  
**Module**: `GeoReferencing`

#### Static Methods

##### GetGeoReferencingSystem
```cpp
static AGeoReferencingSystem* GetGeoReferencingSystem(UObject* WorldContextObject);
```
**Description**: Retrieves the GeoReferencingSystem instance from the current level.

**Parameters**:
- `WorldContextObject`: Any UObject with world context (typically `this`)

**Returns**: Pointer to AGeoReferencingSystem or nullptr if not found

**Usage**:
```cpp
AGeoReferencingSystem* GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
if (GeoRefSystem)
{
    // Use the system
}
```

#### Coordinate Transformation Methods

##### Engine to Geographic
```cpp
void EngineToGeographic(const FVector& EngineCoordinates, FGeographicCoordinates& GeographicCoordinates);
```
**Description**: Converts Unreal Engine coordinates to geographic coordinates.

**Parameters**:
- `EngineCoordinates`: Input position in UE coordinate system (cm)
- `GeographicCoordinates`: Output geographic coordinates (lat/lon/alt)

**Example**:
```cpp
FVector ActorLocation = GetActorLocation();
FGeographicCoordinates GeoCoords;
GeoRefSystem->EngineToGeographic(ActorLocation, GeoCoords);
UE_LOG(LogTemp, Log, TEXT("Location: %f, %f, %f"), 
    GeoCoords.Latitude, GeoCoords.Longitude, GeoCoords.Altitude);
```

##### Geographic to Engine
```cpp
void GeographicToEngine(const FGeographicCoordinates& GeographicCoordinates, FVector& EngineCoordinates);
```
**Description**: Converts geographic coordinates to Unreal Engine coordinates.

**Parameters**:
- `GeographicCoordinates`: Input geographic coordinates (lat/lon/alt)
- `EngineCoordinates`: Output position in UE coordinate system (cm)

**Example**:
```cpp
FGeographicCoordinates GeoCoords(51.5074, -0.1278, 0); // London
FVector EngineLocation;
GeoRefSystem->GeographicToEngine(GeoCoords, EngineLocation);
SetActorLocation(EngineLocation);
```

##### Engine to Projected
```cpp
void EngineToProjected(const FVector& EngineCoordinates, FVector& ProjectedCoordinates);
```
**Description**: Converts Engine coordinates to Projected CRS coordinates.

**Parameters**:
- `EngineCoordinates`: Input position in UE coordinate system (cm)
- `ProjectedCoordinates`: Output projected coordinates (meters, typically)

##### Projected to Engine
```cpp
void ProjectedToEngine(const FVector& ProjectedCoordinates, FVector& EngineCoordinates);
```
**Description**: Converts Projected CRS coordinates to Engine coordinates.

##### Engine to ECEF
```cpp
void EngineToECEF(const FVector& EngineCoordinates, FVector& ECEFCoordinates);
```
**Description**: Converts Engine coordinates to Earth-Centered, Earth-Fixed coordinates.

**Parameters**:
- `EngineCoordinates`: Input position in UE coordinate system (cm)
- `ECEFCoordinates`: Output ECEF coordinates (meters from Earth center)

##### ECEF to Engine
```cpp
void ECEFToEngine(const FVector& ECEFCoordinates, FVector& EngineCoordinates);
```
**Description**: Converts ECEF coordinates to Engine coordinates.

##### Geographic to Projected
```cpp
void GeographicToProjected(const FGeographicCoordinates& GeographicCoordinates, FVector& ProjectedCoordinates);
```
**Description**: Converts Geographic coordinates to Projected CRS coordinates.

##### Projected to Geographic
```cpp
void ProjectedToGeographic(const FVector& ProjectedCoordinates, FGeographicCoordinates& GeographicCoordinates);
```
**Description**: Converts Projected CRS coordinates to Geographic coordinates.

##### Geographic to ECEF
```cpp
void GeographicToECEF(const FGeographicCoordinates& GeographicCoordinates, FVector& ECEFCoordinates);
```
**Description**: Converts Geographic coordinates to ECEF coordinates.

##### ECEF to Geographic
```cpp
void ECEFToGeographic(const FVector& ECEFCoordinates, FGeographicCoordinates& GeographicCoordinates);
```
**Description**: Converts ECEF coordinates to Geographic coordinates.

##### Projected to ECEF
```cpp
void ProjectedToECEF(const FVector& ProjectedCoordinates, FVector& ECEFCoordinates);
```
**Description**: Converts Projected CRS coordinates to ECEF coordinates.

##### ECEF to Projected
```cpp
void ECEFToProjected(const FVector& ECEFCoordinates, FVector& ProjectedCoordinates);
```
**Description**: Converts ECEF coordinates to Projected CRS coordinates.

#### ENU (East-North-Up) Methods

##### GetENUVectorsAtEngineLocation
```cpp
void GetENUVectorsAtEngineLocation(const FVector& EngineCoordinates, FVector& East, FVector& North, FVector& Up);
```
**Description**: Returns the East, North, and Up unit vectors at a given location.

**Parameters**:
- `EngineCoordinates`: Location in Engine coordinates
- `East`: Output unit vector pointing East
- `North`: Output unit vector pointing North
- `Up`: Output unit vector pointing Up (away from Earth center)

**Usage**:
```cpp
FVector East, North, Up;
GeoRefSystem->GetENUVectorsAtEngineLocation(GetActorLocation(), East, North, Up);
// Align actor to face North
FRotator NorthRotation = North.Rotation();
SetActorRotation(NorthRotation);
```

##### GetENUVectorsAtGeographicLocation
```cpp
void GetENUVectorsAtGeographicLocation(const FGeographicCoordinates& GeographicCoordinates, FVector& East, FVector& North, FVector& Up);
```
**Description**: Returns ENU vectors at a geographic location.

##### GetENUVectorsAtProjectedLocation
```cpp
void GetENUVectorsAtProjectedLocation(const FVector& ProjectedCoordinates, FVector& East, FVector& North, FVector& Up);
```
**Description**: Returns ENU vectors at a projected coordinate location.

##### GetENUVectorsAtECEFLocation
```cpp
void GetENUVectorsAtECEFLocation(const FVector& ECEFCoordinates, FVector& East, FVector& North, FVector& Up);
```
**Description**: Returns ENU vectors at an ECEF location.

##### GetECEFENUVectorsAtECEFLocation
```cpp
void GetECEFENUVectorsAtECEFLocation(const FVector& ECEFCoordinates, FVector& ECEFEast, FVector& ECEFNorth, FVector& ECEFUp);
```
**Description**: Returns ENU vectors in pure ECEF frame (not transformed to Engine space).

#### Tangent Transform Methods

##### GetTangentTransformAtEngineLocation
```cpp
FTransform GetTangentTransformAtEngineLocation(const FVector& EngineCoordinates);
```
**Description**: Returns a transform oriented tangent to the ellipsoid at the given location.

**Returns**: FTransform with position and rotation tangent to planet surface

**Usage**:
```cpp
FTransform TangentTransform = GeoRefSystem->GetTangentTransformAtEngineLocation(Location);
Actor->SetActorTransform(TangentTransform);
// Actor is now positioned tangent to the planet surface
```

##### GetTangentTransformAtGeographicLocation
```cpp
FTransform GetTangentTransformAtGeographicLocation(const FGeographicCoordinates& GeographicCoordinates);
```
**Description**: Returns tangent transform at a geographic location.

##### GetTangentTransformAtProjectedLocation
```cpp
FTransform GetTangentTransformAtProjectedLocation(const FVector& ProjectedCoordinates);
```
**Description**: Returns tangent transform at a projected coordinate location.

##### GetTangentTransformAtECEFLocation
```cpp
FTransform GetTangentTransformAtECEFLocation(const FVector& ECEFCoordinates);
```
**Description**: Returns tangent transform at an ECEF location.

##### GetPlanetCenterTransform
```cpp
FTransform GetPlanetCenterTransform();
```
**Description**: Returns a transform positioned at the planet center, oriented tangent to the origin location.

#### Ellipsoid Methods

##### GetGeographicEllipsoidMaxRadius
```cpp
double GetGeographicEllipsoidMaxRadius();
```
**Description**: Returns the maximum (equatorial) radius of the Geographic CRS ellipsoid in meters.

##### GetGeographicEllipsoidMinRadius
```cpp
double GetGeographicEllipsoidMinRadius();
```
**Description**: Returns the minimum (polar) radius of the Geographic CRS ellipsoid in meters.

##### GetProjectedEllipsoidMaxRadius
```cpp
double GetProjectedEllipsoidMaxRadius();
```
**Description**: Returns the maximum radius of the Projected CRS ellipsoid in meters.

##### GetProjectedEllipsoidMinRadius
```cpp
double GetProjectedEllipsoidMinRadius();
```
**Description**: Returns the minimum radius of the Projected CRS ellipsoid in meters.

**Usage**:
```cpp
double EarthRadius = GeoRefSystem->GetGeographicEllipsoidMaxRadius();
UE_LOG(LogTemp, Log, TEXT("Earth radius: %f meters"), EarthRadius);
// Typical output: 6378137.0 meters for WGS84
```

#### Utility Methods

##### IsCRSStringValid
```cpp
bool IsCRSStringValid(FString CRSString, FString& Error);
```
**Description**: Validates a CRS string before using it.

**Parameters**:
- `CRSString`: The CRS string to validate (EPSG code, WKT, or PROJ string)
- `Error`: Output error message if invalid

**Returns**: true if valid, false otherwise

**Usage**:
```cpp
FString ErrorMsg;
if (!GeoRefSystem->IsCRSStringValid(TEXT("EPSG:4326"), ErrorMsg))
{
    UE_LOG(LogTemp, Error, TEXT("Invalid CRS: %s"), *ErrorMsg);
}
```

##### ApplySettings
```cpp
void ApplySettings();
```
**Description**: Applies changes made to CRS or origin properties at runtime. Must be called after modifying properties programmatically.

**Usage**:
```cpp
GeoRefSystem->ProjectedCRS = TEXT("EPSG:32631");
GeoRefSystem->OriginLatitude = 48.8566;
GeoRefSystem->OriginLongitude = 2.3522;
GeoRefSystem->ApplySettings(); // Apply the changes
```

#### Public Properties

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing")
EPlanetShape PlanetShape;
```
**Description**: Planet shape mode (FlatPlanet or RoundPlanet)

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "GeoReferencing")
FString ProjectedCRS;
```
**Description**: String describing the Projected CRS (e.g., "EPSG:32631")

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "GeoReferencing")
FString GeographicCRS;
```
**Description**: String describing the Geographic CRS (e.g., "EPSG:4326")

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing|Origin Location")
bool bOriginAtPlanetCenter;
```
**Description**: If true (Round Planet only), UE origin is at planet center

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing|Origin Location")
bool bOriginLocationInProjectedCRS;
```
**Description**: If true, origin is specified in Projected CRS; if false, in Geographic CRS

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing|Origin Location")
double OriginLatitude;
double OriginLongitude;
double OriginAltitude;
```
**Description**: Origin location in Geographic coordinates (when bOriginLocationInProjectedCRS is false)

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing|Origin Location")
double OriginProjectedCoordinatesEasting;
double OriginProjectedCoordinatesNorthing;
double OriginProjectedCoordinatesUp;
```
**Description**: Origin location in Projected coordinates (when bOriginLocationInProjectedCRS is true)

---

## Structures

### FGeographicCoordinates

**Header**: `GeographicCoordinates.h`  
**Module**: `GeoReferencing`

```cpp
USTRUCT(BlueprintType)
struct FGeographicCoordinates
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing")
    double Longitude;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing")
    double Latitude;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing")
    double Altitude;
};
```

**Description**: Represents a location in geographic coordinates.

**Units**:
- Latitude: Degrees, range [-90, 90] (negative = South, positive = North)
- Longitude: Degrees, range [-180, 180] (negative = West, positive = East)
- Altitude: Meters above the ellipsoid

#### Constructors

```cpp
FGeographicCoordinates();
FGeographicCoordinates(double InLongitude, double InLatitude, double InAltitude);
FGeographicCoordinates(const FVector& LatLongAltVector);
```

**Usage**:
```cpp
// Default constructor (0, 0, 0)
FGeographicCoordinates Origin;

// Explicit values
FGeographicCoordinates London(51.5074, -0.1278, 0);

// From FVector (X=Lat, Y=Lon, Z=Alt)
FVector LatLonAlt(40.7128, -74.0060, 10);
FGeographicCoordinates NewYork(LatLonAlt);
```

#### Methods

##### ToFullText
```cpp
FText ToFullText(int32 IntegralDigitsLatLon = 8, int32 IntegralDigitsAlti = 2, bool bAsDMS = false);
```
**Description**: Formats coordinates as "Lat= Lon= Alt=" text.

**Parameters**:
- `IntegralDigitsLatLon`: Number of decimal places for lat/lon
- `IntegralDigitsAlti`: Number of decimal places for altitude
- `bAsDMS`: If true, format as Degrees-Minutes-Seconds

##### ToCompactText
```cpp
FText ToCompactText(int32 IntegralDigitsLatLon = 8, int32 IntegralDigitsAlti = 2, bool bAsDMS = false);
```
**Description**: Formats coordinates as "(Lat, Lon, Alt)" text.

##### ToSeparateTexts
```cpp
void ToSeparateTexts(FText& OutLatitude, FText& OutLongitude, FText& OutAltitude, 
                     int32 IntegralDigitsLatLon = 8, int32 IntegralDigitsAlti = 2, bool bAsDMS = false);
```
**Description**: Formats coordinates as three separate text values.

### FEllipsoid

**Header**: `Ellipsoid.h`  
**Module**: `GeoReferencing`

```cpp
USTRUCT(BlueprintType)
struct FEllipsoid
{
    FVector Radii;
    FVector RadiiSquared;
    FVector OneOverRadii;
    FVector OneOverRadiiSquared;
};
```

**Description**: Represents an ellipsoid model for geodetic calculations.

#### Constructors

```cpp
FEllipsoid();
FEllipsoid(double RadiusX, double RadiusY, double RadiusZ);
FEllipsoid(const FVector& InRadii);
```

#### Methods

```cpp
double GetMaximumRadius();
double GetMinimumRadius();
FVector GeodeticSurfaceNormal(const FVector& ECEFLocation) const;
FVector GeodeticSurfaceNormal(const FGeographicCoordinates& GeographicCoordinates) const;
```

---

## Enumerations

### EPlanetShape

**Header**: `GeoReferencingSystem.h`

```cpp
UENUM(BlueprintType)
enum class EPlanetShape : uint8
{
    FlatPlanet,   // Projected space, no curvature
    RoundPlanet   // Planet-scale with curvature
};
```

**Description**: Determines how the plugin handles planet geometry.

**FlatPlanet**: 
- For local/regional scales
- Uses projected coordinate systems
- Ignores Earth's curvature
- Suitable for areas up to hundreds of kilometers

**RoundPlanet**: 
- For global/planetary scales
- Uses ellipsoidal Earth model
- Accounts for curvature
- Required for large-scale environments

---

## Blueprint Function Libraries

### UGeoReferencingBFL

**Header**: `GeoReferencingBFL.h`  
**Module**: `GeoReferencing`

Provides Blueprint-accessible functions for formatting Cartesian coordinates.

#### ToFullText
```cpp
static FText ToFullText(FVector& CartesianCoordinates, int32 IntegralDigits = 3);
```
**Description**: Converts a vector to "X= Y= Z=" formatted text.

#### ToCompactText
```cpp
static FText ToCompactText(FVector& CartesianCoordinates, int32 IntegralDigits = 3);
```
**Description**: Converts a vector to "(X, Y, Z)" formatted text.

#### ToSeparateTexts
```cpp
static void ToSeparateTexts(FVector& CartesianCoordinates, FText& OutX, FText& OutY, FText& OutZ, int32 IntegralDigits = 3);
```
**Description**: Converts a vector to three separate text values.

### UGeographicCoordinatesFunctionLibrary

**Header**: `GeographicCoordinates.h`  
**Module**: `GeoReferencing`

Blueprint utility functions for FGeographicCoordinates.

#### MakeGeographicCoordinates
```cpp
static FGeographicCoordinates MakeGeographicCoordinates(const FVector& LatLongAltVector);
```
**Description**: Creates FGeographicCoordinates from a vector where X=Latitude, Y=Longitude, Z=Altitude.

**Usage in Blueprint**: 
- Search for "Make Geographic Coordinates"
- Input: Vector with (Lat, Lon, Alt)
- Output: FGeographicCoordinates structure

---

## Usage Patterns

### Pattern 1: Initialize and Cache GeoReferencingSystem

```cpp
// In your class header
UPROPERTY()
AGeoReferencingSystem* GeoRefSystem;

// In BeginPlay or Initialize
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
    if (!GeoRefSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("GeoReferencingSystem not found in level!"));
    }
}
```

### Pattern 2: Batch Coordinate Conversion

```cpp
void AMyActor::ConvertMultiplePoints(const TArray<FGeographicCoordinates>& GeoPoints, TArray<FVector>& OutEngineLocations)
{
    OutEngineLocations.Empty();
    if (!GeoRefSystem) return;
    
    for (const FGeographicCoordinates& GeoPoint : GeoPoints)
    {
        FVector EngineLocation;
        GeoRefSystem->GeographicToEngine(GeoPoint, EngineLocation);
        OutEngineLocations.Add(EngineLocation);
    }
}
```

### Pattern 3: Position Actor at Geographic Location

```cpp
void AMyActor::SetGeographicLocation(double Latitude, double Longitude, double Altitude)
{
    if (!GeoRefSystem) return;
    
    FGeographicCoordinates GeoCoords(Longitude, Latitude, Altitude);
    FVector EngineLocation;
    GeoRefSystem->GeographicToEngine(GeoCoords, EngineLocation);
    SetActorLocation(EngineLocation);
}
```

### Pattern 4: Orient Actor to Planet Surface

```cpp
void AMyActor::AlignToSurface()
{
    if (!GeoRefSystem) return;
    
    FTransform TangentTransform = GeoRefSystem->GetTangentTransformAtEngineLocation(GetActorLocation());
    SetActorRotation(TangentTransform.GetRotation());
}
```

### Pattern 5: Track and Display Current Location

```cpp
void AMyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!GeoRefSystem || !PlayerPawn) return;
    
    FGeographicCoordinates CurrentLocation;
    GeoRefSystem->EngineToGeographic(PlayerPawn->GetActorLocation(), CurrentLocation);
    
    FString LocationText = FString::Printf(TEXT("Lat: %.6f, Lon: %.6f, Alt: %.2f m"),
        CurrentLocation.Latitude,
        CurrentLocation.Longitude,
        CurrentLocation.Altitude);
    
    // Display on HUD
    UpdateLocationDisplay(LocationText);
}
```

### Pattern 6: Validate CRS Before Use

```cpp
void AMyActor::SetCustomCRS(const FString& CRSString)
{
    if (!GeoRefSystem) return;
    
    FString ErrorMsg;
    if (GeoRefSystem->IsCRSStringValid(CRSString, ErrorMsg))
    {
        GeoRefSystem->ProjectedCRS = CRSString;
        GeoRefSystem->ApplySettings();
        UE_LOG(LogTemp, Log, TEXT("CRS changed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid CRS: %s"), *ErrorMsg);
    }
}
```

### Pattern 7: Get Direction to Geographic Location

```cpp
FVector AMyActor::GetDirectionToGeographicLocation(const FGeographicCoordinates& TargetLocation)
{
    if (!GeoRefSystem) return FVector::ZeroVector;
    
    FVector TargetEngineLocation;
    GeoRefSystem->GeographicToEngine(TargetLocation, TargetEngineLocation);
    
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (TargetEngineLocation - CurrentLocation).GetSafeNormal();
    
    return Direction;
}
```

### Pattern 8: Calculate Distance Between Geographic Points

```cpp
double AMyActor::GetDistanceBetweenGeoPoints(const FGeographicCoordinates& Point1, const FGeographicCoordinates& Point2)
{
    if (!GeoRefSystem) return 0.0;
    
    FVector Loc1, Loc2;
    GeoRefSystem->GeographicToEngine(Point1, Loc1);
    GeoRefSystem->GeographicToEngine(Point2, Loc2);
    
    // Distance in centimeters, convert to meters
    return FVector::Dist(Loc1, Loc2) / 100.0;
}
```

---

## Performance Considerations

1. **Cache the GeoReferencingSystem**: Don't call `GetGeoReferencingSystem()` every frame
2. **Batch conversions**: Convert multiple points in a single frame when possible
3. **Avoid tick conversions**: Use events or timers instead of converting every tick
4. **Pre-convert static data**: Convert static geographic data at load time
5. **Use appropriate CRS**: Choose the simplest CRS that meets your needs

## Thread Safety

- GeoReferencingSystem methods are **not thread-safe**
- All conversions must be called from the game thread
- PROJ library operations are not thread-safe

## Module Dependencies

To use this plugin in your C++ project, add to your `.Build.cs` file:

```csharp
PublicDependencyModuleNames.AddRange(new string[] { 
    "Core", 
    "CoreUObject", 
    "Engine",
    "GeoReferencing"  // Add this
});
```

---

**Version**: 1.0  
**Last Updated**: 2025
