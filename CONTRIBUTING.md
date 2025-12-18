# Contributing to GeoReferencing Plugin

Thank you for your interest in contributing to the GeoReferencing plugin! This document provides guidelines and information for contributors.

## Table of Contents
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Code Organization](#code-organization)
- [Coding Standards](#coding-standards)
- [Making Changes](#making-changes)
- [Testing](#testing)
- [Submitting Changes](#submitting-changes)
- [Reporting Issues](#reporting-issues)

## Getting Started

Before contributing, please:

1. **Review the Documentation**: Read the [README.md](README.md), [QUICKSTART.md](QUICKSTART.md), and [API.md](API.md)
2. **Check Existing Issues**: Look for existing issues or feature requests
3. **Discuss Major Changes**: Open an issue to discuss significant changes before implementing
4. **Follow the Guidelines**: Adhere to the coding standards and submission process

## Development Setup

### Prerequisites

- Unreal Engine 4.27+ or 5.0+
- Visual Studio 2019/2022 (Windows) or Xcode (macOS)
- Git
- Basic understanding of C++ and Unreal Engine plugin development

### Setting Up Your Development Environment

1. **Fork the Repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/GeoReferencing.git
   cd GeoReferencing
   ```

2. **Create a Test Project**
   - Create a new Unreal Engine C++ project
   - Copy the GeoReferencing folder to the project's `Plugins` directory
   - Generate project files and open in your IDE

3. **Build the Plugin**
   - In Unreal Editor: File → Refresh Visual Studio Project
   - Build the solution in your IDE
   - Enable the plugin in the editor

## Code Organization

```
GeoReferencing/
├── Content/                    # Blueprint assets and examples
│   ├── Blueprints/            # Example Blueprint actors
│   ├── Models/                # 3D models and materials
│   └── UtilityWidgets/        # UI widgets
├── Resources/                  # Plugin resources
│   ├── Icon128.png            # Plugin icon
│   └── PROJ/                  # PROJ database files
├── Source/
│   ├── GeoReferencing/        # Runtime module
│   │   ├── Private/           # Implementation files (.cpp)
│   │   └── Public/            # Header files (.h)
│   ├── GeoReferencingEditor/  # Editor module
│   │   ├── Private/
│   │   └── Public/
│   └── ThirdParty/            # Third-party libraries
│       └── vcpkg-installed/   # PROJ and SQLite
├── API.md                      # API documentation
├── QUICKSTART.md              # Quick start guide
├── README.md                  # Main documentation
└── GeoReferencing.uplugin     # Plugin descriptor
```

### Key Components

**Runtime Module** (`GeoReferencing/`):
- `GeoReferencingSystem.h/cpp`: Main coordinate transformation system
- `GeographicCoordinates.h/cpp`: Geographic coordinate structure
- `Ellipsoid.h/cpp`: Ellipsoid calculations
- `RoundPlanetPawn.h/cpp`: Specialized pawn for planet navigation
- `GeoReferencingBFL.h/cpp`: Blueprint function library
- `UFSProjSupport.h/cpp`: PROJ library integration

**Editor Module** (`GeoReferencingEditor/`):
- Editor-only functionality
- Blueprint library for editor tools

## Coding Standards

### General Guidelines

1. **Follow Unreal Engine Coding Standards**: https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/
2. **Use Epic's Naming Conventions**:
   - Classes: `AGeoReferencingSystem`, `UGeoReferencingBFL`
   - Functions: `PascalCase` - `GetGeoReferencingSystem()`
   - Variables: `PascalCase` - `ProjectedCRS`
   - Private members: No prefix - just use `private:` section
   - Boolean properties: `bOriginAtPlanetCenter`

3. **Code Style**:
   - Use tabs for indentation (Unreal standard)
   - Opening braces on same line for functions
   - Use `const` and `constexpr` where appropriate
   - Prefer `nullptr` over `NULL`

### Example Code Style

```cpp
void AGeoReferencingSystem::EngineToGeographic(const FVector& EngineCoordinates, FGeographicCoordinates& GeographicCoordinates)
{
    if (!IsValid(this))
    {
        UE_LOG(LogGeoReferencing, Error, TEXT("Invalid GeoReferencingSystem"));
        return;
    }
    
    // Implementation here
}
```

### Documentation

1. **Header Comments**: Use Unreal's documentation format
   ```cpp
   /**
    * Brief description of what this function does.
    * 
    * @param EngineCoordinates - Input coordinates in UE space
    * @param GeographicCoordinates - Output coordinates in geographic space
    */
   ```

2. **Blueprint Exposure**: Add appropriate metadata
   ```cpp
   UFUNCTION(BlueprintCallable, Category = "GeoReferencing|Transformations")
   void MyFunction();
   ```

3. **Property Metadata**: Use helpful editor metadata
   ```cpp
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeoReferencing", 
             meta = (ClampMin = "-90", ClampMax = "90"))
   double Latitude;
   ```

### Thread Safety

- GeoReferencingSystem is **not thread-safe**
- All functions must be called from the game thread
- Document any thread-safety considerations in comments

## Making Changes

### Before Starting

1. **Create a Branch**: Use descriptive branch names
   ```bash
   git checkout -b feature/add-new-crs-support
   git checkout -b bugfix/fix-ecef-conversion
   ```

2. **Keep Changes Focused**: One feature or fix per branch
3. **Write Tests**: Add or update tests for your changes
4. **Update Documentation**: Update relevant documentation files

### Development Workflow

1. **Make Your Changes**: Implement your feature or fix
2. **Test Thoroughly**: 
   - Test in both Flat and Round Planet modes
   - Test with different CRS configurations
   - Verify Blueprint nodes work correctly
   - Test on target platforms if possible

3. **Check for Regressions**: Ensure existing functionality still works
4. **Update Documentation**: 
   - Update README.md if needed
   - Update API.md for API changes
   - Add code examples if applicable

### Commit Messages

Use clear, descriptive commit messages:

```
Add support for custom ellipsoid definitions

- Allow users to define custom ellipsoid parameters
- Add validation for ellipsoid radii
- Update API documentation
- Add Blueprint nodes for custom ellipsoids
```

Format:
- First line: Brief summary (50 characters or less)
- Blank line
- Detailed description (wrapped at 72 characters)
- List specific changes with bullet points

## Testing

### Manual Testing

1. **Create Test Cases**: Test your changes in different scenarios
2. **Test Both Modes**: Verify in Flat Planet and Round Planet modes
3. **Test Edge Cases**: Try extreme values and boundary conditions
4. **Cross-Platform**: Test on different platforms if possible

### Test Checklist

- [ ] Flat Planet mode works correctly
- [ ] Round Planet mode works correctly
- [ ] Blueprint nodes are accessible and functional
- [ ] C++ API functions correctly
- [ ] No crashes or errors in Output Log
- [ ] Performance is acceptable
- [ ] Documentation is updated
- [ ] Examples work as described

### Example Test Scenarios

1. **Basic Coordinate Conversion**:
   - Convert a known location to geographic coordinates
   - Verify the result matches expected values
   - Convert back and verify round-trip accuracy

2. **Origin Configuration**:
   - Test with different origin locations
   - Verify conversions are correct relative to origin
   - Test runtime origin changes with ApplySettings()

3. **CRS Validation**:
   - Test valid EPSG codes
   - Test invalid CRS strings
   - Test custom PROJ strings

## Submitting Changes

### Pull Request Process

1. **Update Your Branch**:
   ```bash
   git fetch origin
   git rebase origin/main
   ```

2. **Push Your Changes**:
   ```bash
   git push origin feature/your-feature-name
   ```

3. **Create Pull Request**:
   - Go to GitHub and create a PR
   - Fill out the PR template
   - Link any related issues

4. **PR Description Should Include**:
   - Summary of changes
   - Motivation and context
   - Testing performed
   - Screenshots/videos if UI changes
   - Breaking changes (if any)

### PR Template

```markdown
## Description
Brief description of what this PR does

## Motivation and Context
Why is this change needed? What problem does it solve?

## Changes Made
- Change 1
- Change 2
- Change 3

## Testing Performed
- [ ] Tested in Flat Planet mode
- [ ] Tested in Round Planet mode
- [ ] Tested Blueprint nodes
- [ ] Tested C++ API
- [ ] Verified documentation updates

## Screenshots/Videos
(if applicable)

## Breaking Changes
List any breaking changes and migration steps

## Checklist
- [ ] Code follows the project's coding standards
- [ ] Documentation has been updated
- [ ] All tests pass
- [ ] No new warnings introduced
- [ ] Commit messages are clear
```

### Code Review

- Be responsive to feedback
- Make requested changes promptly
- Discuss disagreements respectfully
- Update documentation as needed

## Reporting Issues

### Before Reporting

1. **Search Existing Issues**: Check if the issue already exists
2. **Verify the Problem**: Ensure it's reproducible
3. **Check Documentation**: Verify you're using the plugin correctly

### Issue Template

```markdown
## Description
Clear description of the issue

## Steps to Reproduce
1. Step 1
2. Step 2
3. Step 3

## Expected Behavior
What you expected to happen

## Actual Behavior
What actually happened

## Environment
- Unreal Engine Version: 5.3
- Platform: Windows 11
- Plugin Version: 1.0
- CRS Configuration: EPSG:4326 / EPSG:32631
- Planet Mode: Flat Planet

## Output Log
```
Paste relevant log output here
```

## Screenshots
(if applicable)

## Additional Context
Any other relevant information
```

### Issue Categories

- **Bug Report**: Something isn't working correctly
- **Feature Request**: Suggestion for new functionality
- **Documentation**: Issues with documentation
- **Question**: General questions about usage
- **Performance**: Performance-related issues

## Additional Guidelines

### Third-Party Dependencies

- PROJ library is the main dependency
- Avoid adding new dependencies unless absolutely necessary
- Document any new dependencies thoroughly

### Platform Support

When adding features, consider:
- Windows, Linux, macOS
- Android, iOS
- Ensure changes work on all supported platforms

### Performance

- Profile performance-critical code
- Avoid unnecessary conversions
- Cache results when appropriate
- Document performance implications

### Backward Compatibility

- Maintain backward compatibility when possible
- Document breaking changes clearly
- Provide migration guides for breaking changes

## Getting Help

- **Documentation**: Read the docs first
- **Issues**: Search existing issues
- **Discussions**: Use GitHub Discussions for questions
- **Code Review**: Ask for clarification in PR comments

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (Unreal Engine EULA).

---

Thank you for contributing to the GeoReferencing plugin! Your contributions help make geospatial development in Unreal Engine better for everyone.
