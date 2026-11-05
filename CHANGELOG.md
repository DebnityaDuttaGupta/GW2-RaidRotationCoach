# Changelog

All notable changes to **GW2 Rotation Coach** will be documented in this file.

---

## [0.525] - 2026-11-05

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
* **Multi-Path Directory Discovery:** Automatically detects the `Gw2RotationCoach` assets folder across multiple installation paths (`addons/Gw2RotationCoach`, root directory, and relative module directories).
* **Missing Assets In-Game Diagnostic:** Displays a clear diagnostic banner and an "Open Addons Folder" shortcut if benchmark asset files are missing, preventing blank showcase windows.
* **Nexus GitHub Auto-Updater:** Integrated native `UP_GitHub` update provider targeting repository releases.
* **Modern Condition Harbinger Build:** Updated Condition Harbinger rotation and slot mapping to match the modern *Elixir of Risk* + *Plague Signet* setup, alongside a dedicated *Signet of Spite* variant.

### Fixed
* **Passive Trait & Sigil Proc Elimination:** Filtered out uncastable passive traits (`Doom Approaches` / `Approaching Doom`, `Sunspot`, `Chilling Nova`, `Aim-Assisted Rocket`, `Earthen Blast`, `Deathly Haste`, `Unflinching Fortitude`) and passive sigil/rune procs from active rotation sequences, preventing false prompts and skipped steps.
* **Skill Database Recharge Values:** Connected `metadata.cooldownSeconds` to database recharge values, activating accurate in-game cooldown timers for all 2,300+ skills.
* **Instant Cast Duration Handling:** Corrected parser logic so instant-cast skills (0 ms) preserve zero duration rather than defaulting to 500 ms, eliminating artificial channel bar lockouts.

### Changed
* **Decoupled Action Bar Highlighter:** Completely unlinked Weapon Bar, Utility Bar, and Profession Bars into independent horizontal and vertical offsets. Dragging one bar no longer shifts other bars.
* **Refined Virtuoso HUD Alignment:** Re-calibrated default Virtuoso Bladesong slot positions (`F1` through `F5`) to align with native in-game interface coordinates.
* **Build Catalog Disambiguation:** Extended build showcase tree formatting to display both primary and secondary offhand weapon sets and unique variant tags across all 270 builds.
* **Developer Alignment Tooling:** Restricted drag handles to display exclusively when the Action Bar Alignment Grid diagnostic mode is enabled.
* **Version Baseline:** Set version to `0.525` (`Oneleaf525`).
