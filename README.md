# GW2 Rotation Coach

**Current Version:** `v0.527`  
**Author:** `Oneleaf`  
**Framework:** [Raidcore Nexus](https://raidcore.gg/Nexus)

A real-time combat rotation trainer, benchmark pacing analyzer, and in-game action bar overlay for Guild Wars 2. Designed to help players learn, practice, and master raid, strike, and fractal rotations.

GW2 Rotation Coach only reads local game state via standard MumbleLink and user inputs - it never writes to, modifies, or automates the game. See the [Addon Policy & Compliance](#addon-policy--compliance) section below for details on how it stays within ArenaNet's guidelines.

---

## Highlights

* **Real-Time Rotation Ribbon HUD:** Floating prompt sequence showing upcoming skills with live channel progress and mistake detection.
* **Upgraded Action Bar Slot Highlighter:** High-visibility in-game skill slot highlight featuring dynamic breathing glow, tactical HUD reticle brackets, high-contrast keybind badges, and category-aware color coding.
* **Build & Gear Showcase:** Instant access to 271 benchmark profiles across all 9 professions and all 27 Elite Specializations with template chat codes and Snow Crows guide links.
* **Dual-Percentage Benchmark Pacing:** Live execution gap analysis comparing your pacing against official 85% and 95% raid benchmark targets.
* **Profession Reset Mechanics:** Intelligent cooldown tracking across all 9 professions (Renewed Focus, Berserk, Fresh Air, Pet Swaps, Legend Stance Swaps, Signets).
* **Fair Play & Compliance Protection:** Automatic overlay suppression in competitive game modes (PvP and WvW) and optional restriction to the Special Forces Training Area (Golem only).
* **Automatic Updates:** Native Raidcore Nexus auto-updater (`UP_GitHub`) support.

---

## Installation Guide

### Prerequisites
* **Raidcore Nexus:** Make sure you have Raidcore Nexus installed. If not, grab the loader from [Raidcore Nexus](https://raidcore.gg/Nexus).

### Step-by-Step Installation
1. Download the release package (`Gw2RotationCoach-v0.527.zip` or the files from the [Releases](https://github.com/DebnityaDuttaGupta/GW2-RaidRotationCoach/releases) page).
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
   > The addon features automatic multi-path discovery and will scan `addons/Gw2RotationCoach`, the game root, and the DLL's folder. If the assets folder is ever misplaced, an in-game diagnostic banner with an **Open Addons Folder** button will guide you.
4. Start Guild Wars 2.
5. On your first launch, the **Master Control Panel** will automatically open on screen.

---

## Features & How to Use Them

### 1. Master Control Panel (`Alt + Shift + T`)
Your central dashboard for managing the addon:
* **Display & HUD Style:** Toggle between ribbon HUD modes, adjust icon sizes, lookahead count, and opacity.
* **Skills & Keybinds:** Configure your slot bindings.
* **Build & Benchmark:** View your active rotation, toggle auto-detection, and check network latency.
* **Encounter & Fair Play Restrictions:** Configure PvP/WvW suppression and golem-only mode.
* **One-Click Display:** Click **Show All Windows** to open or hide all panels at once.

### 2. Real-Time Rotation Ribbon HUD
* **Floating Prompt Ribbon:** Floats above your screen showing the sequence of upcoming skills. Drag it anywhere that fits your setup.
* **3 Presentation Modes:**
  1. *Transparent Single Key:* Minimalist key badges with directional flow arrows.
  2. *Skill Icon Images:* Full high-resolution skill icons with keybind badges.
  3. *Action Bar Slot Overlay:* Action bar numbers and slot identifiers.
* **Live Cast Channel Bar:** Real-time progress bar displaying cast channel duration and remaining animation seconds. Instant skills (0 ms) cast immediately without false channel lockouts.
* **Mistake Feedback:** Flashes red when you press the wrong skill or interrupt a cast, logging the error in the Mistake History window.

### 3. Upgraded Action Bar Slot Highlighter (Direct In-Game Overlay)
* **Direct Skill Highlights:** Draws glowing borders directly over your in-game action bars to indicate which skill to cast next.
* **Dynamic Breathing Glow:** Smooth sine-wave animated pulse on the active skill box to catch your peripheral vision during combat.
* **Tactical Reticle Accents:** Chamfered corner HUD brackets framing each active slot.
* **Keybind Badges:** Compact badges pinned to the slot showing your mapped hotkey (`1`, `F1`, `8`).
* **Category-Aware Colors:**
  * Radiant Amber for weapon and utility skills.
  * Cyan for profession mechanics (`F1`–`F5`).
  * Emerald Green for healing abilities.
  * Amethyst Purple for elite skills.
  * Crimson Flash for mistake alerts.
* **Fully Decoupled Controls:** Weapon bar (`1`–`5`), Utility bar (`6`–`0`), and Profession mechanics (`F1`–`F5`) have completely independent horizontal and vertical adjustments.
* **Diagnostic Alignment Grid:** Check `Highlight All Action Bar Slots` to reveal alignment frames for pixel-perfect positioning.

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

## Addon Policy & Compliance

GW2 Rotation Coach is designed from the ground up to operate within ArenaNet's **Third-Party Programs Policy**:

* **No Automation ("1 Key = 1 Action"):** GW2 Rotation Coach is purely an informational, visual training guide. It sends zero simulated keystrokes, executes no macros, does not play the game for you, and does not automate any player action.
* **No Unfair Competitive Advantage:** In competitive game modes (Structured PvP and World vs. World), the addon **automatically disables** combat-relevant overlays (the Rotation Ribbon and Action Bar Highlighter) to ensure fair play against other players.
* **Optional Golem-Only Practice Mode:** For players who wish to restrict rotation guidance strictly to dummy practice, a dedicated toggle restricts overlays exclusively to the **Special Forces Training Area** (Map ID 1154).
* **Safe Reading Only:** The addon reads player context via the official Guild Wars 2 MumbleLink interface. It does not write to, patch, or alter game client memory or game files.
* **Disclaimer:** All third-party programs in Guild Wars 2 are used at the player's own risk. GW2 Rotation Coach is an unofficial, community-made tool and is not affiliated with or endorsed by ArenaNet.

---

## Keyboard Shortcuts

| Hotkey | Action |
| :--- | :--- |
| `Alt + Shift + T` | Toggle Master Control Panel |
| `Alt + Shift + R` | Reset Rotation Sequence to the Beginning |
| `Alt + Shift + C` | Re-center Floating HUD Ribbon to Screen Center |
| `` ` `` (Backtick) | Toggle Weapon Set / Weapon Swap |
| `Escape` | Close Open Addon Windows |

---

## Credits

GW2 Rotation Coach is built with gratitude to the Guild Wars 2 community and these projects:

* **[Raidcore Nexus](https://raidcore.gg/Nexus)** - The modern addon platform and loader hosting GW2 Rotation Coach.
* **[Dear ImGui](https://github.com/ocornut/imgui)** by Omar Cornut - The immediate-mode graphical user interface library rendering the in-game HUD.
* **[Snow Crows](https://snowcrows.com/)** - Benchmark data, rotation timings, and build template standards for high-end Guild Wars 2 PvE.
* **ArenaNet** - Guild Wars 2 and all related assets are © ArenaNet, LLC and NCSOFT Corporation.

---

## License

GW2 Rotation Coach is distributed under the [Freeware Community License](LICENSE).  
Copyright © 2026 Oneleaf. All rights reserved.
