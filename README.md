## MenuCustomization SDK - Custom Menu DLL

### Overview

The **MenuCustomization** project is a C++/ImGui DLL template that allows resellers to build and upload their own custom in‑game menu UI to the ChairFBI ecosystem.  

This project only handles the menu rendering. The cheat logic, security and shared‑memory layout are handled by the main ChairFBI infrastructure.

### Key Features

- **Custom ImGui UI**: Full freedom to design your own layout, tabs, colors, fonts and widgets.
- **Multi‑game support**: Includes predefined settings structures for PUBG, Apex, Fortnite, MW3/BO6.
- **Shared‑memory compatible**: `CheatSettings` structures mirror the original cheat shared memory, so your menu updates the same values the real cheat uses.
- **Loader integration**: The DLL is loaded by the ChairFBI loader and wired to the rendering pipeline automatically.

---

### Development Setup

- **Requirements**
  - Visual Studio 2019 or newer
  - Windows 10/11 (x64)
  - C++17 or newer
  - DirectX 11 support (linked by the project)

- **Project configuration**
  - Open `MenuCustomization.sln`.
  - Select the configuration **Release | x64**.
  - Build the `MenuCustomization` project; this produces the DLL you will upload to the panel.

- **Important constraints**
  - Do **not** modify the contracts in `main.cpp`. This file wires the DLL to the loader:
    - It initializes ImGui.
    - It calls `InitMyMenu` once.
    - It calls `RenderMyMenu` every frame.
  - Your customization must be done in:
    - `InitMyMenu` (initialization for your menu).
    - `RenderMyMenu` (per‑frame rendering logic).
    - ImGui code (styles, widgets, helper functions).

---

### Customization Guide

#### Core files

- `MenuCustomization/GUI/GUI.h`  
  Declares:
  - `void InitMyMenu(ID3D11Device* g_pd3dDevice);`
  - `void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig);`

- `MenuCustomization/GUI/*.cpp`  
  Implement your actual menu logic and visuals here (tabs, windows, widgets).

- `MenuCustomization/Imgui/*`  
  Full ImGui implementation and helpers; you can freely edit this to change style, fonts, themes or to add your own ImGui utilities.

- `MenuCustomization/CheatSettings/*`  
  Static settings structures for each game (PUBG, Apex, Fortnite, MW3/BO6).  
  These must **match the original cheat shared‑memory layout** and should **not be structurally changed** (no adding/removing/reordering fields).

---

### InitMyMenu and RenderMyMenu contracts

#### InitMyMenu

```cpp
void InitMyMenu(ID3D11Device* g_pd3dDevice);
```

- Called **once**, after ImGui context creation and DirectX 11 setup.
- Typical responsibilities:
  - Load fonts, textures and icons using `g_pd3dDevice`.
  - Configure global ImGui style (colors, rounding, spacing).
  - Initialize any static menu resources.

The loader owns the Direct3D device; you must **not** destroy it.

#### RenderMyMenu

```cpp
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig);
```

`RenderMyMenu` is called **every frame** by `RenderMenu` in `main.cpp`:

```154:181:MenuCustomization/main.cpp
void RenderMenu(bool *ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS *Settings, CConfig *g_pConfig, float mouse_wheel)
{
    ...
    if (*ShowMenu == true)
        RenderMyMenu(ShowMenu, Settings, g_pConfig);
    ...
}
```

Inside `RenderMyMenu` you build the whole ImGui layout for the cheat:

- **`ShowMenu`**
  - Pointer to a boolean that controls menu visibility.
  - You can toggle it from inside the menu, for example when the user clicks a “Close” or “Hide” button:

    ```cpp
    if (ImGui::Button("Close menu"))
        *ShowMenu = false;
    ```

- **`Settings`**
  - Pointer to the **shared‑memory settings struct** of the real cheat for the current game.
  - Defined in `CheatSettings/Games/*.h` and must mirror exactly the structure used by the cheat.
  - You should **bind ImGui widgets directly to its fields**:

    ```cpp
    void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig)
    {
        // Example: aimbot toggle
        ImGui::Checkbox("Aimbot Enabled", &Settings->Aimbot.Enabled);

        // Example: FOV slider
        ImGui::SliderFloat("Aimbot FOV", &Settings->Aimbot.Fov, 0.0f, 30.0f);
    }
    ```

  - **Do not change the struct layout** in `CheatSettings` (no adding/removing/reordering fields).  
    You are expected only to **read/write the existing members**.

- **`g_pConfig`**
  - Pointer to the internal configuration system instance.
  - Passed and owned by the loader; do **not** reallocate, free or replace it.
  - Use it to load/save presets or apply default values (the exact API depends on the `CConfig` implementation used in your base project). A typical pattern:

    ```cpp
    if (ImGui::Button("Save Config"))
        g_pConfig->Save("default");

    if (ImGui::Button("Load Config"))
        g_pConfig->Load("default");
    ```

Putting it all together, `RenderMyMenu` is the only place where you:

- Draw your custom ImGui windows/tabs.
- Bind widgets to `Settings` fields.
- Hook config actions (save/load) through `g_pConfig`.

The loader takes care of sharing `Settings` with the real cheat and managing the `CConfig` lifetime.

---

### Build Instructions

1. Open `MenuCustomization.sln` in Visual Studio.
2. Select **Release | x64** configuration.
3. Build the `MenuCustomization` project.
4. After a successful build, take the generated DLL from the `x64/Release` output directory (same folder where Visual Studio places the project binaries).
5. This DLL is what you will upload to the reseller panel as your custom menu.

---

### Panel Integration / Usage

The MenuCustomization DLL is integrated through the reseller panel.

1. **Configure the cheat**
   - In the panel, open the cheat you want to use for menu testing.
   - Adjust the cheat settings as usual.

2. **Enable Custom Menu**
   - In the cheat configuration, enable the **“Custom Menu”** option.

3. **Upload the menu DLL**
   - Go to the **“Upload Menu”** section.
   - Upload the DLL you compiled from this project (`Release | x64` build).

4. **Use the Test Custom Menu cheat**
   - In the panel, there are special cheat entries whose name contains:  
     `Test Custom Menu - <GAME>`
     - Example: `Test Custom Menu - PUBG`
   - Assign/use one of these test cheats to inject **only the menu**, without requiring the real game to be running.
   - This is intended for **debug and visual testing** of your menu.

5. **Production usage**
   - Once your menu is stable, end‑users will see and interact with it when using the normal cheats.
   - The same `Settings` structures are used by both test and real cheats, so any value you tweak in the menu will map directly to the live cheat behavior.

---

### Best Practices & Limitations

- **Do not modify `CheatSettings` layouts**
  - Keep all game settings structs exactly as provided.
  - Only read or write existing fields.

- **Do not change core contracts**
  - Keep the signatures of:
    - `InitMyMenu(ID3D11Device* g_pd3dDevice)`
    - `RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig)`
    - `DllMain` in `main.cpp`
  - Do not block or sleep inside `RenderMyMenu`; it runs every frame.

- **ImGui and style**
  - Feel free to customize all ImGui styles, fonts and helper functions.
  - Prefer lightweight drawing per frame to keep rendering smooth.

---

### File Structure (simplified)

```text
MenuCustomization/
├── CheatSettings/
│   ├── Config.h                 # Global settings and shared types
│   └── Games/                   # Per‑game shared‑memory layouts
│       ├── Apex.h
│       ├── BO6.h
│       ├── Fortnite.h
│       ├── MW3.h
│       └── PUBG.h
├── GUI/
│   ├── GUI.h                    # Declares InitMyMenu / RenderMyMenu
│   ├── GUI.cpp and game UIs     # Implement your custom ImGui menu
│   └── ...                      # Extra GUI helpers/elements
├── Imgui/                       # ImGui library + platform/DX11 bindings
├── main.cpp                     # DLL entry point and render wiring (do not modify contracts)
└── MenuCustomization.vcxproj    # Visual Studio project
```

---

### License

This project is provided under a **commercial/internal license** for authorized ChairFBI resellers only.  
Unauthorized use, distribution or reverse engineering of this project is strictly prohibited.


