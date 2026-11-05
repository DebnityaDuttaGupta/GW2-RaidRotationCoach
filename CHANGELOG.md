# Changelog

All notable changes to **GW2 Rotation Coach** will be documented in this file.

---

## [0.526] - 2026-11-05

### Added
* **Multi-Path Directory Discovery:** Automatically detects the `Gw2RotationCoach` assets folder across multiple installation paths (`addons/Gw2RotationCoach`, root game folder, and DLL module paths), ensuring builds load even if files are extracted into alternate directory structures.
* **Missing Assets In-Game Diagnostic:** Displays a clear diagnostic banner and an "Open Addons Folder" shortcut if benchmark asset files are missing, eliminating confusing blank showcase windows.
* **Nexus GitHub Auto-Updater:** Integrated native `UP_GitHub` update provider targeting repository releases.
* **Modern Condition Harbinger Build:** Updated Condition Harbinger rotation and slot mapping to match the modern *Elixir of Risk* + *Plague Signet* setup, alongside a dedicated *Signet of Spite* variant.

### Fixed
* **Passive Trait & Sigil Proc Elimination:** Filtered out uncastable passive traits (`Doom Approaches` / `Approaching Doom`, `Sunspot`, `Chilling Nova`, `Aim-Assisted Rocket`, `Earthen Blast`, `Deathly Haste`, `Unflinching Fortitude`) and passive sigil/rune procs from active rotation sequences, preventing false prompts and skipped steps.
* **Skill Database Recharge Values:** Connected `metadata.cooldownSeconds` to database recharge values, activating accurate in-game cooldown timers for all 2,300+ skills across all 9 professions.
* **Instant Cast Duration Handling:** Corrected parser logic so instant-cast skills (0 ms) preserve zero duration rather than defaulting to 500 ms, eliminating artificial channel bar lockouts.

---

## [0.525] - 2026-11-04

### Added
* **Dynamic Network Ping Auto-Detection:** Automatically discovers the active game server connection on port `6112`, samples TCP round-trip latency, and applies real-time smoothed ping to pacing targets. Includes an auto-detect toggle with manual override fallback.
* **Dual-Percentage Benchmark Pacing Metrics:** Overhauled pacing analysis to display cast speed as a percentage and report exact deficit percentages against both 85% and 95% benchmark standards.
* **Full Profession Cooldown Reset Support:** Added comprehensive skill cooldown refresh mechanics across all professions:
  * Guardian: *Renewed Focus* resetting all active virtue abilities.
  * Warrior: *Berserk* instantly refreshing primal bursts.
  * Elementalist: *Fresh Air* resetting Air Attunement and *Weave Self* resetting attunements.
  * Ranger: Pet swaps and Soulbeast beastmode transitions.
  * Revenant: Legend swaps resetting stance cooldowns and energy.
  * Thief: *Deadeye's Mark* refresh on defeat.
  * Mesmer: *Signet of the Ether* resetting all phantasms and *Signet of Illusions* resetting all shatters and bladesongs.
* **Weapon Loadout Disambiguation:** Updated build showcase tree formatting to display both primary and secondary offhand weapon sets (e.g., `Condition Virtuoso (Dagger & Sword / Focus)` vs. `Condition Virtuoso (Dagger & Sword / Pistol)`).
* **First-Run Onboarding:** Automatically opens the Master Control panel upon initial installation when no prior configuration file is present.

### Changed
* **Decoupled Action Bar Highlighter:** Completely unlinked Weapon Bar, Utility Bar, and Profession Bars into independent horizontal and vertical offsets. Dragging one bar no longer shifts other bars.
* **Refined Virtuoso HUD Alignment:** Re-calibrated default Virtuoso Bladesong slot positions (`F1` through `F5`) to align with native in-game interface coordinates.
* **Developer Alignment Tooling:** Restricted drag handles to display exclusively when the Action Bar Alignment Grid diagnostic mode is enabled.
* **Version Baseline:** Set version to `0.525` (`Oneleaf525`).
