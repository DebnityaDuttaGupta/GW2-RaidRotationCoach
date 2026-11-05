# GW2 Rotation Coach

**Current Version:** `v0.526`  
**Author:** `Oneleaf525`  
**Framework:** Raidcore Nexus

A real-time combat rotation trainer, benchmark pacing analyzer, and in-game action bar overlay for Guild Wars 2. Designed to help players learn, practice, and master raid, strike, and fractal rotations.

---

## Installation Guide (For Players & Testers)

### Prerequisites
* **Raidcore Nexus:** Make sure you have Raidcore Nexus installed. If not, grab the loader from [Raidcore Nexus](https://raidcore.gg/Nexus).

### Step-by-Step Installation
1. Download the release package (`Gw2RotationCoach-v0.526.zip` or the files from the [Releases](https://github.com/DebnityaDuttaGupta/GW2-RaidRotationCoach/releases) page).
2. Locate your Guild Wars 2 game directory:
   * **Steam:** `...\SteamLibrary\steamapps\common\Guild Wars 2\addons\`
   * **Default Standalone:** `C:\Program Files\Guild Wars 2\addons\`
3. Extract the downloaded package so that `Gw2RotationCoach.dll` and the `Gw2RotationCoach` folder are placed directly into `addons`:
   ```
   <Guild Wars 2>\addons\
       ├── Gw2RotationCoach.dll
       └── Gw2RotationCoach\
           ├── bench\      (271 benchmark rotation profiles)
           ├── img\        (Skill icon textures)
           └── skills\     (Skill mapping databases)
   ```
   > [!TIP]
   > The addon features automatic multi-path discovery and will scan `addons/Gw2RotationCoach`, the game root, and the DLL's folder. If the assets folder is ever misplaced, the in-game Build Showcase window will display a helpful path banner with an **Open Addons Folder** button.
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
* **Live Cast Channel Bar:** Real-time progress bar displaying cast channel duration and remaining animation seconds. Instant skills (0 ms) cast immediately without false channel lockouts.
* **Mistake Feedback:** Flashes red when you press the wrong skill or interrupt a cast, logging the error in the Mistake History window.

### 3. Action Bar Slot Highlighter (Direct In-Game Overlay)
* **Direct Skill Highlights:** Draws glowing borders directly over your in-game action bars to indicate which skill to cast next.
* **Fully Decoupled Controls:** Weapon bar (`1`–`5`), Utility bar (`6`–`0`), and Profession mechanics (`F1`–`F5`) have completely independent horizontal and vertical adjustments. Moving one bar never affects the others.
* **UI Scale Presets:** One-click presets for **Small (36px)**, **Normal (48px)**, and **Large (54px)** UI scaling.
* **Diagnostic Alignment Grid:** Check `Highlight All Action Bar Slots` to reveal drag handles for pixel-perfect HUD alignment.

### 4. Dynamic Live Ping Auto-Detection
* Automatically detects the active game connection on TCP port `6112` and continuously measures real-time network latency.
* Dynamically adapts benchmark queue window thresholds to your actual connection latency.

### 5. Dual-Percentage Benchmark Pacing Metrics
* Tracks your execution speed and cast gap timing in real time.
* Displays pacing and queue deficits directly in percentages relative to official **85%** and **95%** raid benchmark targets.

### 6. Full Profession Cooldown Reset System
* Accurate real-time cooldown tracking powered by the 2,300+ skill database.
* Comprehensive instant-reset mechanics across all 9 professions:
  * **Guardian:** *Renewed Focus* instantly recharges all virtues and tome chapters.
  * **Warrior:** *Berserk* instantly resets primal bursts.
  * **Elementalist:** *Fresh Air* instantly refreshes Air Attunement; *Weave Self* reduces attunement cooldowns.
  * **Ranger:** Pet swaps and Soulbeast beastmode transitions.
  * **Revenant:** Legend stance swaps resetting energy and cooldowns.
  * **Thief:** *Deadeye's Mark* refresh on defeat.
  * **Mesmer:** *Signet of the Ether* resets all phantasms; *Signet of Illusions* resets all shatters and bladesongs.

### 7. Build & Gear Showcase
* Browse **271 benchmark profiles** across all 9 professions and all 27 Elite Specializations.
* Complete weapon loadout disambiguation (e.g. *Dagger & Sword / Focus* vs. *Dagger & Sword / Pistol*).
* One-click copying of in-game Build Template Chat Codes and Snow Crows guide links.
* Automatic character and build detection via MumbleLink.

---

## Automatic Updates
GW2 Rotation Coach includes native **Raidcore Nexus Auto-Update (`UP_GitHub`)** support. When loaded in Nexus, the addon checks for new releases automatically so you can update in-game with a single click.

---

## Keyboard Shortcuts
| Hotkey | Action |
| :--- | :--- |
| `Alt + Shift + T` | Toggle Master Control Panel |
| `Alt + Shift + R` | Reset Rotation Sequence to the Beginning |
| `Alt + Shift + C` | Re-center Floating HUD Ribbon to Screen Center |
| `` ` `` (Backtick) | Toggle Weapon Set / Weapon Swap |
| `Escape` | Close Open Addon Windows |
