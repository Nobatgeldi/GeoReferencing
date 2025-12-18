# GeoReferencing for Unreal Engine

Build geospatially accurate worlds in Unreal Engine with the GeoReferencing plugin. This page combines the essentials from the main documentation and quick start guide so you can install the plugin and verify it in just a few minutes.

## Quick Start (5 minutes)

1. **Install the plugin**
   - Copy the `GeoReferencing` folder into your project's `Plugins` directory.
   - Restart Unreal Engine, then enable **Edit → Plugins → GeoReferencing** and restart when prompted.
2. **Add the GeoReferencingSystem**
   - Open or create a level.
   - In **Window → Place Actors**, search for **GeoReferencingSystem** and drag it into the level (one per level).
3. **Configure your location**
   - Choose **Planet Shape**: `Flat Planet` for local areas, `Round Planet` for planet-scale.
   - Set CRS values:
     - Flat planet example (New York City): `Projected CRS: EPSG:32618`, `Geographic CRS: EPSG:4326`, `Origin Projected Coordinates Easting: 583000`, `Origin Projected Coordinates Northing: 4507000`.
     - Round planet example: `Geographic CRS: EPSG:4326`, `Origin At Planet Center: false`, `Origin Latitude: 0`, `Longitude: 0`, `Altitude: 0`.
4. **Test conversions**
   - Blueprint flow: `BeginPlay → Get Player Pawn → Get Actor Location → Get GeoReferencingSystem → Engine To Geographic → Print String`.
   - For markers, convert Geographic → Engine in Construction Script and set actor location.

## Installation (details)

From the README:
1. Download or clone the repository.
2. Copy the `GeoReferencing` folder to your project's `Plugins` directory (or to `Engine/Plugins` for an engine-wide install).
3. Restart the editor and enable the plugin under **Edit → Plugins**.

## Key Features

- Bidirectional conversions between engine, geographic, projected (e.g., UTM), and ECEF coordinates.
- EPSG codes, WKT, and PROJ string support.
- Flat and round planet modes with ENU vectors and tangent transforms.
- Blueprint-friendly APIs and the RoundPlanetPawn for planet-scale navigation.

## Helpful Resources

- Full guide: [README.md](https://github.com/Nobatgeldi/GeoReferencing/blob/main/README.md)
- Quick start: [QUICKSTART.md](https://github.com/Nobatgeldi/GeoReferencing/blob/main/QUICKSTART.md)
- Documentation index: [DOCUMENTATION_INDEX.md](https://github.com/Nobatgeldi/GeoReferencing/blob/main/DOCUMENTATION_INDEX.md)
- API reference: [API.md](https://github.com/Nobatgeldi/GeoReferencing/blob/main/API.md)
