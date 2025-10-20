# OpenXRExtended

**OpenXRExtended** is an Unreal Engine plugin exposing advanced OpenXR features **without relying on third-party plugins**.  
It provides a clean **Blueprint** and **C++** API to enable, disable, and configure OpenXR extensions commonly used for VR/AR development — especially on **Meta Quest**, **PICO**, and other OpenXR-compatible headsets.

---

## Purpose

The plugin centralizes XR rendering and runtime controls that are typically spread across different vendor SDKs.  
It aims to:
- Simplify cross-headset development.
- Provide direct Blueprint access for quick iteration.
- Serve as a foundation for future OpenXR extensions.

---

## Exposed Functions

All functions are **static** and **BlueprintCallable**, accessible under the *XR | OpenXR* category.

### Rendering and Performance

| Function | Description |
|-----------|-------------|
| `SetSpaceWarpEnabled(bool bEnable)` | Enables or disables **Space Warp (ASW)** for time-warped extrapolated frames. |
| `SetXRRenderTargetScalePercent(int32 Percent)` | Sets the **render scale** in percentage (100 = native resolution). |
| `SetXRDynamicResolutionEnabled(bool bEnable)` | Enables or disables **dynamic resolution** scaling. |
| `SetVRSFoveation(EXRFoveationLevel Level, bool bGazeTracked=false, bool bPreviewMask=false)` | Configures **Variable Rate Shading (VRS)** foveation level with optional gaze tracking. |
| `SetFBFoveation(EXRFoveationLevel Level, bool bDynamic=false, float VerticalOffsetDeg=0.f)` | Configures **Fixed Foveated Rendering (FFR)**, compatible with Meta and PICO headsets. |

### Passthrough and Stereo Layers

| Function | Description |
|-----------|-------------|
| `SetPassthroughEnabled(bool bEnable, bool bAlsoSetStartupCVars=true)` | Enables passthrough (mixed reality) mode if available. |
| `SetStereoLayerEmulationEnabled(bool bEnable)` | Simulates stereo layers in non-VR rendering for debugging. |
| `SetStereoLayerLegacyFacingEnabled(bool bEnable)` | Enables legacy stereo layer orientation. |
| `SetStereoLayerEmulatedDepthEnabled(bool bEnable)` | Enables depth simulation for stereo layers. |
| `SetAlphaInvertPassEnabled(bool bEnable)` | Inverts the alpha pass, useful for custom blending. |

### Environment and Display

| Function | Description |
|-----------|-------------|
| `SetEnvironmentBlendMode(EXREnvironmentBlendMode Mode)` | Sets the **environment blend mode** (opaque, additive, alpha). |
| `SetHMDRefreshRateHz(float Hz)` | Attempts to change the HMD **display refresh rate** (Meta extension). |

---

## Installation

1. Copy the **OpenXRExtended/** folder into your project’s `Plugins/` directory.  
2. Enable the plugin in **Edit → Plugins → XR → OpenXRExtended**.  
3. Restart the Unreal Editor.  
4. Access the functions in Blueprints or through C++.

---

## Compatibility

- **Unreal Engine:** 5.7+  
- **Platforms:**  
  - Windows, Linux (OpenXR)  
  - Android (Meta Quest, PICO)  
- **Supported Headsets:**  
  - Meta Quest 2 / 3 / Pro  
  - PICO 4  
  - HP Reverb, Varjo, and other OpenXR devices

---

## TODO

- [ ] Expose the **MetaXR extension** to dynamically change HMD refresh rate (`XR_FB_display_refresh_rate`).  
- [ ] Document required **AndroidManifest.xml** modifications for certain OpenXR features (passthrough, dynamic foveation, etc.).  
- [ ] Add **OpenXR Toolkit Layer** integration for diagnostics and profile presets.

---

## Technical Notes

The library uses **static Blueprint-callable methods** to avoid dependency on runtime UObject instances.  
It relies on:
- `OpenXRHMD`
- `XRBase`
- `RHI`  
and uses runtime CVars to apply settings safely in **Shipping builds**.

---

## License (MIT)

Copyright (c) 2025 Yannick Comte

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
