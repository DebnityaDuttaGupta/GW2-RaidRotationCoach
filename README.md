# GW2 Raid Rotation Coach

A standalone Guild Wars 2 rotation coach addon built on Raidcore's Nexus framework.

Provides real-time rotation assistance, visual HUD ribbon with custom keybind badges, and in-game action bar skill slot highlights.

---

## Features

- **Real-Time Rotation HUD Ribbon:**
  - Visual mode with floating skill keybinds and arrows.
  - In-game texture mode with skill icons.
  - Direct in-game action bar slot highlight overlays.

- **Accurate In-Game Slot Alignment:**
  - Native profession layouts (Mesmer, Elementalist, Guardian, Warrior, Ranger, Necromancer, Thief, Engineer, Revenant).
  - Dynamic Elementalist active attunement sizing and elevation matching the native UI.
  - Proper Catalyst Jade Sphere energy gauge offset.

- **Native Keybind Integration:**
  - Automatic import of exported Guild Wars 2 input binding XML files (`Documents\Guild Wars 2\InputBinds\*.xml`).
  - In-game dual-keybinding configuration with quick-attach for mouse buttons (M3, M4, M5).
  - Single-key badge formatting on HUD ribbons.

- **Snow Crows Benchmark Rotation Support:**
  - Built-in reference rotations for raid and strike builds.
  - Automatic profession and specialization detection via MumbleLink.

---

## Installation

1. Download `Gw2RotationCoach.dll` from the repository releases or the `main` branch.
2. Place `Gw2RotationCoach.dll` into your `<Guild Wars 2>\addons\` directory.
3. Start Guild Wars 2 with Raidcore Nexus installed.
4. Press `Alt + Shift + T` to open the Master Control Panel, or open it through the Nexus Addon Manager.

---

## Default Shortcuts

- `Alt + Shift + T`: Toggle Master Control Panel
- `Alt + Shift + R`: Reset Rotation Progress to Start
