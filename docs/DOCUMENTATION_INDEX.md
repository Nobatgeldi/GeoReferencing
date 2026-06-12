# GeoReferencing Plugin - Documentation Index

Welcome to the GeoReferencing plugin documentation. This index will help you find the information you need.

## Quick Links

- 🚀 **New User?** Start with [QUICKSTART.md](QUICKSTART.md) - Get up and running in 5 minutes
- 📖 **Full Guide** [README.md](README.md) - Complete documentation with all features
- 💻 **Developer?** See [API.md](API.md) - Full API reference for C++ and Blueprint
- 🗺️ **GeoJSON/GeoTIFF?** See [GEOJSON_LOADER.md](GEOJSON_LOADER.md) - C++ and Blueprint loading guide
- 🤝 **Contributing?** Read [CONTRIBUTING.md](CONTRIBUTING.md) - Guidelines for contributors
- 📋 **Version Info** [CHANGELOG.md](CHANGELOG.md) - Version history and changes

## Documentation Files

### For Users

#### [README.md](README.md) - Main Documentation (21 KB)
**Complete user guide with everything you need to use the plugin**

**Contents:**
- Plugin overview and key features
- System requirements
- Installation instructions (2 methods)
- Getting started guide
- Core concepts explained
- API reference summary
- Blueprint usage examples
- 4 detailed usage examples
- Comprehensive troubleshooting guide
- Additional resources and links

**Best For:**
- Understanding plugin capabilities
- Learning core concepts
- Finding usage examples
- Troubleshooting issues

#### [QUICKSTART.md](QUICKSTART.md) - Quick Start Guide (4.5 KB)
**Get started in 5 minutes with step-by-step instructions**

**Contents:**
- Fast installation steps
- Your first georeferenced scene (3 steps)
- Common EPSG codes for major regions
- Useful code snippets
- Next steps

**Best For:**
- First-time users
- Quick setup
- Testing the plugin
- Learning by doing

### For Developers

#### [API.md](API.md) - API Reference (21 KB)
**Complete technical reference for developers**

**Contents:**
- Core classes documentation
  - AGeoReferencingSystem (30+ methods)
  - FGeographicCoordinates
  - FEllipsoid
  - ARoundPlanetPawn
- All coordinate transformation methods
- ENU vectors and tangent transforms
- Blueprint function libraries
- 8 common usage patterns with code
- Performance considerations
- Thread safety notes
- Module dependencies

**Best For:**
- C++ developers
- Blueprint developers
- API integration
- Advanced usage patterns

#### [GEOJSON_LOADER.md](GEOJSON_LOADER.md) - GeoJSON & GeoTIFF Loading Guide
**Dedicated guide for importing and exporting GeoJSON data**

**Contents:**
- What is GeoJSON and supported geometry types
- UGeoJSONReader and UGeoJSONWriter class reference
- C++ code examples (load file, load string, export, full pipeline)
- Blueprint node-by-node examples with diagrams
- GeoJSON file format reference with samples
- Error handling patterns
- Coordinate order convention explained
- GeoTIFF status and planned Phase 4 features
- Frequently Asked Questions

**Best For:**
- Loading GIS data exported from QGIS or ArcGIS
- Placing actors from geographic coordinates
- Sending/receiving GeoJSON over HTTP
- C++ and Blueprint developers working with GeoJSON

#### [CONTRIBUTING.md](CONTRIBUTING.md) - Contributor Guide (11 KB)
**Everything you need to contribute to the project**

**Contents:**
- Development setup
- Code organization
- Coding standards and style
- Making changes workflow
- Testing guidelines
- Pull request process
- Issue reporting template

**Best For:**
- Contributors
- Code reviewers
- Maintainers
- Anyone submitting changes

### Project Management

#### [CHANGELOG.md](CHANGELOG.md) - Version History (5.9 KB)
**Track versions, features, and changes**

**Contents:**
- Version 1.0.0 features
- Future planned enhancements
- Known issues
- Breaking changes (none yet)
- Migration guides (for future versions)

**Best For:**
- Tracking changes
- Version planning
- Update information

#### [LICENSE.md](LICENSE.md) - License Information
**Legal information and usage terms**

**Contents:**
- Unreal Engine EULA reference
- Usage terms

## Documentation by Task

### I want to...

**Install the plugin**
→ [QUICKSTART.md](QUICKSTART.md) (5 min) or [README.md](README.md) Installation section

**Understand what the plugin does**
→ [README.md](README.md) Overview and Features sections

**Convert coordinates in Blueprint**
→ [README.md](README.md) Blueprints Usage section

**Convert coordinates in C++**
→ [API.md](API.md) Usage Patterns section

**Set up a UTM project**
→ [README.md](README.md) Example 1 or [QUICKSTART.md](QUICKSTART.md) Step 2

**Set up a global/planet-scale project**
→ [README.md](README.md) Example 2

**Use the RoundPlanetPawn**
→ [README.md](README.md) Navigation Features + [API.md](API.md) ARoundPlanetPawn

**Fix coordinate conversion issues**
→ [README.md](README.md) Troubleshooting section

**Load or export GeoJSON data in C++ or Blueprint**
→ [GEOJSON_LOADER.md](GEOJSON_LOADER.md) dedicated guide

**Understand coordinate systems**
→ [README.md](README.md) Core Concepts section

**Find EPSG codes**
→ [QUICKSTART.md](QUICKSTART.md) Common Coordinate Systems section

**Contribute code**
→ [CONTRIBUTING.md](CONTRIBUTING.md) complete guide

**Report a bug**
→ [CONTRIBUTING.md](CONTRIBUTING.md) Reporting Issues section

**See what's new**
→ [CHANGELOG.md](CHANGELOG.md) Version History

## Documentation Statistics

- **Total Files**: 7 documentation files
- **Total Lines**: ~2,600 lines
- **Total Size**: ~100 KB
- **Main Docs**: ~90 KB (README, API, QUICKSTART, GEOJSON_LOADER)
- **Contributing**: ~19 KB (CONTRIBUTING, CHANGELOG)

## Key Features Documented

✅ GeoJSON import/export with C++ and Blueprint examples  
✅ GeoTIFF loading status and roadmap  
✅ Complete coordinate transformation system  
✅ Multi-CRS support (Geographic, Projected, ECEF)  
✅ Flat and Round Planet modes  
✅ Blueprint and C++ APIs  
✅ RoundPlanetPawn navigation  
✅ ENU vectors and tangent transforms  
✅ EPSG code support  
✅ Installation and setup  
✅ Usage examples  
✅ Troubleshooting guide  
✅ API reference  
✅ Contributing guidelines  

## Getting Help

1. **Check Documentation**: Start with the relevant doc file above
2. **Search Issues**: Look for existing GitHub issues
3. **Troubleshooting**: See README.md troubleshooting section
4. **Output Log**: Check Unreal Engine Output Log for errors
5. **Create Issue**: Use the template in CONTRIBUTING.md

## Additional Resources

- **EPSG Registry**: https://epsg.io
- **PROJ Documentation**: https://proj.org
- **Unreal Engine Docs**: https://docs.unrealengine.com

---

**Last Updated**: December 2024  
**Plugin Version**: 1.0  
**Maintained by**: COVISART
