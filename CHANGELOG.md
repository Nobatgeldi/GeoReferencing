# Changelog

All notable changes to the GeoReferencing plugin will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - Current Version

### Added
- Comprehensive coordinate transformation system supporting multiple CRS types
- Support for Engine, Geographic, Projected, and ECEF coordinate systems
- Bidirectional conversions between all coordinate system pairs
- Flat Planet mode for local/regional scale projects
- Round Planet mode for global/planetary scale projects
- AGeoReferencingSystem actor for managing all coordinate transformations
- FGeographicCoordinates structure for lat/lon/alt representation
- FEllipsoid structure for geodetic calculations
- ARoundPlanetPawn specialized pawn for planet-scale navigation
- Smooth "fly-to" location animations with customizable curves
- Dynamic speed adjustment based on altitude
- Height Above Terrain (HAT) tracking
- ENU (East-North-Up) vector calculations at any location
- Tangent transform calculations for surface-aligned objects
- EPSG code support for standard CRS definitions
- WKT (Well-Known Text) format support
- Custom PROJ string support
- CRS validation functionality
- Blueprint-accessible coordinate conversion nodes
- Blueprint function library for coordinate formatting
- Full C++ API for all functionality
- Platform support: Windows, Linux, macOS, iOS, Android
- PROJ library integration for professional-grade transformations
- SQLite database for CRS definitions

### Blueprint Support
- All coordinate conversion functions exposed to Blueprints
- Type-safe coordinate structures
- User-friendly node names with K2_ prefix system
- Blueprint Function Library for text formatting
- Geographic Coordinates type with full Blueprint support

### Documentation
- Comprehensive README.md with full feature documentation
- Quick Start Guide (QUICKSTART.md) for 5-minute setup
- Complete API reference documentation (API.md)
- Contributing guidelines (CONTRIBUTING.md)
- This changelog (CHANGELOG.md)
- Installation instructions
- Usage examples for common scenarios
- Troubleshooting guide
- Platform-specific notes

### Examples
- Location marker system example
- Coordinate display system example
- Geographic data import example
- Custom CRS configuration examples
- UTM project setup examples
- Global navigation examples

### Features

#### Coordinate Systems
- Engine coordinates (UE native)
- Geographic coordinates (Latitude/Longitude/Altitude)
- Projected coordinates (UTM, State Plane, etc.)
- ECEF coordinates (Earth-Centered, Earth-Fixed)

#### Planet Modes
- **Flat Planet**: For areas up to hundreds of kilometers
  - Uses projected coordinate systems
  - Ignores Earth's curvature
  - Optimal for local/regional projects
  
- **Round Planet**: For planetary-scale environments
  - Accounts for Earth's curvature
  - Uses ellipsoidal Earth model
  - Supports global navigation
  - Configurable origin (planet center or surface)

#### Navigation
- RoundPlanetPawn with automatic surface orientation
- Orbital and surface-relative movement modes
- Customizable flight curves
- Speed scaling based on altitude
- Mouse wheel speed control
- Smooth camera animations

#### Ellipsoid Support
- Automatic ellipsoid extraction from CRS
- Support for common models (WGS84, GRS80, etc.)
- Ellipsoid parameter queries
- Geodetic surface normal calculations

### Dependencies
- PROJ library (included)
- SQLiteCore plugin (auto-enabled)
- Multi-platform PROJ binaries included

### Configuration
- Flexible origin configuration
- Runtime CRS changes with ApplySettings()
- Config file support for CRS definitions
- Per-level GeoReferencingSystem instances

### Performance
- Efficient coordinate transformations
- Caching recommendations documented
- Thread safety notes provided
- Best practices guide included

## Future Enhancements (Planned)

### Potential Future Features
- [ ] Additional coordinate system presets
- [ ] Enhanced visualization tools
- [ ] Integration with common GIS data formats
- [ ] World composition integration helpers
- [ ] Advanced caching mechanisms
- [ ] Additional navigation pawns
- [ ] Performance optimizations
- [ ] Additional example content
- [ ] Video tutorials
- [ ] Enhanced editor UI

## Known Issues

### Current Limitations
- Coordinate transformations are not thread-safe (must use game thread)
- Very large distances from origin may require world origin rebasing
- PROJ library initialization requires valid database files

### Platform-Specific Notes
- All platforms currently supported as designed
- No known platform-specific issues

## Version History

### Version 1.0.0
- Initial release with comprehensive feature set
- Complete documentation
- Multi-platform support
- Blueprint and C++ API
- Example content included

## Breaking Changes

### None Yet
This is the initial documented version. Future breaking changes will be listed here.

## Migration Guides

### None Yet
Future versions with breaking changes will include migration guides here.

## Deprecations

### None Yet
Deprecated features will be listed here with migration paths.

## Credits

### Based On
- Epic Games' GeoReferencing plugin
- Enhanced with comprehensive documentation and examples

### Third-Party Libraries
- **PROJ** (X/MIT License): Cartographic projections library
- **SQLite** (Public Domain): Database engine for PROJ data

### Contributors
- See GitHub contributors list for all contributors

## Support

For support, please:
1. Check the documentation in README.md, QUICKSTART.md, and API.md
2. Review the troubleshooting section
3. Search existing GitHub issues
4. Create a new issue if needed

## License

This plugin is governed by the Unreal® Engine End User License Agreement.
See LICENSE.md for details.

---

**Maintained by**: COVISART  
**Based on**: Epic Games GeoReferencing Plugin  
**Last Updated**: December 2024
