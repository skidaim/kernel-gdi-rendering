# Kernel GDI Rendering example

## Fully-kernel CS2 cheat that:

* **Patches DWM (user-mode) from kernel** to suppress independent flip:
  * `COverlayContext::OverlaysEnabled()` → returns false
  * `COverlayContext::IsCandidateDirectFlipCompatbile(...)` → returns false
  * (Both are resolved via **dwmcore.pdb** and hot-patched in `dwmcore.dll` inside dwm.exe.)   
  * Result: Game's swapchain (or any for that matter) stays **Composed: Flip**, dwm composes and your GDI boxes show on top of the game (even in windowed fullscreen/fullscreen).
  * Make sure to NOT disable fullscreen optimizations if you want fullscreen, that will lead to FSE (Hardware: Legacy Flip) and DWM won't takeover.
* **Draws boxes with kernel GDI** (`NtGdiPatBlt`). 
* **Toggles drawing at runtime** with a key (XButton1) to switch between DirectFlip and Independent Flip.   
   
Expects `dwmcore.pdb` + `dwmcore.dll` from `System32`, and uses a custom PDB parser ([credits](https://gist.github.com/namazso/4bfafdb0233f72f5d13bfee825c203f7)). 
Temporarily spoofs the current thread as DWM’s Win32 thread to make win32k calls for drawing.
  
## A few caveats

1. **Mode “latch”:**
   If the game has already latched **Hardware: Independent Flip**, patching won’t change anything until DWM **re-evaluates** the path.
   A single alt tab (or any real occlusion, like an external overlay) makes it drop to **Composed: Flip** with the patches active.
   Example: Having Intel PresentMon running on CS2 window, even in windowed mode, will make it immediately switch (probably bc of capture, try with OBS)

3. **`OverlaysEnabled=false` yield “Always Composed”:**  
  Inspecting dwmcore.dll we find COverlayContext::CheckAndRecordOverlayCandidate:
  
  <img width="859" height="509" alt="image" src="https://github.com/user-attachments/assets/27a9e33d-af3e-4906-bf9e-08711a237940" />
  
  What I found was that by only patching OverlaysEnabled to always return false was enough to force my game window to `Composed: Flip` at all times, 
  while only patching IsCandidateDirectFlipCompatible didn't (it took the MPO route and back to independent flip).   
  
  BUT, when I set the registry key I mentioned above (without patching anything), then it does take the Hardware: Composed Flip route.    
  This behavior doesn't seem consistent to me.  
        
  At first glance I thought it was something like:   
     
  ```c
  if (... IsCandidateDirectFlipCompatbile() ...) // Hardware: Independent Flip path
  {
      // record DirectFlip candidate (calls CDirectFlipInfo::Init etc.)
  }
  else // Hardware Composed: Independent Flip path
  {
      if (OverlaysEnabled()){  // fall back: try overlays (IsCandidateOverlayCompatible etc.) or compose
          ...
      }
  }
  
  ```
     
  But obviously not since patching OverlaysEnabled also blocks the Hardware: Independent Flip path. 
    
