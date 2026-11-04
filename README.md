# GW2 Rotation Coach

**Current Version:** `v0.525`  
**Author:** `Oneleaf525`  
**Framework:** Raidcore Nexus

A real-time combat rotation trainer, benchmark pacing analyzer, and in-game action bar overlay for Guild Wars 2. Designed to help players learn, practice, and master raid, strike, and fractal rotations.

---

## Installation Guide (For Players & Testers)

### Prerequisites
* **Raidcore Nexus:** Make sure you have Raidcore Nexus installed. If not, grab the loader from [Raidcore Nexus](https://raidcore.gg/Nexus).

### Step-by-Step Installation
1. On GitHub, click the green **`<> Code`** button and select **Download ZIP**, or download the files from Releases.
2. Locate your Guild Wars 2 game directory:
   * **Steam:** `...\SteamLibrary\steamapps\common\Guild Wars 2\addons\`
   * **Default Standalone:** `C:\Program Files\Guild Wars 2\addons\`
3. Extract the downloaded package so that `Gw2RotationCoach.dll` and the `Gw2RotationCoach` folder are placed directly into `addons`:
   ```
   <Guild Wars 2>\addons\
       ├── Gw2RotationCoach.dll
       └── Gw2RotationCoach\
           ├── bench\      (270 benchmark rotation profiles)
           ├── img\        (Skill icon textures)
           └── skills\     (Skill mapping databases)
   ```
4. Start Guild Wars 2.
5. On your first launch, the **Master Control Panel** will automatically open on screen.

---

## Core Features & How to Use Them

### 1. Master Control Panel (`Alt + Shift + T`)
Your central dashboard for managing the addon:
* **Display & HUD Style:** Toggle between ribbon HUD modes, adjust icon sizes, lookahead count, and opacity.
* **Skills & Keybinds:** Configure your slot bindings.
* **Build & Benchmark:** View your active rotation, toggle auto-detection, and check network latency.
* **One-Click Display:** Click **Show All Windows** to open or hide all panels at once.

### 2. Real-Time Rotation Ribbon HUD
* **Floating Prompt Ribbon:** Floats above your screen showing the sequence of upcoming skills. Drag it anywhere that fits your setup.
* **3 Presentation Modes:**
  1. *Transparent Single Key:* Minimalist key badges with directional flow arrows.
  2. *Skill Icon Images:* Full high-resolution skill icons with keybind badges.
  3. *Action Bar Slot Overlay:* Action bar numbers and slot identifiers.
* **Live Cast Channel Bar:** Real-time progress bar displaying cast channel duration and remaining animation seconds.
* **Mistake Feedback:** Flashes red when you press the wrong skill or interrupt a cast, logging the error in the Mistake History window.

### 3. Action Bar Slot Highlighter (Direct In-Game Overlay)
* **Direct Skill Highlights:** Draws glowing borders directly over your in-game action bars to indicate which skill to cast next.
* **Fully Decoupled Controls:** Weapon bar (`1`–`5`), Utility bar (`6`–`0`), and Profession mechanics (`F1`–`F5`) have completely independent horizontal and vertical adjustments. Moving one bar never affects the others.
* **UI Scale Presets:** One-click presets for **Small (36px)**, **Normal (48px)**, and **Large (54px)** UI scaling.
* **Diagnostic Alignment Grid:** Check `Highlight All Action Bar Slots` to reveal drag handles for pixel-perfect HUD alignment.

### 4. Dynamic Live Ping Auto-Detection
* Automatically detects the active game connection on TCP port `6112` and continuously measures real-time network latency.
* Calibrates rotation pacing dynamically against your connection speed.
* Can be toggled to a manual slider in Master Control if you prefer a fixed ping value.

### 5. Benchmark Pacing Metrics (85% & 95% Targets)
* **Execution Speed Ratio:** Displays your cast speed as a percentage of top benchmark pacing.
* **Deficit Percentages:** Tells you exactly how much slower you are relative to both the **85%** and **95%** benchmark targets (e.g., `+18.5% slower than 95% target`).
* **On-Pace Status:** Displays green confirmation when you match or beat the benchmark speed.

### 6. Build & Gear Showcase Window
* **270 Benchmark Builds:** Search and select builds across all 9 professions and elite specializations.
* **Clear Weapon Disambiguation:** Differentiates offhand weapon variants (e.g., `Condition Virtuoso (Dagger & Sword / Focus)` vs. `Condition Virtuoso (Dagger & Sword / Pistol)`).
* **Copy Build Template:** One-click button copies the in-game chat link (`[&DQcBHR...]`) to your clipboard. Paste it directly into your game Build tab.
* **Copy Guide URL:** One-click button copies the benchmark build guide URL.
* **Equipment Loadout:** Displays recommended stats, runes, sigils, relics, food, and utility consumables.

### 7. Keybind Setup & Mouse Button Support
* Open Master Control $\rightarrow$ **Skills & Keybinds**.
* Click **Bind** next to any slot and press your desired key.
* Use the quick-attach buttons (`+M3`, `+M4`, `+M5`) to instantly assign mouse buttons.
* **Auto-Import:** Automatically detects exported keybind XML files from `Documents\Guild Wars 2\InputBinds\`.

---

## Default Hotkeys & Shortcuts

* `Alt + Shift + T`: Toggle Master Control Panel
* `Alt + Shift + R`: Reset Rotation Sequence to the Beginning
* `Escape`: Closes open addon windows (can be reopened via hotkey or Nexus menu)

---

## Troubleshooting

* **Addon window does not appear:** Press `Alt + Shift + T`. If it doesn't open, verify that `Gw2RotationCoach.dll` is in `<Guild Wars 2>\addons\` and enabled in the Nexus Addon Manager (`~` or Nexus top bar icon).
* **Skill highlights don't line up with my action bar:** Open Master Control $\rightarrow$ check `Highlight All Action Bar Slots (Diagnostic / Alignment Grid)`. Use the drag handles or the Developer Controls sliders to position the boxes over your skills, then uncheck the box.
* **HUD displays the wrong skills:** Make sure `Auto-Detect Character & Build` is enabled in Master Control, or manually select your build from the Build & Gear Showcase.
* **Keybind badges don't match my keys:** Open Master Control $\rightarrow$ `Skills & Keybinds` and assign your keys, or click `+M4`/`+M5` for mouse bindings.
