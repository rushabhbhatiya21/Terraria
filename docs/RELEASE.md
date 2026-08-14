# Release Guide

This guide explains how to produce an upload-ready production zip and publish it on GitHub Releases.

## What Gets Distributed

Release zip should contain:

- Terraframe.exe
- resources/

Do not commit build artifacts to git. Keep out/ ignored.

## Exact Build Commands (Windows PowerShell)

1. Remove old production build folder (important if cache was created with different flags):

```powershell
Remove-Item -Recurse -Force d:\dev\ter\out\build\prod
```

2. Configure production build:

```powershell
cmake -S d:/dev/ter -B d:/dev/ter/out/build/prod -DPRODUCTION_BUILD:BOOL=ON
```

3. Build and package zip:

```powershell
cmake --build d:/dev/ter/out/build/prod --config Release --target release_zip
```

4. Locate the generated zip:

```powershell
Get-Item d:/dev/ter/out/build/prod/Terraframe-0.1.0-windows.zip
```

## Expected Artifact Name

A file similar to this is expected:

- Terraframe-0.1.0-windows.zip

## Publish on GitHub Releases

1. Push source commits to GitHub (without out/ artifacts).
2. Open repository on GitHub.
3. Go to Releases.
4. Click Draft a new release.
5. Create/select tag (example: v0.1.0).
6. Set release title (example: Terraframe v0.1.0).
7. Upload the generated zip in Assets.
8. Click Publish release.

## Release Notes Template

Use this template for each GitHub release.

```md
## Terraframe vX.Y.Z

Release date: YYYY-MM-DD

### Summary
Short 2-4 sentence overview of what this release improves.

### New
- Feature or content addition 1
- Feature or content addition 2
- Feature or content addition 3

### Improvements
- Quality/performance/UX improvement 1
- Improvement 2

### Fixes
- Bug fix 1
- Bug fix 2

### Known Issues
- Known issue 1
- Known issue 2

### Upgrade Notes
- Any manual notes for players (optional)

### Assets
- Terraframe-0.1.0-windows.zip
```

### Example (short)

```md
## Terraframe v0.1.0

Release date: 2026-08-14

### Summary
First public packaged build of Terraframe with a production zip layout.

### New
- Added boss spawn keybind for testing.
- Added initial boss loot bundle.

### Improvements
- Re-enabled enemy auto-spawn and audio lifecycle.

### Fixes
- Save/load consistency fixes for map and entity reconstruction.

### Known Issues
- THRUST and CAST attack styles are still incomplete.

### Assets
- Terraframe-0.1.0-windows.zip
```

## Verification Before Publishing

1. Extract the zip into a clean folder.
2. Confirm Terraframe.exe and resources/ are siblings.
3. Launch Terraframe.exe and verify game starts correctly.
4. Check menu startup, audio, and basic gameplay interaction.
