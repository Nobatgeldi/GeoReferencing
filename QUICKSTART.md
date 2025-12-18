# Quick Start Guide

This guide will get you up and running with the GeoReferencing plugin in 5 minutes.

## Installation

1. Copy the `GeoReferencing` folder to your project's `Plugins` directory
2. Restart Unreal Engine
3. Enable the plugin: **Edit → Plugins → Search "GeoReferencing" → Enable**
4. Restart when prompted

## Your First Georeferenced Scene

### Step 1: Add the GeoReferencingSystem (1 minute)

1. Open or create a level
2. Open **Window → Place Actors**
3. Search for "GeoReferencingSystem"
4. Drag it into your level

### Step 2: Configure Your Location (2 minutes)

Select the GeoReferencingSystem in the World Outliner and set these properties:

#### For a Small Local Area (City/Region)

**Example: New York City**
```
Planet Shape: Flat Planet
Projected CRS: EPSG:32618  (UTM Zone 18N)
Geographic CRS: EPSG:4326  (WGS84)
Origin Location In Projected CRS: true
Origin Projected Coordinates Easting: 583000
Origin Projected Coordinates Northing: 4507000
```

**Example: London, UK**
```
Planet Shape: Flat Planet
Projected CRS: EPSG:32630  (UTM Zone 30N)
Geographic CRS: EPSG:4326  (WGS84)
Origin Location In Projected CRS: true
Origin Projected Coordinates Easting: 699000
Origin Projected Coordinates Northing: 5710000
```

#### For a Planet-Scale Environment

```
Planet Shape: Round Planet
Geographic CRS: EPSG:4326  (WGS84)
Origin At Planet Center: false
Origin Location In Projected CRS: false
Origin Latitude: 0.0
Origin Longitude: 0.0
Origin Altitude: 0.0
```

### Step 3: Test It (2 minutes)

#### Option A: Display Your Location (Blueprint)

1. Create a new Blueprint (Actor or HUD)
2. Add this simple graph:

```
Event BeginPlay
→ Get Player Pawn
→ Get Actor Location
→ Get GeoReferencingSystem
→ Engine To Geographic
→ ToFullText (from Geographic Coordinates)
→ Print String
```

3. Run the game and see your geographic coordinates printed

#### Option B: Create a Location Marker (Blueprint)

1. Create a Blueprint Actor: `Content → Right-click → Blueprint Class → Actor`
2. Name it `BP_GeoMarker`
3. Add a Static Mesh component for visibility
4. Add these variables:
   - `Latitude` (double) = 40.7580 (example: Statue of Liberty)
   - `Longitude` (double) = -73.9855
   - `Altitude` (double) = 0

5. In the Construction Script:
```
Get GeoReferencingSystem
→ Make Geographic Coordinates (Longitude, Latitude, Altitude)
→ Geographic To Engine
→ Set Actor Location (result)
```

6. Drag multiple instances into your level
7. Change the Latitude/Longitude in each instance's Details panel
8. Watch them automatically position themselves!

## Common Coordinate Systems (EPSG Codes)

### United States
- **UTM Zone 10N** (West Coast): `EPSG:32610`
- **UTM Zone 13N** (Central): `EPSG:32613`
- **UTM Zone 18N** (East Coast): `EPSG:32618`

### Europe
- **UTM Zone 30N** (UK, Spain): `EPSG:32630`
- **UTM Zone 32N** (Germany, Italy): `EPSG:32632`
- **UTM Zone 35N** (Eastern Europe): `EPSG:32635`

### Asia
- **UTM Zone 51N** (Japan): `EPSG:32651`
- **UTM Zone 49N** (China East): `EPSG:32649`

### Global
- **WGS84 Geographic**: `EPSG:4326` (Lat/Lon)
- **Web Mercator**: `EPSG:3857` (Google Maps, OpenStreetMap)

**Find Your EPSG Code**: Visit https://epsg.io and search by location

## Next Steps

- **Read the full [README.md](README.md)** for complete documentation
- **Explore the example Blueprints** in `Content/Blueprints/`
- **Try the RoundPlanetPawn** for planet-scale navigation
- **Import real GIS data** using coordinate conversions

## Need Help?

- Check the **Output Log** for error messages
- Verify your EPSG codes at https://epsg.io
- Review the troubleshooting section in [README.md](README.md)

## Useful Snippets

### Get Current Geographic Location (C++)
```cpp
AGeoReferencingSystem* GeoRefSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
FGeographicCoordinates GeographicCoords;
GeoRefSystem->EngineToGeographic(GetActorLocation(), GeographicCoords);
```

### Convert Many Points at Once (C++)
```cpp
TArray<FVector> EngineLocations;
for (const FGeographicCoordinates& GeoCoord : MyGeoData)
{
    FVector EngineLocation;
    GeoRefSystem->GeographicToEngine(GeoCoord, EngineLocation);
    EngineLocations.Add(EngineLocation);
}
```

### Validate CRS String (C++)
```cpp
FString ErrorMsg;
if (!GeoRefSystem->IsCRSStringValid(TEXT("EPSG:4326"), ErrorMsg))
{
    UE_LOG(LogTemp, Error, TEXT("Invalid CRS: %s"), *ErrorMsg);
}
```

---

That's it! You now have a georeferenced Unreal Engine project. Happy mapping! 🗺️
