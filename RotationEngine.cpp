#include "RotationEngine.h"
#include <numeric>
#include <algorithm>

RotationEngine::RotationEngine()
{
    activeProfileIndex = 0;
    activeProfilePointer = nullptr;

    autoDetectionEnabled = true;
    lastDetectedProfession = 0;
    lastDetectedSpecialization = 0;
    autoDetectionCooldownSeconds = 0.5f;

    currentStepIndex = 0;
    executionStatus = RotationExecutionStatus::WaitingForFirstSkill;
    activeWeaponSetIndex = 0;

    redFlashRemainingSeconds = 0.0f;
    redFlashTotalDurationSeconds = 0.5f;

    networkLatencyMilliseconds = 50.0;
    hasPreviousActionTimestamp = false;

    activePresentationStyle = PromptPresentationStyle::SimpleTextRibbon;

    activeAttunementCategory = SkillSlotCategory::ProfessionSkillOne;

    autoSkipCooldownSkillsEnabled = true;
    activeCastRemainingSeconds = 0.0f;
    activeCastTotalSeconds = 0.0f;
    activeCastingSkillName = "";
    isChannelingActive = false;
    hasPreviousInputTimestamp = false;

    LoadBuildProfileByIndex(0);
}

void RotationEngine::LoadBuildProfileByIndex(uint32_t profileIndex)
{
    const BuildProfile* selectedProfile = rotationCatalog.GetProfilePointerByIndex(profileIndex);

    if (selectedProfile == nullptr)
    {
        return;
    }

    activeProfileIndex = profileIndex;
    activeProfilePointer = selectedProfile;

    rotationSteps = selectedProfile->sequenceSteps;

    for (const auto& step : rotationSteps)
    {
        uint32_t stepWeaponSet = DetermineStepWeaponSet(step);

        if (stepWeaponSet == 0 || stepWeaponSet == 1)
        {
            activeWeaponSetIndex = stepWeaponSet;
            break;
        }
    }

    ResetRotationProgress();
}

void RotationEngine::LoadBenchmarks(const std::string& directoryPath)
{
    rotationCatalog.LoadBenchmarkDirectory(directoryPath);
}

void RotationEngine::AssignSkillToSlot(const std::string& skillName, SkillSlotCategory targetSlotCategory)
{
    if (skillName.empty())
    {
        return;
    }

    customSkillSlotAssignments[skillName] = targetSlotCategory;
}

SkillSlotCategory RotationEngine::GetEffectiveSlotCategory(const RotationStep& rotationStep) const
{
    auto iterator = customSkillSlotAssignments.find(rotationStep.skillName);

    if (iterator != customSkillSlotAssignments.end())
    {
        return iterator->second;
    }

    return rotationStep.slotCategory;
}

void RotationEngine::ClearCustomSlotAssignments()
{
    customSkillSlotAssignments.clear();
}

const std::unordered_map<std::string, SkillSlotCategory>& RotationEngine::GetCustomSlotAssignments() const
{
    return customSkillSlotAssignments;
}

std::vector<std::string> RotationEngine::GetUniqueUtilitySkillNames() const
{
    std::vector<std::string> uniqueSkillNames;

    for (const auto& step : rotationSteps)
    {
        bool isUtility = (step.slotCategory == SkillSlotCategory::UtilitySkillOne ||
                          step.slotCategory == SkillSlotCategory::UtilitySkillTwo ||
                          step.slotCategory == SkillSlotCategory::UtilitySkillThree);

        if (isUtility)
        {
            if (std::find(uniqueSkillNames.begin(), uniqueSkillNames.end(), step.skillName) == uniqueSkillNames.end())
            {
                uniqueSkillNames.push_back(step.skillName);
            }
        }
    }

    return uniqueSkillNames;
}

float RotationEngine::GetSkillRemainingCooldownSeconds(const std::string& skillName) const
{
    auto iterator = skillCooldownTimers.find(skillName);

    if (iterator != skillCooldownTimers.end())
    {
        return iterator->second;
    }

    return 0.0f;
}

void RotationEngine::SetSkillCooldown(const std::string& skillName, float cooldownSeconds)
{
    if (skillName.empty() || cooldownSeconds <= 0.0f)
    {
        return;
    }

    skillCooldownTimers[skillName] = cooldownSeconds;

    ApplySpecialSkillCooldownEffects(skillName);
}

void RotationEngine::ApplySpecialSkillCooldownEffects(const std::string& skillName)
{
    if (skillName.find("Signet of the Ether") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Phantasmal") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Signet of Illusions") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Bladesong") != std::string::npos ||
                timerPair.first.find("Bladeturn") != std::string::npos ||
                timerPair.first.find("Mind Wrack") != std::string::npos ||
                timerPair.first.find("Cry of Frustration") != std::string::npos ||
                timerPair.first.find("Diversion") != std::string::npos ||
                timerPair.first.find("Distortion") != std::string::npos ||
                timerPair.first.find("Continuum Split") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Renewed Focus") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Virtue of") != std::string::npos ||
                timerPair.first.find("Spear of Justice") != std::string::npos ||
                timerPair.first.find("Wings of Resolve") != std::string::npos ||
                timerPair.first.find("Shield of Courage") != std::string::npos ||
                timerPair.first.find("Rushing Justice") != std::string::npos ||
                timerPair.first.find("Flowing Resolve") != std::string::npos ||
                timerPair.first.find("Crashing Courage") != std::string::npos ||
                timerPair.first.find("Chapter") != std::string::npos ||
                timerPair.first.find("Epilogue") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Berserk") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Arc Divider") != std::string::npos ||
                timerPair.first.find("Decapitate") != std::string::npos ||
                timerPair.first.find("Combustive Shot") != std::string::npos ||
                timerPair.first.find("Skull Grinder") != std::string::npos ||
                timerPair.first.find("Rupturing Smash") != std::string::npos ||
                timerPair.first.find("Flamestrike") != std::string::npos ||
                timerPair.first.find("Gun Flame") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Fresh Air") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Air Attunement") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Weave Self") != std::string::npos || skillName.find("Tailored Victory") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Attunement") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Swap Pet") != std::string::npos || skillName.find("Pet Swap") != std::string::npos ||
             skillName.find("Enter Beastmode") != std::string::npos || skillName.find("Leave Beastmode") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Command") != std::string::npos ||
                timerPair.first.find("Beast") != std::string::npos ||
                timerPair.first.find("Bite") != std::string::npos ||
                timerPair.first.find("Maul") != std::string::npos ||
                timerPair.first.find("Swipe") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Deadeye's Mark") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Deadeye's Mark") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
    else if (skillName.find("Legendary ") != std::string::npos && skillName.find("Stance") != std::string::npos)
    {
        for (auto& timerPair : skillCooldownTimers)
        {
            if (timerPair.first.find("Facet") != std::string::npos ||
                timerPair.first.find("Orders from the Above") != std::string::npos ||
                timerPair.first.find("Soulcleave") != std::string::npos)
            {
                timerPair.second = 0.0f;
            }
        }
    }
}

void RotationEngine::ResetAllCooldowns()
{
    skillCooldownTimers.clear();
}

uint32_t RotationEngine::GetActiveWeaponSetIndex() const
{
    return activeWeaponSetIndex;
}

void RotationEngine::SetActiveWeaponSetIndex(uint32_t weaponSetIndex)
{
    activeWeaponSetIndex = weaponSetIndex;
}

void RotationEngine::ToggleActiveWeaponSet()
{
    activeWeaponSetIndex = (activeWeaponSetIndex == 0) ? 1 : 0;
}

uint32_t RotationEngine::DetermineStepWeaponSet(const RotationStep& rotationStep) const
{
    if (rotationStep.slotCategory == SkillSlotCategory::WeaponSwap)
    {
        return 3;
    }

    bool isWeaponSlot = (rotationStep.slotCategory == SkillSlotCategory::WeaponSkillOne ||
                         rotationStep.slotCategory == SkillSlotCategory::WeaponSkillTwo ||
                         rotationStep.slotCategory == SkillSlotCategory::WeaponSkillThree ||
                         rotationStep.slotCategory == SkillSlotCategory::WeaponSkillFour ||
                         rotationStep.slotCategory == SkillSlotCategory::WeaponSkillFive);

    if (!isWeaponSlot)
    {
        return 2;
    }

    const std::string& name = rotationStep.skillName;

    if (name.find("Chapter") != std::string::npos ||
        name.find("Tome") != std::string::npos ||
        name.find("Shroud") != std::string::npos ||
        name.find("Life Transfer") != std::string::npos ||
        name.find("Tainted Shackles") != std::string::npos ||
        name.find("Forge") != std::string::npos ||
        name.find("Photon") != std::string::npos ||
        name.find("Holo") != std::string::npos ||
        name.find("Kit") != std::string::npos ||
        name.find("Bomb") != std::string::npos ||
        name.find("Grenade") != std::string::npos ||
        name.find("Flamethrower") != std::string::npos ||
        name.find("Elixir Gun") != std::string::npos ||
        name.find("Tool Kit") != std::string::npos ||
        name.find("Med Blaster") != std::string::npos ||
        name.find("Cleansing Field") != std::string::npos ||
        name.find("Flying Cutter") != std::string::npos ||
        name.find("Bladecall") != std::string::npos ||
        name.find("Unstable Bladestorm") != std::string::npos ||
        name.find("Celestial Avatar") != std::string::npos)
    {
        return 2;
    }

    if (name.find("Staff") != std::string::npos ||
        name.find("Symbol of Swiftness") != std::string::npos ||
        name.find("Empower") != std::string::npos ||
        name.find("Longbow") != std::string::npos ||
        name.find("Pistol") != std::string::npos ||
        name.find("Torch") != std::string::npos ||
        name.find("Dagger") != std::string::npos ||
        name.find("Focus") != std::string::npos ||
        name.find("Phantasmal Warden") != std::string::npos ||
        name.find("Warhorn") != std::string::npos ||
        name.find("Rifle") != std::string::npos ||
        name.find("Shortbow") != std::string::npos)
    {
        return 1;
    }

    if (name.find("GS") != std::string::npos ||
        name.find("Greatsword") != std::string::npos ||
        name.find("Symbol of Resolution") != std::string::npos ||
        name.find("Shield of Judgment") != std::string::npos ||
        name.find("Axe") != std::string::npos ||
        name.find("Sword") != std::string::npos ||
        name.find("Mace") != std::string::npos ||
        name.find("Hammer") != std::string::npos ||
        name.find("Shield") != std::string::npos ||
        name.find("Scepter") != std::string::npos)
    {
        return 0;
    }

    return 0;
}

uint32_t RotationEngine::GetDetectedProfessionNumber() const
{
    return lastDetectedProfession;
}

uint32_t RotationEngine::GetDetectedSpecializationNumber() const
{
    return lastDetectedSpecialization;
}

bool RotationEngine::DoesStepMatchActiveWeapon(const RotationStep& rotationStep) const
{
    uint32_t stepWeaponSet = DetermineStepWeaponSet(rotationStep);

    if (stepWeaponSet == 2 || stepWeaponSet == 3)
    {
        return true;
    }

    return (stepWeaponSet == activeWeaponSetIndex);
}

std::string RotationEngine::GetRequiredWeaponNameForStep(const RotationStep& rotationStep) const
{
    uint32_t stepWeaponSet = DetermineStepWeaponSet(rotationStep);

    if (stepWeaponSet == 1)
    {
        return "Secondary Weapon";
    }

    if (stepWeaponSet == 0)
    {
        return "Primary Weapon";
    }

    return "";
}

float RotationEngine::CalculateDefaultSkillCooldownSeconds(const std::string& skillName, SkillSlotCategory slotCategory) const
{
    const SkillDefinitionMetadata* nameMetadata = rotationCatalog.FindSkillMetadataByName(skillName);
    if (nameMetadata != nullptr && nameMetadata->cooldownSeconds > 0)
    {
        return static_cast<float>(nameMetadata->cooldownSeconds);
    }

    for (const auto& step : rotationSteps)
    {
        if (step.skillName == skillName && step.skillIdentifier > 0)
        {
            const SkillDefinitionMetadata* metadata = rotationCatalog.FindSkillMetadata(step.skillIdentifier);
            if (metadata != nullptr && metadata->cooldownSeconds > 0)
            {
                return static_cast<float>(metadata->cooldownSeconds);
            }
        }
    }
    if (skillName.find("Feel My Wrath") != std::string::npos)
    {
        return 35.0f;
    }

    if (skillName.find("Dragon's Maw") != std::string::npos)
    {
        return 50.0f;
    }

    if (skillName.find("Procession of Blades") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Test of Faith") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Mantra of Potence") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Mantra of Solace") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Empower") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Symbol of Swiftness") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Whirling Wrath") != std::string::npos)
    {
        return 6.0f;
    }

    if (skillName.find("Binding Blade") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Leap of Faith") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Symbol of Wrath") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Spear of Justice") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Thousand Cuts") != std::string::npos)
    {
        return 60.0f;
    }

    if (skillName.find("Signet of the Ether") != std::string::npos)
    {
        return 30.0f;
    }

    if (skillName.find("Signet of Illusions") != std::string::npos)
    {
        return 60.0f;
    }

    if (skillName.find("Bladesong Harmony") != std::string::npos)
    {
        return 12.0f;
    }

    if (skillName.find("Bladesong Sorrow") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Phantasmal Swordsman") != std::string::npos)
    {
        return 15.0f;
    }

    if (skillName.find("Phantasmal Warden") != std::string::npos)
    {
        return 20.0f;
    }

    if (skillName.find("Bladecall") != std::string::npos)
    {
        return 5.0f;
    }

    if (skillName.find("Flying Cutter") != std::string::npos)
    {
        return 0.0f;
    }

    for (const auto& step : rotationSteps)
    {
        if (step.skillName == skillName && step.skillIdentifier > 0)
        {
            const SkillDefinitionMetadata* metadata = rotationCatalog.FindSkillMetadata(step.skillIdentifier);
            if (metadata != nullptr && metadata->rechargeSeconds > 0 && metadata->skillName == skillName)
            {
                return static_cast<float>(metadata->rechargeSeconds);
            }
        }
    }

    if (slotCategory == SkillSlotCategory::EliteSkill)
    {
        return 45.0f;
    }

    if (slotCategory == SkillSlotCategory::HealingSkill)
    {
        return 15.0f;
    }

    if (slotCategory == SkillSlotCategory::UtilitySkillOne ||
        slotCategory == SkillSlotCategory::UtilitySkillTwo ||
        slotCategory == SkillSlotCategory::UtilitySkillThree)
    {
        return 20.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSkillFive)
    {
        return 20.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSkillFour)
    {
        return 15.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSkillThree)
    {
        return 12.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSkillTwo)
    {
        return 6.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSkillOne)
    {
        return 0.0f;
    }

    if (slotCategory == SkillSlotCategory::WeaponSwap)
    {
        return 9.0f;
    }

    return 10.0f;
}

bool RotationEngine::IsSkillOnCooldown(const RotationStep& rotationStep) const
{
    return (GetSkillRemainingCooldownSeconds(rotationStep.skillName) > 0.5f);
}

void RotationEngine::AdvanceToNextReadyStep()
{
    if (rotationSteps.empty())
    {
        return;
    }

    uint32_t candidateStepIndex = (currentStepIndex + 1) % rotationSteps.size();
    uint32_t skippedStepsCount = 0;
    lastAutoSkippedSkillName = "";

    while (IsSkillOnCooldown(rotationSteps[candidateStepIndex]) && skippedStepsCount < rotationSteps.size())
    {
        lastAutoSkippedSkillName = rotationSteps[candidateStepIndex].skillName;
        candidateStepIndex = (candidateStepIndex + 1) % rotationSteps.size();
        skippedStepsCount = skippedStepsCount + 1;
    }

    if (skippedStepsCount >= rotationSteps.size())
    {
        currentStepIndex = (currentStepIndex + 1) % rotationSteps.size();
        lastAutoSkippedSkillName = "";
    }
    else
    {
        currentStepIndex = candidateStepIndex;
    }
}

void RotationEngine::ProcessDetectedInput(const std::string& detectedInputName, KeybindDetector& keybindDetector)
{
    if (rotationSteps.empty())
    {
        return;
    }

    if (!keybindDetector.IsKeyBoundToAnyRotationSlot(detectedInputName))
    {
        return;
    }

    auto currentInstant = std::chrono::steady_clock::now();

    if (hasPreviousInputTimestamp)
    {
        auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentInstant - lastInputRegistrationTimestamp).count();

        if (elapsedMilliseconds < 100)
        {
            return;
        }
    }

    lastInputRegistrationTimestamp = currentInstant;
    hasPreviousInputTimestamp = true;

    if (keybindDetector.MatchesSlotKeybind("ProfessionSkill1", detectedInputName, "F1"))
    {
        activeAttunementCategory = SkillSlotCategory::ProfessionSkillOne;
    }
    else if (keybindDetector.MatchesSlotKeybind("ProfessionSkill2", detectedInputName, "F2"))
    {
        activeAttunementCategory = SkillSlotCategory::ProfessionSkillTwo;
    }
    else if (keybindDetector.MatchesSlotKeybind("ProfessionSkill3", detectedInputName, "F3"))
    {
        activeAttunementCategory = SkillSlotCategory::ProfessionSkillThree;
    }
    else if (keybindDetector.MatchesSlotKeybind("ProfessionSkill4", detectedInputName, "F4") || keybindDetector.MatchesSlotKeybind("ProfessionSkill4", detectedInputName, "E"))
    {
        activeAttunementCategory = SkillSlotCategory::ProfessionSkillFour;
    }

    if (detectedInputName == "`" || keybindDetector.MatchesSlotKeybind("WeaponSwap", detectedInputName, "`"))
    {
        ToggleActiveWeaponSet();

        const RotationStep& potentialSwapStep = rotationSteps[currentStepIndex];
        if (potentialSwapStep.slotCategory == SkillSlotCategory::WeaponSwap ||
            potentialSwapStep.skillName.find("Stow") != std::string::npos ||
            potentialSwapStep.skillName.find("Exit") != std::string::npos ||
            potentialSwapStep.skillName.find("Drop") != std::string::npos)
        {
            AdvanceToNextReadyStep();
            return;
        }
    }

    const RotationStep& initialExpectedStep = rotationSteps[currentStepIndex];
    SkillSlotCategory initialEffectiveSlot = GetEffectiveSlotCategory(initialExpectedStep);
    std::string initialSlotName = keybindDetector.GetSlotNameFromCategory(initialEffectiveSlot);

    bool inputMatched = keybindDetector.MatchesSlotKeybind(initialSlotName, detectedInputName, initialExpectedStep.defaultKeybindName);

    if (!inputMatched && keybindDetector.IsKeybindAutoLearningEnabled())
    {
        if (detectedInputName == "Mouse4" || detectedInputName == "Mouse5" || detectedInputName == "MiddleMouse")
        {
            keybindDetector.AssociateKeyWithSkillSlot(initialSlotName, detectedInputName);
            inputMatched = true;
        }
    }

    if (!inputMatched)
    {
        const RotationStep* upcomingStepPointer = GetLookaheadStepPointer(1);

        if (upcomingStepPointer != nullptr)
        {
            SkillSlotCategory upcomingSlotCategory = GetEffectiveSlotCategory(*upcomingStepPointer);
            std::string upcomingSlotName = keybindDetector.GetSlotNameFromCategory(upcomingSlotCategory);
            bool upcomingInputMatched = keybindDetector.MatchesSlotKeybind(upcomingSlotName, detectedInputName, upcomingStepPointer->defaultKeybindName);

            if (upcomingInputMatched)
            {
                float skippedRechargeDuration = CalculateDefaultSkillCooldownSeconds(initialExpectedStep.skillName, initialExpectedStep.slotCategory);
                SetSkillCooldown(initialExpectedStep.skillName, skippedRechargeDuration);

                AdvanceToNextReadyStep();
                inputMatched = true;
            }
        }
    }

    const RotationStep& activeStepToProcess = rotationSteps[currentStepIndex];
    SkillSlotCategory activeEffectiveSlot = GetEffectiveSlotCategory(activeStepToProcess);
    std::string activeSlotName = keybindDetector.GetSlotNameFromCategory(activeEffectiveSlot);

    bool weaponMatched = DoesStepMatchActiveWeapon(activeStepToProcess);

    if (inputMatched && !weaponMatched)
    {
        uint32_t stepWeaponSet = DetermineStepWeaponSet(activeStepToProcess);

        if (stepWeaponSet == 0 || stepWeaponSet == 1)
        {
            activeWeaponSetIndex = stepWeaponSet;
            weaponMatched = true;
        }
    }

    currentInstant = std::chrono::steady_clock::now();

    if (hasPreviousActionTimestamp)
    {
        auto elapsedNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentInstant - lastActionTimestamp).count();
        double elapsedMilliseconds = static_cast<double>(elapsedNanoseconds) / 1000000.0;
        recordedGapIntervals.push_back(elapsedMilliseconds);
    }

    lastActionTimestamp = currentInstant;
    hasPreviousActionTimestamp = true;

    if (inputMatched && weaponMatched)
    {
        executionStatus = RotationExecutionStatus::ActiveInCombat;

        if (keybindDetector.IsKeybindAutoLearningEnabled())
        {
            keybindDetector.AssociateKeyWithSkillSlot(activeSlotName, detectedInputName);
        }

        float rechargeDuration = CalculateDefaultSkillCooldownSeconds(activeStepToProcess.skillName, activeStepToProcess.slotCategory);
        SetSkillCooldown(activeStepToProcess.skillName, rechargeDuration);

        float expectedCastSeconds = static_cast<float>(activeStepToProcess.expectedDurationMilliseconds) / 1000.0f;
        if (expectedCastSeconds <= 0.0f)
        {
            uint32_t databaseCastMs = rotationCatalog.GetSkillCastTimeMilliseconds(activeStepToProcess.skillIdentifier, activeStepToProcess.skillName);
            expectedCastSeconds = static_cast<float>(databaseCastMs) / 1000.0f;
        }

        if (expectedCastSeconds > 0.15f)
        {
            activeCastTotalSeconds = expectedCastSeconds;
            activeCastRemainingSeconds = expectedCastSeconds;
            activeCastingSkillName = activeStepToProcess.skillName;
            isChannelingActive = true;
        }

        AdvanceToNextReadyStep();
        return;
    }

    TriggerRedFlash();

    std::string expectedKeyText = keybindDetector.GetFormattedKeybindText(activeSlotName, activeStepToProcess.defaultKeybindName);

    ExecutionMistake recordedMistake;
    recordedMistake.timestampMilliseconds = GetCurrentTimestampMilliseconds();
    recordedMistake.stepIndex = currentStepIndex;

    if (!weaponMatched)
    {
        recordedMistake.expectedSkillName = activeStepToProcess.skillName + " (Wrong Weapon Set!)";
    }
    else
    {
        recordedMistake.expectedSkillName = activeStepToProcess.skillName + " [" + expectedKeyText + "]";
    }

    recordedMistake.actualInputName = detectedInputName;
    mistakeHistory.push_back(recordedMistake);

    for (const auto& candidateStep : rotationSteps)
    {
        SkillSlotCategory candidateCategory = GetEffectiveSlotCategory(candidateStep);
        std::string candidateSlotName = keybindDetector.GetSlotNameFromCategory(candidateCategory);

        if (keybindDetector.MatchesSlotKeybind(candidateSlotName, detectedInputName, candidateStep.defaultKeybindName))
        {
            float miscastCooldown = CalculateDefaultSkillCooldownSeconds(candidateStep.skillName, candidateStep.slotCategory);
            SetSkillCooldown(candidateStep.skillName, miscastCooldown);
            break;
        }
    }
}

void RotationEngine::ProcessSkillActivation(uint32_t skillIdentifier, uint32_t durationMilliseconds)
{
    if (rotationSteps.empty())
    {
        return;
    }

    auto currentInstant = std::chrono::steady_clock::now();

    if (hasPreviousInputTimestamp)
    {
        auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentInstant - lastInputRegistrationTimestamp).count();

        if (elapsedMilliseconds < 150)
        {
            return;
        }
    }

    lastInputRegistrationTimestamp = currentInstant;
    hasPreviousInputTimestamp = true;

    const RotationStep& expectedStep = rotationSteps[currentStepIndex];

    if (expectedStep.skillIdentifier == skillIdentifier)
    {
        executionStatus = RotationExecutionStatus::ActiveInCombat;

        float rechargeDuration = CalculateDefaultSkillCooldownSeconds(expectedStep.skillName, expectedStep.slotCategory);
        SetSkillCooldown(expectedStep.skillName, rechargeDuration);

        float expectedCastSeconds = static_cast<float>(expectedStep.expectedDurationMilliseconds) / 1000.0f;
        if (expectedCastSeconds <= 0.0f)
        {
            uint32_t databaseCastMs = rotationCatalog.GetSkillCastTimeMilliseconds(expectedStep.skillIdentifier, expectedStep.skillName);
            expectedCastSeconds = static_cast<float>(databaseCastMs) / 1000.0f;
        }

        if (expectedCastSeconds > 0.15f)
        {
            activeCastTotalSeconds = expectedCastSeconds;
            activeCastRemainingSeconds = expectedCastSeconds;
            activeCastingSkillName = expectedStep.skillName;
            isChannelingActive = true;
        }

        AdvanceToNextReadyStep();
        return;
    }

    TriggerRedFlash();

    ExecutionMistake recordedMistake;
    recordedMistake.timestampMilliseconds = GetCurrentTimestampMilliseconds();
    recordedMistake.stepIndex = currentStepIndex;
    recordedMistake.expectedSkillName = expectedStep.skillName;
    recordedMistake.actualInputName = "SkillIdentifier " + std::to_string(skillIdentifier);
    mistakeHistory.push_back(recordedMistake);

    uint32_t forwardSearchIndex = currentStepIndex + 1;
    while (forwardSearchIndex < rotationSteps.size())
    {
        if (rotationSteps[forwardSearchIndex].skillIdentifier == skillIdentifier)
        {
            currentStepIndex = forwardSearchIndex;

            float rechargeDuration = CalculateDefaultSkillCooldownSeconds(rotationSteps[currentStepIndex].skillName, rotationSteps[currentStepIndex].slotCategory);
            SetSkillCooldown(rotationSteps[currentStepIndex].skillName, rechargeDuration);

            AdvanceToNextReadyStep();
            return;
        }

        forwardSearchIndex = forwardSearchIndex + 1;
    }
}

void RotationEngine::AdvanceToNextStep()
{
    if (rotationSteps.empty())
    {
        return;
    }

    currentStepIndex = (currentStepIndex + 1) % rotationSteps.size();
}

void RotationEngine::TriggerRedFlash()
{
    redFlashRemainingSeconds = redFlashTotalDurationSeconds;
}

void RotationEngine::UpdatePerFrame(float deltaSeconds)
{
    if (redFlashRemainingSeconds > 0.0f)
    {
        redFlashRemainingSeconds = redFlashRemainingSeconds - deltaSeconds;

        if (redFlashRemainingSeconds < 0.0f)
        {
            redFlashRemainingSeconds = 0.0f;
        }
    }

    for (auto& timerPair : skillCooldownTimers)
    {
        if (timerPair.second > 0.0f)
        {
            timerPair.second = timerPair.second - deltaSeconds;

            if (timerPair.second < 0.0f)
            {
                timerPair.second = 0.0f;
            }
        }
    }

    if (activeCastRemainingSeconds > 0.0f)
    {
        activeCastRemainingSeconds = activeCastRemainingSeconds - deltaSeconds;

        if (activeCastRemainingSeconds <= 0.0f)
        {
            activeCastRemainingSeconds = 0.0f;
            activeCastTotalSeconds = 0.0f;
            activeCastingSkillName = "";
            isChannelingActive = false;
        }
    }

    if (autoSkipCooldownSkillsEnabled && executionStatus == RotationExecutionStatus::ActiveInCombat && !rotationSteps.empty())
    {
        const RotationStep& currentExpectedStep = rotationSteps[currentStepIndex];

        if (IsSkillOnCooldown(currentExpectedStep))
        {
            AdvanceToNextReadyStep();
        }
    }

    if (autoDetectionEnabled)
    {
        autoDetectionCooldownSeconds = autoDetectionCooldownSeconds - deltaSeconds;

        if (autoDetectionCooldownSeconds <= 0.0f)
        {
            autoDetectionCooldownSeconds = 0.5f;

            uint32_t detectedProfession = 0;
            uint32_t detectedSpecialization = 0;

            if (mumbleLinkReader.QueryCharacterIdentity(detectedProfession, detectedSpecialization) && detectedProfession > 0)
            {
                if (detectedProfession != lastDetectedProfession || detectedSpecialization != lastDetectedSpecialization)
                {
                    lastDetectedProfession = detectedProfession;
                    lastDetectedSpecialization = detectedSpecialization;

                    bool currentProfileMatchesCharacter = (activeProfilePointer != nullptr) &&
                        (activeProfilePointer->professionIdentifierNumber == detectedProfession) &&
                        (detectedSpecialization == 0 || activeProfilePointer->specializationIdentifierNumber == detectedSpecialization);

                    if (!currentProfileMatchesCharacter)
                    {
                        uint32_t matchingProfileIndex = rotationCatalog.FindMatchingProfileIndex(detectedProfession, detectedSpecialization);
                        LoadBuildProfileByIndex(matchingProfileIndex);
                    }
                }
            }
        }
    }
}

void RotationEngine::ResetRotationProgress()
{
    currentStepIndex = 0;
    executionStatus = RotationExecutionStatus::WaitingForFirstSkill;
    hasPreviousActionTimestamp = false;
    hasPreviousInputTimestamp = false;
    recordedGapIntervals.clear();
    mistakeHistory.clear();
    redFlashRemainingSeconds = 0.0f;
    lastAutoSkippedSkillName = "";
    activeCastRemainingSeconds = 0.0f;
    activeCastTotalSeconds = 0.0f;
    activeCastingSkillName = "";
    isChannelingActive = false;
    ResetAllCooldowns();
}

RotationExecutionStatus RotationEngine::GetExecutionStatus() const
{
    return executionStatus;
}

uint32_t RotationEngine::GetCurrentStepIndex() const
{
    return currentStepIndex;
}

uint32_t RotationEngine::GetTotalStepCount() const
{
    return static_cast<uint32_t>(rotationSteps.size());
}

const RotationStep* RotationEngine::GetStepPointer(uint32_t requestedStepIndex) const
{
    if (requestedStepIndex < rotationSteps.size())
    {
        return &rotationSteps[requestedStepIndex];
    }

    return nullptr;
}

const RotationStep* RotationEngine::GetCurrentStepPointer() const
{
    if (rotationSteps.empty())
    {
        return nullptr;
    }

    return &rotationSteps[currentStepIndex];
}

const RotationStep* RotationEngine::GetNextStepPointer() const
{
    if (rotationSteps.empty())
    {
        return nullptr;
    }

    uint32_t nextIndex = (currentStepIndex + 1) % rotationSteps.size();
    return &rotationSteps[nextIndex];
}

const RotationStep* RotationEngine::GetPreviousStepPointer() const
{
    if (rotationSteps.empty())
    {
        return nullptr;
    }

    uint32_t previousIndex = (currentStepIndex == 0) ? (static_cast<uint32_t>(rotationSteps.size()) - 1) : (currentStepIndex - 1);
    return &rotationSteps[previousIndex];
}

const RotationStep* RotationEngine::GetLookaheadStepPointer(uint32_t lookaheadOffset) const
{
    if (rotationSteps.empty())
    {
        return nullptr;
    }

    uint32_t lookaheadIndex = (currentStepIndex + lookaheadOffset) % rotationSteps.size();
    return &rotationSteps[lookaheadIndex];
}

bool RotationEngine::IsRedFlashActive() const
{
    return (redFlashRemainingSeconds > 0.0f);
}

float RotationEngine::GetRedFlashOpacity() const
{
    if (redFlashTotalDurationSeconds <= 0.0f)
    {
        return 0.0f;
    }

    return (redFlashRemainingSeconds / redFlashTotalDurationSeconds);
}

BenchmarkEvaluation RotationEngine::CalculateBenchmarkEvaluation() const
{
    BenchmarkEvaluation evaluationResult;
    evaluationResult.successfulCastCount = currentStepIndex;
    evaluationResult.mistakeCount = static_cast<uint32_t>(mistakeHistory.size());
    evaluationResult.executionSpeedPercentage = 0.0;
    evaluationResult.deficitPercentageNinetyFive = 0.0;
    evaluationResult.deficitPercentageEightyFive = 0.0;

    double baselineEightyFivePercentMilliseconds = 210.0;
    double baselineNinetyFivePercentMilliseconds = 85.0;

    evaluationResult.benchmarkEightyFivePercentMilliseconds = baselineEightyFivePercentMilliseconds + (networkLatencyMilliseconds * 0.4);
    evaluationResult.benchmarkNinetyFivePercentMilliseconds = baselineNinetyFivePercentMilliseconds + (networkLatencyMilliseconds * 0.2);

    if (recordedGapIntervals.empty())
    {
        evaluationResult.currentAverageGapMilliseconds = 0.0;
        evaluationResult.pacingDeltaMilliseconds = 0.0;
        return evaluationResult;
    }

    double totalIntervalSum = std::accumulate(recordedGapIntervals.begin(), recordedGapIntervals.end(), 0.0);
    evaluationResult.currentAverageGapMilliseconds = totalIntervalSum / static_cast<double>(recordedGapIntervals.size());

    evaluationResult.pacingDeltaMilliseconds = evaluationResult.currentAverageGapMilliseconds - evaluationResult.benchmarkNinetyFivePercentMilliseconds;

    if (evaluationResult.currentAverageGapMilliseconds > 0.0)
    {
        evaluationResult.executionSpeedPercentage = (evaluationResult.benchmarkNinetyFivePercentMilliseconds / evaluationResult.currentAverageGapMilliseconds) * 100.0;

        if (evaluationResult.currentAverageGapMilliseconds > evaluationResult.benchmarkNinetyFivePercentMilliseconds)
        {
            evaluationResult.deficitPercentageNinetyFive = ((evaluationResult.currentAverageGapMilliseconds - evaluationResult.benchmarkNinetyFivePercentMilliseconds) / evaluationResult.benchmarkNinetyFivePercentMilliseconds) * 100.0;
        }

        if (evaluationResult.currentAverageGapMilliseconds > evaluationResult.benchmarkEightyFivePercentMilliseconds)
        {
            evaluationResult.deficitPercentageEightyFive = ((evaluationResult.currentAverageGapMilliseconds - evaluationResult.benchmarkEightyFivePercentMilliseconds) / evaluationResult.benchmarkEightyFivePercentMilliseconds) * 100.0;
        }
    }

    return evaluationResult;
}

const std::vector<ExecutionMistake>& RotationEngine::GetMistakeHistory() const
{
    return mistakeHistory;
}

PromptPresentationStyle RotationEngine::GetPresentationStyle() const
{
    return activePresentationStyle;
}

void RotationEngine::SetPresentationStyle(PromptPresentationStyle newPresentationStyle)
{
    activePresentationStyle = newPresentationStyle;
}

double RotationEngine::GetNetworkLatencyMilliseconds() const
{
    return networkLatencyMilliseconds;
}

void RotationEngine::SetNetworkLatencyMilliseconds(double latencyMilliseconds)
{
    networkLatencyMilliseconds = latencyMilliseconds;
}

const BuildProfile* RotationEngine::GetActiveProfilePointer() const
{
    return activeProfilePointer;
}

const RotationCatalog& RotationEngine::GetCatalogReference() const
{
    return rotationCatalog;
}

RotationCatalog& RotationEngine::GetMutableCatalogReference()
{
    return rotationCatalog;
}

uint32_t RotationEngine::GetActiveProfileIndex() const
{
    return activeProfileIndex;
}

bool RotationEngine::IsAutoDetectionEnabled() const
{
    return autoDetectionEnabled;
}

void RotationEngine::SetAutoDetectionEnabled(bool enabledState)
{
    autoDetectionEnabled = enabledState;
}

uint64_t RotationEngine::GetCurrentTimestampMilliseconds() const
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

const std::string& RotationEngine::GetLastAutoSkippedSkillName() const
{
    return lastAutoSkippedSkillName;
}

void RotationEngine::ClearLastAutoSkippedSkillName()
{
    lastAutoSkippedSkillName = "";
}

bool RotationEngine::IsInActiveGameplay()
{
    return mumbleLinkReader.IsInActiveGameplay();
}

bool RotationEngine::IsChannelingActive() const
{
    return isChannelingActive;
}

float RotationEngine::GetCastProgressPercentage() const
{
    if (activeCastTotalSeconds <= 0.0f)
    {
        return 1.0f;
    }

    float percentage = 1.0f - (activeCastRemainingSeconds / activeCastTotalSeconds);

    if (percentage < 0.0f)
    {
        return 0.0f;
    }

    if (percentage > 1.0f)
    {
        return 1.0f;
    }

    return percentage;
}

float RotationEngine::GetActiveCastRemainingSeconds() const
{
    return activeCastRemainingSeconds;
}

const std::string& RotationEngine::GetActiveCastingSkillName() const
{
    return activeCastingSkillName;
}

bool RotationEngine::IsAutoSkipCooldownSkillsEnabled() const
{
    return autoSkipCooldownSkillsEnabled;
}

void RotationEngine::SetAutoSkipCooldownSkillsEnabled(bool isEnabled)
{
    autoSkipCooldownSkillsEnabled = isEnabled;
}

SkillSlotCategory RotationEngine::GetActiveAttunementCategory() const
{
    return activeAttunementCategory;
}

void RotationEngine::SetActiveAttunementCategory(SkillSlotCategory attunementCategory)
{
    activeAttunementCategory = attunementCategory;
}


bool RotationEngine::IsInCompetitiveGameMode()
{
    return mumbleLinkReader.IsInCompetitiveGameMode();
}

bool RotationEngine::IsInSpecialForcesTrainingArea()
{
    return mumbleLinkReader.IsInSpecialForcesTrainingArea();
}

uint32_t RotationEngine::GetCurrentMapIdentifier()
{
    return mumbleLinkReader.GetCurrentMapIdentifier();
}

uint32_t RotationEngine::GetCurrentMapType()
{
    return mumbleLinkReader.GetCurrentMapType();
}
