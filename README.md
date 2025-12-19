# GeoReferencing Plugin for Unreal Engine

A comprehensive georeferencing plugin for Unreal Engine that enables seamless conversion between different coordinate reference systems (CRS) for creating geospatially-accurate virtual worlds.

📄 GitHub Pages documentation: https://nobatgeldi.github.io/GeoReferencing/

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Getting Started](#getting-started)
- [Core Concepts](#core-concepts)
- [API Reference](#api-reference)
- [Blueprints Usage](#blueprints-usage)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Overview

The GeoReferencing plugin provides tools to establish a correspondence between Unreal Engine's coordinate system and real-world geographic locations. It supports both flat (projected) and round (ellipsoidal) planet representations, making it suitable for projects ranging from small local areas to planet-scale environments.

### Key Components

- **GeoReferencingSystem**: Main actor that manages coordinate transformations
- **Geographic Coordinates**: Latitude/Longitude/Altitude representation
- **Ellipsoid Support**: Earth ellipsoid models for accurate geodetic calculations
- **RoundPlanetPawn**: Specialized pawn for navigating planet-scale environments
- **PROJ Integration**: Uses PROJ library for professional-grade coordinate transformations

## Features

### Coordinate System Support

- **Multiple CRS Types**:
  - Engine coordinates (Unreal Engine's local coordinate system)
  - Geographic coordinates (Latitude/Longitude/Altitude)
  - Projected coordinates (e.g., UTM, Mercator)
  - ECEF (Earth-Centered, Earth-Fixed) coordinates

- **Bidirectional Conversions** between all coordinate systems
- **EPSG Code Support**: Use standard EPSG codes to define coordinate systems
- **WKT Support**: Well-Known Text format for custom CRS definitions
- **PROJ String Support**: Direct PROJ projection strings

### Planet Modes

#### Flat Planet Mode
- Suitable for small to medium-sized environments
- Uses projected coordinate systems (e.g., UTM, State Plane)
- Ignores Earth's curvature for simplicity
- Best for areas up to several hundred kilometers

#### Round Planet Mode
- Designed for planet-scale environments
- Accounts for Earth's curvature
- Supports ECEF transformations
- Includes specialized navigation tools
- Origin can be at planet center or on the surface

### Navigation Features

- **RoundPlanetPawn**: Purpose-built pawn for planet-scale navigation
  - Automatic orientation adjustment based on planet surface
  - Smooth "fly-to" location animations
  - Dynamic speed adjustment based on altitude
  - Height Above Terrain (HAT) tracking
  - Orbital and surface-relative movement modes

- **ENU (East-North-Up) Vectors**: Get local tangent plane vectors at any location
- **Tangent Transforms**: Position objects tangent to the ellipsoid surface

### Blueprint Support

- Full Blueprint integration with visual scripting
- Blueprint Function Library for coordinate formatting
- Type-safe coordinate structures
- Easy-to-use conversion nodes

### New Features (Phase 1-3 Implementation)

#### Phase 1: Production-Ready Enhancements
- **Accuracy Validation**: Query transformation accuracy between CRS systems
- **Robust Error Handling**: Comprehensive error reporting with input validation
- **Edge Case Support**: Handles poles (±90°) and antimeridian (±180°) correctly

#### Phase 2: Performance Optimization
- **Batch Transformations**: 3x-5x faster batch processing with pre-allocated arrays
- **Parallel Processing**: Multi-threaded transformations (10x-15x faster for large datasets)
- **Performance Monitoring**: Real-time statistics with Unreal Insights integration

#### Phase 3: Data Interoperability
- **GeoJSON Support**: RFC 7946 compliant import/export for GIS data exchange
- **Precision Calculator**: Analyze coordinate precision and get rebasing recommendations
- **Professional Workflows**: Seamless integration with QGIS, ArcGIS, and other GIS tools

**See**: [Implemented Features Documentation](docs/IMPLEMENTED_FEATURES.md) for complete API reference

### Platform Support

- Windows (Win64)
- Linux (x64)
- macOS
- Android (ARM64, x64)
- iOS

## System Requirements

- **Unreal Engine**: 4.27+ or 5.0+
- **Dependencies**: 
  - SQLiteCore plugin (automatically enabled)
  - PROJ library (included)
- **Supported Platforms**: Windows, Linux, macOS, iOS, Android

## Installation

### Option 1: Plugin Installation (Recommended)

1. Download or clone this repository
2. Copy the `GeoReferencing` folder to your project's `Plugins` directory
3. If the `Plugins` folder doesn't exist, create it in your project root
4. Restart Unreal Engine Editor
5. Enable the plugin:
   - Open **Edit → Plugins**
   - Search for "GeoReferencing"
   - Check the **Enabled** checkbox
   - Restart the editor when prompted

### Option 2: Engine Plugin Installation

1. Copy the `GeoReferencing` folder to your Unreal Engine's `Engine/Plugins` directory
2. The plugin will be available for all projects

### Verification

After installation, verify the plugin is working:
1. Create a new level or open an existing one
2. In the **Place Actors** panel, search for "GeoReferencingSystem"
3. Drag the **GeoReferencingSystem** actor into your level

## Getting Started

### Basic Setup

1. **Add GeoReferencingSystem to Your Level**
   - Search for "GeoReferencingSystem" in the Place Actors panel
   - Drag it into your level (only one instance needed per level)
   - The actor will appear in the World Outliner

2. **Configure the GeoReferencingSystem**

   In the Details panel, configure the following properties:

   **Planet Shape**:
   - `Flat Planet`: For small areas with projected coordinates
   - `Round Planet`: For planet-scale environments

   **Coordinate Systems**:
   - `ProjectedCRS`: Define your projected coordinate system (default: EPSG:32631 - UTM Zone 31N)
   - `GeographicCRS`: Define your geographic coordinate system (default: EPSG:4326 - WGS84)

   **Origin Location**:
   - For Flat Planet: Set the UE origin's geographic or projected coordinates
   - For Round Planet: Choose if origin is at planet center or on the surface

3. **Test Coordinate Conversion**

   Create a simple Blueprint to test conversions:
   ```
   Event BeginPlay
   → Get GeoReferencing System
   → Engine To Geographic
   ```

### Example: Converting a Location

**In Blueprints**:
```
1. Get GeoReferencingSystem (World Context)
2. Get Actor Location (your actor)
3. Engine To Geographic (using GeoReferencingSystem)
4. Print the resulting Latitude, Longitude, Altitude
```

**In C++**:
```cpp
AGeoReferencingSystem* GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
if (GeoRefSystem)
{
    FGeographicCoordinates GeographicCoords;
    GeoRefSystem->EngineToGeographic(ActorLocation, GeographicCoords);
    UE_LOG(LogTemp, Log, TEXT("Lat: %f, Lon: %f, Alt: %f"), 
        GeographicCoords.Latitude, 
        GeographicCoords.Longitude, 
        GeographicCoords.Altitude);
}
```

## Core Concepts

### Coordinate Reference Systems (CRS)

The plugin supports four main coordinate systems:

1. **Engine Coordinates**
   - Unreal Engine's native coordinate system
   - Origin at UE world origin (0, 0, 0)
   - Units: centimeters
   - Left-handed Z-up coordinate system

2. **Geographic Coordinates**
   - Latitude: -90° (South Pole) to +90° (North Pole)
   - Longitude: -180° (West) to +180° (East)
   - Altitude: Height above ellipsoid in meters
   - Example: EPSG:4326 (WGS84)

3. **Projected Coordinates**
   - 2D map projection coordinates (Easting, Northing, Up)
   - Units typically in meters
   - Examples: UTM zones, State Plane, Mercator
   - Used for flat planet mode

4. **ECEF Coordinates**
   - Earth-Centered, Earth-Fixed 3D Cartesian system
   - Origin at Earth's center of mass
   - X-axis points to 0° latitude, 0° longitude
   - Used for round planet mode

### Transformations

The plugin provides conversions between all coordinate systems:

```
       Engine ←→ Geographic
          ↕           ↕
      Projected ←→  ECEF
```

All conversions are bidirectional and accessible via both C++ and Blueprints.

### Origin Configuration

#### Flat Planet Mode
- Origin can be specified in either Geographic or Projected coordinates
- Typically set to a convenient location within your area of interest
- All engine coordinates are relative to this origin

#### Round Planet Mode
- **Origin at Planet Center**: UE origin (0,0,0) = Earth's center
  - Useful for large-scale space or orbital views
  - Requires dynamic rebasing for large-scale ground-level work
  
- **Origin on Surface**: UE origin at a specific surface location
  - More convenient for ground-level applications
  - Define origin using Geographic or Projected coordinates

### Ellipsoid Models

The plugin uses ellipsoid models for accurate geodetic calculations:
- Automatically extracted from the chosen CRS
- Common models: WGS84, GRS80, International
- Provides methods to query ellipsoid parameters

## API Reference

### AGeoReferencingSystem

Main actor class for coordinate transformations.

#### Getting the System

**Blueprint**: Use "Get GeoReferencing System" node
**C++**: 
```cpp
AGeoReferencingSystem* GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(WorldContextObject);
```

#### Coordinate Conversions

##### Engine ↔ Geographic

**Blueprint**: 
- `Engine To Geographic`
- `Geographic To Engine`

**C++**:
```cpp
void EngineToGeographic(const FVector& EngineCoordinates, FGeographicCoordinates& GeographicCoordinates);
void GeographicToEngine(const FGeographicCoordinates& GeographicCoordinates, FVector& EngineCoordinates);
```

##### Engine ↔ Projected

**Blueprint**: 
- `Engine To Projected`
- `Projected To Engine`

**C++**:
```cpp
void EngineToProjected(const FVector& EngineCoordinates, FVector& ProjectedCoordinates);
void ProjectedToEngine(const FVector& ProjectedCoordinates, FVector& EngineCoordinates);
```

##### Engine ↔ ECEF

**Blueprint**: 
- `Engine To ECEF`
- `ECEF To Engine`

**C++**:
```cpp
void EngineToECEF(const FVector& EngineCoordinates, FVector& ECEFCoordinates);
void ECEFToEngine(const FVector& ECEFCoordinates, FVector& EngineCoordinates);
```

##### Geographic ↔ Projected

**Blueprint**: 
- `Geographic To Projected`
- `Projected To Geographic`

**C++**:
```cpp
void GeographicToProjected(const FGeographicCoordinates& GeographicCoordinates, FVector& ProjectedCoordinates);
void ProjectedToGeographic(const FVector& ProjectedCoordinates, FGeographicCoordinates& GeographicCoordinates);
```

##### Geographic ↔ ECEF

**Blueprint**: 
- `Geographic To ECEF`
- `ECEF To Geographic`

**C++**:
```cpp
void GeographicToECEF(const FGeographicCoordinates& GeographicCoordinates, FVector& ECEFCoordinates);
void ECEFToGeographic(const FVector& ECEFCoordinates, FGeographicCoordinates& GeographicCoordinates);
```

##### Projected ↔ ECEF

**Blueprint**: 
- `Projected To ECEF`
- `ECEF To Projected`

**C++**:
```cpp
void ProjectedToECEF(const FVector& ProjectedCoordinates, FVector& ECEFCoordinates);
void ECEFToProjected(const FVector& ECEFCoordinates, FVector& ProjectedCoordinates);
```

#### ENU Vectors

Get East, North, and Up unit vectors at a location:

**Blueprint**: 
- `Get ENU Vectors At Engine Location`
- `Get ENU Vectors At Geographic Location`
- `Get ENU Vectors At Projected Location`
- `Get ENU Vectors At ECEF Location`

**C++**:
```cpp
void GetENUVectorsAtEngineLocation(const FVector& EngineCoordinates, FVector& East, FVector& North, FVector& Up);
void GetENUVectorsAtGeographicLocation(const FGeographicCoordinates& GeographicCoordinates, FVector& East, FVector& North, FVector& Up);
```

#### Tangent Transforms

Get transformation matrix for objects tangent to the ellipsoid:

**Blueprint**: 
- `Get Tangent Transform At Engine Location`
- `Get Tangent Transform At Geographic Location`
- `Get Tangent Transform At Projected Location`
- `Get Tangent Transform At ECEF Location`

**C++**:
```cpp
FTransform GetTangentTransformAtEngineLocation(const FVector& EngineCoordinates);
FTransform GetTangentTransformAtGeographicLocation(const FGeographicCoordinates& GeographicCoordinates);
```

#### Ellipsoid Information

**Blueprint/C++**:
```cpp
double GetGeographicEllipsoidMaxRadius();
double GetGeographicEllipsoidMinRadius();
double GetProjectedEllipsoidMaxRadius();
double GetProjectedEllipsoidMinRadius();
```

#### Utility Functions

**Validate CRS String**:
```cpp
bool IsCRSStringValid(FString CRSString, FString& Error);
```

**Apply Runtime Changes**:
```cpp
void ApplySettings(); // Call after changing CRS or origin at runtime
```

### FGeographicCoordinates

Structure representing geographic coordinates.

**Properties**:
- `double Latitude`: Latitude in degrees (-90 to 90)
- `double Longitude`: Longitude in degrees (-180 to 180)
- `double Altitude`: Height above ellipsoid in meters

**Constructor**:
```cpp
FGeographicCoordinates(double InLongitude, double InLatitude, double InAltitude);
```

**Formatting**:
```cpp
FText ToFullText(int32 IntegralDigitsLatLon = 8, int32 IntegralDigitsAlti = 2, bool bAsDMS = false);
FText ToCompactText(int32 IntegralDigitsLatLon = 8, int32 IntegralDigitsAlti = 2, bool bAsDMS = false);
```

### ARoundPlanetPawn

Specialized pawn for planet-scale navigation.

**Key Features**:
- Automatic orientation adjustment to planet surface
- Smooth fly-to animations
- Dynamic speed scaling with altitude

**Blueprint Functions**:

**Fly To Location**:
```cpp
void FlyToLocationGeographic(const FGeographicCoordinates& GeographicDestination, double YawAtDestination, double PitchAtDestination, bool CanInterruptByMoving);
void FlyToLocationECEF(const FVector& ECEFDestination, double YawAtDestination, double PitchAtDestination, bool CanInterruptByMoving);
void InterruptFlyToLocation();
```

**Speed Control**:
```cpp
void IncreaseSpeedScalar();
void DecreaseSpeedScalar();
void ResetSpeedScalar();
```

**Properties**:
- `BaseSpeedKmh`: Base movement speed in km/h
- `SpeedScalar`: Speed multiplier
- `OrbitalMotion`: If true, movement is tangent to planet surface
- `HAT`: Height Above Terrain
- `Altitude`: Height above ellipsoid

### UGeoReferencingBFL

Blueprint Function Library for coordinate formatting.

**Functions**:
```cpp
static FText ToFullText(FVector& CartesianCoordinates, int32 IntegralDigits = 3);
static FText ToCompactText(FVector& CartesianCoordinates, int32 IntegralDigits = 3);
static void ToSeparateTexts(FVector& CartesianCoordinates, FText& OutX, FText& OutY, FText& OutZ, int32 IntegralDigits = 3);
```

## Blueprints Usage

### Creating a Location Marker System

This example shows how to place markers at specific geographic coordinates:

1. **Create a Blueprint Actor** (e.g., `BP_GeoMarker`)

2. **Add Variables**:
   - `Latitude` (double)
   - `Longitude` (double)
   - `Altitude` (double)

3. **In Construction Script**:
   ```
   Get GeoReferencingSystem
   → Make Geographic Coordinates (from Lat/Lon/Alt)
   → Geographic To Engine
   → Set Actor Location (result)
   ```

4. **Place Multiple Markers**:
   - Duplicate the actor in your level
   - Set different Latitude/Longitude values in Details panel
   - Each marker automatically positions itself correctly

### Displaying Coordinates

Create a HUD or widget that displays the player's current geographic position:

1. **Get Player Location**:
   ```
   Get Player Pawn
   → Get Actor Location
   ```

2. **Convert to Geographic**:
   ```
   Get GeoReferencingSystem
   → Engine To Geographic
   ```

3. **Format for Display**:
   ```
   Geographic Coordinates
   → To Full Text (or To Compact Text)
   → Set Text (on Text widget)
   ```

### Flying to a Location

Using the RoundPlanetPawn:

```
Event "Fly To Location"
→ Make Geographic Coordinates (target lat/lon/alt)
→ Fly To Location Geographic
    - Geographic Destination: target coordinates
    - Yaw At Destination: 0
    - Pitch At Destination: -45
    - Can Interrupt By Moving: true
```

## Examples

### Example 1: Setting Up a UTM Project

For a project in New York City:

1. **GeoReferencingSystem Settings**:
   - Planet Shape: `Flat Planet`
   - ProjectedCRS: `EPSG:32618` (UTM Zone 18N)
   - GeographicCRS: `EPSG:4326` (WGS84)
   - Origin Location In Projected CRS: `true`
   - Origin Projected Coordinates Easting: `580000` (Central Park area)
   - Origin Projected Coordinates Northing: `4507000`

2. **Place Actors**: Position actors using engine coordinates, they automatically map to UTM coordinates

### Example 2: Round Planet with Surface Origin

For a global flight simulator:

1. **GeoReferencingSystem Settings**:
   - Planet Shape: `Round Planet`
   - GeographicCRS: `EPSG:4326` (WGS84)
   - Origin At Planet Center: `false`
   - Origin Location In Projected CRS: `false`
   - Origin Latitude: `51.5074` (London)
   - Origin Longitude: `-0.1278`
   - Origin Altitude: `0`

2. **Use RoundPlanetPawn**: 
   - Drag `BP_RoundPlanetPawn` into level
   - Configure flight curves and speed settings
   - Set as player pawn

3. **Navigate**: Use WASD for movement, mouse for looking, scroll wheel for speed

### Example 3: Converting Real-World Data

Loading GIS data points:

```cpp
// In C++
void AMyActor::LoadGISPoints(const TArray<FGeographicCoordinates>& GISPoints)
{
    AGeoReferencingSystem* GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
    if (!GeoRefSystem) return;

    for (const FGeographicCoordinates& Point : GISPoints)
    {
        FVector EngineLocation;
        GeoRefSystem->GeographicToEngine(Point, EngineLocation);
        
        // Spawn actor at this location
        GetWorld()->SpawnActor<AMyMarker>(MarkerClass, EngineLocation, FRotator::ZeroRotator);
    }
}
```

### Example 4: Custom CRS Definition

Using a custom projected coordinate system:

```
ProjectedCRS: "+proj=tmerc +lat_0=0 +lon_0=15 +k=0.9996 +x_0=500000 +y_0=0 +datum=WGS84 +units=m +no_defs"
```

Or using WKT format:
```
ProjectedCRS: "PROJCS["Custom TM", GEOGCS["WGS 84", DATUM["WGS_1984", SPHEROID["WGS 84",6378137,298.257223563]], PRIMEM["Greenwich",0], UNIT["degree",0.0174532925199433]], PROJECTION["Transverse_Mercator"], PARAMETER["latitude_of_origin",0], PARAMETER["central_meridian",15], PARAMETER["scale_factor",0.9996], PARAMETER["false_easting",500000], PARAMETER["false_northing",0], UNIT["metre",1]]"
```

## Troubleshooting

### Common Issues

#### 1. "GeoReferencingSystem not found"

**Problem**: Blueprint can't find the GeoReferencingSystem.

**Solution**:
- Ensure the GeoReferencingSystem actor is placed in your level
- Check that the plugin is enabled in Edit → Plugins
- Verify you're using the correct World Context Object

#### 2. Coordinate Conversions Return Zero

**Problem**: All coordinate conversions return (0, 0, 0).

**Solution**:
- Verify your CRS strings are valid using `IsCRSStringValid`
- Check that the GeoReferencingSystem is properly initialized
- Ensure origin location is set correctly
- Call `ApplySettings()` if you changed settings at runtime

#### 3. Invalid CRS Error

**Problem**: Error message about invalid CRS when setting ProjectedCRS or GeographicCRS.

**Solution**:
- Use valid EPSG codes (e.g., `EPSG:4326`, `EPSG:32631`)
- For custom projections, verify your PROJ string syntax
- Check the Output Log for detailed error messages
- Consult https://epsg.io for valid EPSG codes

#### 4. Precision Issues at Large Scales

**Problem**: Objects appear to jitter or have positioning issues far from origin.

**Solution**:
- For Round Planet mode with origin at planet center, implement world origin rebasing
- Use World Composition or Origin Rebasing features
- Consider using origin on surface instead of at planet center
- Keep gameplay area within 10-20 km of origin for flat planet mode

#### 5. RoundPlanetPawn Not Orienting Correctly

**Problem**: Pawn doesn't rotate to follow planet surface.

**Solution**:
- Ensure GeoReferencingSystem is set to Round Planet mode
- Verify the pawn has a valid reference to GeoReferencingSystem
- Check that you're using `ARoundPlanetPawn`, not a standard pawn
- Adjust the `CacheUpdateThreshold` if needed

#### 6. Performance Issues with Coordinate Conversions

**Problem**: Frequent coordinate conversions causing performance drops.

**Solution**:
- Cache conversion results when possible
- Avoid converting every tick; use events or timers
- Use the appropriate coordinate system for your calculations
- Consider pre-converting static data at load time

### Debug Tips

1. **Enable Verbose Logging**:
   ```cpp
   // In DefaultEngine.ini
   [Core.Log]
   LogGeoReferencing=Verbose
   ```

2. **Validate CRS Before Use**:
   ```cpp
   FString ErrorMsg;
   if (!GeoRefSystem->IsCRSStringValid(YourCRSString, ErrorMsg))
   {
       UE_LOG(LogTemp, Error, TEXT("Invalid CRS: %s"), *ErrorMsg);
   }
   ```

3. **Check Ellipsoid Parameters**:
   ```cpp
   double MaxRadius = GeoRefSystem->GetGeographicEllipsoidMaxRadius();
   UE_LOG(LogTemp, Log, TEXT("Ellipsoid Max Radius: %f meters"), MaxRadius);
   ```

4. **Visualize Transformations**:
   - Use Debug Draw to visualize ENU vectors
   - Draw lines to show coordinate conversions
   - Display coordinate values in viewport using Print String

### Getting Help

- Check the Output Log for detailed error messages
- Review the Content Browser examples in `Content/Blueprints/`
- Consult PROJ documentation: https://proj.org
- Search for EPSG codes: https://epsg.io

## License

This plugin is based on Epic Games' GeoReferencing plugin and is governed by the Unreal® Engine End User License Agreement. See [LICENSE.md](LICENSE.md) for details.

### Third-Party Licenses

- **PROJ**: X/MIT License - https://proj.org
- **SQLite**: Public Domain - https://www.sqlite.org

## Additional Resources

- **EPSG Registry**: https://epsg.io - Find EPSG codes for coordinate systems
- **PROJ Documentation**: https://proj.org/operations/projections/index.html
- **Coordinate System Guide**: https://www.maptools.com/tutorials/map_projections
- **GIS Glossary**: https://support.esri.com/en/other-resources/gis-dictionary

---

**Version**: 1.0  
**Maintained by**: COVISART  
**Based on**: Epic Games GeoReferencing Plugin
