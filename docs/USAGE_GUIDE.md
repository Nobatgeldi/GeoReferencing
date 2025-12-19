# Phase 1-3 Features Usage Guide

**Quick Reference Guide for Phase 1-3 Implemented Features**

---

## Quick Links

- **Complete API**: See [IMPLEMENTED_FEATURES.md](IMPLEMENTED_FEATURES.md)
- **API Reference**: See [../API.md](../API.md)
- **Quick Start**: See [../QUICKSTART.md](../QUICKSTART.md)

---

## At a Glance: What's New

| Phase | Feature | What It Does | When to Use |
|-------|---------|--------------|-------------|
| **1** | Accuracy Validation | Query transformation precision | Mission-critical applications |
| **1** | Error Handling | Get detailed error messages | Production environments |
| **1** | Edge Case Fixes | Handle poles & antimeridian | Global coverage |
| **2** | Batch API | Transform 1000s of points fast | Large datasets |
| **2** | Parallel Processing | Use multiple CPU cores | 10K+ points |
| **2** | Performance Stats | Monitor transformation speed | Optimization |
| **3** | GeoJSON Import | Load GIS data | QGIS/ArcGIS workflows |
| **3** | GeoJSON Export | Save for GIS analysis | Data exchange |
| **3** | Precision Calculator | Know when to rebase | Large worlds |

---

## 5-Minute Usage Examples

### Example 1: Safe Coordinate Transformation

**Blueprint**:
1. Add "Geographic To Engine Safe" node
2. Connect geographic coords input
3. Branch on success/failure
4. Use error message if failed

**C++**:
```cpp
FGeoReferencingError Error;
FVector EngineCoords;

if (GeoSystem->GeographicToEngineSafe(GeoCoords, EngineCoords, Error))
{
    SpawnActor(EngineCoords);
}
else
{
    ShowErrorDialog(Error.ErrorMessage);
}
```

### Example 2: Import 1000 Points from GeoJSON

**Blueprint**:
1. Use "Load Geo JSON File" node
2. Provide file path (e.g., "C:/Data/locations.geojson")
3. Get arrays of coordinates and properties
4. Loop through and spawn actors

**C++**:
```cpp
TArray<FGeographicCoordinates> Points;
TArray<FString> Properties;

if (UGeoJSONReader::LoadGeoJSONFile(FilePath, Points, Properties))
{
    TArray<FVector> EnginePoints;
    GeoSystem->GeographicToEngineBatch(Points, EnginePoints);
    
    for (int32 i = 0; i < EnginePoints.Num(); ++i)
    {
        SpawnMarker(EnginePoints[i], Properties[i]);
    }
}
```

### Example 3: Monitor World Precision

**Blueprint**:
1. On Player Tick
2. Get Actor Location
3. Use "Get Precision At Location"
4. Branch on "Requires Rebasing"
5. Show warning if true

**C++**:
```cpp
void AMyCharacter::Tick(float DeltaTime)
{
    if (GeoSystem->ShouldRebaseAtLocation(GetActorLocation()))
    {
        ShowRebasingWarning();
    }
}
```

### Example 4: Process 1 Million Points Efficiently

**C++ Only** (Blueprint can use single-threaded batch):
```cpp
TArray<FGeographicCoordinates> HugeDataset;  // 1M points
TArray<FVector> Results;

// Single-threaded: ~1 second
GeoSystem->GeographicToEngineBatch(HugeDataset, Results);

// Multi-threaded: ~150 ms (8 threads)
GeoSystem->GeographicToEngineBatchParallel(HugeDataset, Results, 8);
```

---

## Common Workflows

### Workflow 1: Import GIS Data → Place Actors

```
1. Export data from QGIS/ArcGIS as GeoJSON
2. Use UGeoJSONReader::LoadGeoJSONFile()
3. Use GeographicToEngineBatch() to transform
4. Loop and spawn actors at positions
```

**Blueprint Flow**:
```
Load Geo JSON File
  → Geographic To Engine Batch
    → For Each Loop
      → Spawn Actor from Class
```

### Workflow 2: Collect Actor Positions → Export for Analysis

```
1. Get all actors of class
2. Convert positions to geographic (EngineToGeographicBatch)
3. Create properties array with actor metadata
4. Use UGeoJSONWriter::SaveGeoJSONFile()
5. Open in QGIS/ArcGIS for analysis
```

### Workflow 3: Real-time Precision Monitoring

```
1. Add precision check to player tick
2. Get precision at player location
3. Display HUD warning when precision degrades
4. Trigger world origin rebasing when critical
```

### Workflow 4: Optimized Data Loading

```
1. Load large dataset from database/file
2. Use GeographicToEngineBatchParallel() if >1000 points
3. Monitor with GetPerformanceStats()
4. Optimize thread count based on performance
```

---

## Decision Tree: Which API to Use?

### For Single Transformation:

**Need error details?**
- Yes → `GeographicToEngineSafe()` (Blueprint) or `TryGeographicToEngine()` (C++)
- No, simple use → `GeographicToEngine()` (original)

**Need accuracy info?**
- Yes → `GeographicToEngineWithAccuracy()`

### For Multiple Transformations:

**How many points?**
- < 100 → Use loop with single transforms
- 100-1000 → Use `GeographicToEngineBatch()`
- > 1000 → Use `GeographicToEngineBatchParallel()` (C++)

### For GIS Data:

**File format?**
- GeoJSON → Use `UGeoJSONReader` / `UGeoJSONWriter`
- Shapefile → Wait for Phase 4
- GeoTIFF → Wait for Phase 4

---

## Performance Optimization Tips

### 1. Batch Everything

**❌ Don't**:
```cpp
for (auto& Geo : Points)
{
    FVector Engine;
    GeoSystem->GeographicToEngine(Geo, Engine);
    Results.Add(Engine);
}
```

**✅ Do**:
```cpp
TArray<FVector> Results;
GeoSystem->GeographicToEngineBatch(Points, Results);
```

**Speedup**: 3x-5x faster

### 2. Use Parallel for Large Datasets

**❌ Don't** (single-threaded batch):
```cpp
GeoSystem->GeographicToEngineBatch(HugeDataset, Results);  // 1 second
```

**✅ Do** (multi-threaded):
```cpp
GeoSystem->GeographicToEngineBatchParallel(HugeDataset, Results, 8);  // 150ms
```

**Speedup**: 10x-15x faster

### 3. Monitor Performance

```cpp
#if !UE_BUILD_SHIPPING
    FGeoReferencingStats Stats = GeoSystem->GetPerformanceStats();
    UE_LOG(LogTemp, Log, TEXT("Avg: %.2f μs, Max: %.2f μs"), 
        Stats.AverageTransformTimeMicroseconds,
        Stats.MaxTransformTimeMicroseconds);
#endif
```

### 4. Pre-allocate When Possible

The batch APIs already pre-allocate internally, but ensure your input arrays are properly sized:

```cpp
TArray<FGeographicCoordinates> Points;
Points.Reserve(ExpectedCount);  // Pre-allocate
```

---

## Error Handling Patterns

### Pattern 1: User-Friendly Error Display

```cpp
FGeoReferencingError Error;
if (!GeoSystem->GeographicToEngineSafe(UserInput, Result, Error))
{
    FText ErrorText = FText::FromString(Error.ErrorMessage);
    ShowMessageBox(TEXT("Coordinate Error"), ErrorText);
}
```

### Pattern 2: Validation Before Processing

```cpp
bool ValidateCoordinate(const FGeographicCoordinates& Geo)
{
    if (Geo.Latitude < -90 || Geo.Latitude > 90) return false;
    if (Geo.Longitude < -180 || Geo.Longitude > 180) return false;
    return true;
}

if (ValidateCoordinate(Input))
{
    GeoSystem->GeographicToEngine(Input, Output);
}
```

### Pattern 3: Fallback Strategies

```cpp
FVector Result;
FString Error;

if (GeoSystem->TryGeographicToEngine(Geo, Result, &Error))
{
    return Result;
}
else
{
    UE_LOG(LogTemp, Warning, TEXT("Using default location. Error: %s"), *Error);
    return DefaultLocation;
}
```

---

## Precision Management Patterns

### Pattern 1: Continuous Monitoring

```cpp
class AMyGameMode : public AGameMode
{
    FTimerHandle PrecisionCheckTimer;
    
    void BeginPlay()
    {
        GetWorldTimerManager().SetTimer(
            PrecisionCheckTimer,
            this,
            &AMyGameMode::CheckPrecision,
            5.0f,   // Every 5 seconds
            true    // Loop
        );
    }
    
    void CheckPrecision()
    {
        FVector PlayerLoc = GetFirstPlayerController()->GetPawn()->GetActorLocation();
        FCoordinatePrecision Precision = GeoSystem->GetPrecisionAtLocation(PlayerLoc);
        
        if (Precision.bRequiresRebasing)
        {
            NotifyPlayer(Precision.Recommendation);
        }
    }
};
```

### Pattern 2: Distance-Based Rebasing

```cpp
void CheckRebasingNeed()
{
    double MaxDistance = GeoSystem->GetRecommendedRebasingDistanceKm();
    FVector PlayerLoc = GetPlayerLocation();
    double DistanceKm = PlayerLoc.Size() / 100000.0;  // cm to km
    
    if (DistanceKm > MaxDistance * 0.8)  // 80% threshold
    {
        PrepareForRebasing();
    }
}
```

### Pattern 3: HUD Display

```cpp
void UpdatePrecisionHUD()
{
    FCoordinatePrecision Precision = GeoSystem->GetPrecisionAtLocation(CameraLocation);
    
    FLinearColor Color = FLinearColor::Green;
    if (Precision.PrecisionCentimeters > 10) Color = FLinearColor::Yellow;
    if (Precision.PrecisionCentimeters > 50) Color = FLinearColor::Red;
    
    DrawText(
        FString::Printf(TEXT("Precision: %.1f cm"), Precision.PrecisionCentimeters),
        Color
    );
}
```

---

## GeoJSON Workflow Patterns

### Pattern 1: Import → Process → Export

```cpp
void ProcessGISData()
{
    // Import
    TArray<FGeographicCoordinates> Points;
    TArray<FString> Properties;
    UGeoJSONReader::LoadGeoJSONFile(TEXT("input.geojson"), Points, Properties);
    
    // Process
    TArray<FVector> EnginePoints;
    GeoSystem->GeographicToEngineBatch(Points, EnginePoints);
    
    for (int32 i = 0; i < EnginePoints.Num(); ++i)
    {
        ProcessLocation(EnginePoints[i], Properties[i]);
        // Modify Properties[i] based on processing
    }
    
    // Export
    UGeoJSONWriter::SaveGeoJSONFile(TEXT("output.geojson"), Points, Properties);
}
```

### Pattern 2: Actor Collection for GIS Analysis

```cpp
void ExportActorsForAnalysis()
{
    TArray<FGeographicCoordinates> Locations;
    TArray<FString> Properties;
    
    for (AActor* Actor : GetAllActorsOfClass(AMyActorClass::StaticClass()))
    {
        FGeographicCoordinates Geo;
        GeoSystem->EngineToGeographic(Actor->GetActorLocation(), Geo);
        Locations.Add(Geo);
        
        FString Props = FString::Printf(
            TEXT("{\"name\":\"%s\",\"health\":%d,\"type\":\"%s\"}"),
            *Actor->GetName(),
            Cast<AMyActorClass>(Actor)->Health,
            *Actor->GetClass()->GetName()
        );
        Properties.Add(Props);
    }
    
    UGeoJSONWriter::SaveGeoJSONFile(TEXT("actors.geojson"), Locations, Properties);
}
```

### Pattern 3: Real-time Data Streaming

```cpp
void UpdateGeoJSONFeed()
{
    TArray<FGeographicCoordinates> CurrentPositions;
    TArray<FString> PlayerData;
    
    for (APlayerState* Player : GetPlayerStates())
    {
        FGeographicCoordinates Geo;
        GeoSystem->EngineToGeographic(Player->GetPawn()->GetActorLocation(), Geo);
        CurrentPositions.Add(Geo);
        
        FString Data = FString::Printf(
            TEXT("{\"player\":\"%s\",\"timestamp\":%lld}"),
            *Player->GetPlayerName(),
            FDateTime::Now().ToUnixTimestamp()
        );
        PlayerData.Add(Data);
    }
    
    FString GeoJSON = UGeoJSONWriter::ExportToGeoJSONString(CurrentPositions, PlayerData);
    SendToWebService(GeoJSON);
}
```

---

## Blueprint Examples

### Example 1: Safe Transform with Error Display

**Nodes**:
1. `Geographic To Engine Safe`
2. `Branch` (on return value)
3. True: `Spawn Actor`
4. False: `Print String` (Error Message from struct)

### Example 2: Batch Import from GeoJSON

**Nodes**:
1. `Load Geo JSON File`
2. `Geographic To Engine Batch`
3. `For Each Loop` (on Engine Coordinates array)
4. `Spawn Actor from Class` (in loop body)

### Example 3: Precision Warning

**Nodes**:
1. `Event Tick`
2. `Get Actor Location`
3. `Get Precision At Location`
4. `Branch` (on "Requires Rebasing")
5. True: `Create Widget` + `Add to Viewport`

### Example 4: Performance Monitoring

**Nodes**:
1. After batch operation
2. `Get Performance Stats`
3. `Break Struct` (FGeoReferencingStats)
4. `Print String` (Total Transformations, Average Time)

---

## Troubleshooting

### Issue: "Transform failed: Invalid latitude"

**Solution**: Validate input before transformation
```cpp
if (Lat < -90 || Lat > 90) { /* reject */ }
```

### Issue: Slow batch processing

**Solution**: Use parallel version for large datasets
```cpp
if (Points.Num() > 1000)
{
    GeographicToEngineBatchParallel(Points, Results, 8);
}
```

### Issue: GeoJSON won't load

**Solution**: Validate GeoJSON format (RFC 7946)
- Coordinate order: [longitude, latitude, altitude]
- Check JSON syntax with online validator
- Ensure file path is absolute

### Issue: Precision degrading

**Solution**: Check distance from origin
```cpp
FCoordinatePrecision P = GetPrecisionAtLocation(Loc);
if (P.DistanceFromOriginKm > 1000) { /* rebase */ }
```

---

## Next Steps

1. **Read Full Documentation**: [IMPLEMENTED_FEATURES.md](IMPLEMENTED_FEATURES.md)
2. **Explore API**: [../API.md](../API.md)
3. **Try Examples**: Start with GeoJSON import/export
4. **Optimize**: Profile with performance stats
5. **Plan Ahead**: Review [Phase 4 Analysis](PHASE4_DETAILED_ANALYSIS.md)

---

**Questions?** File an issue on GitHub or check the full documentation.
