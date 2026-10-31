#ifndef ROTATION_ENGINE_HEADER
#define ROTATION_ENGINE_HEADER

#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "RotationTypes.h"
#include "KeybindDetector.h"
#include "RotationCatalog.h"
#include "MumbleLinkReader.h"

class RotationEngine
{
public:
    RotationEngine();

    void LoadBuildProfileByIndex(uint32_t profileIndex);

    void LoadBenchmarks(const std::string& directoryPath);

    void ProcessDetectedInput(const std::string& detectedInputName, KeybindDetector& keybindDetector);

    void ProcessSkillActivation(uint32_t skillIdentifier, uint32_t durationMilliseconds);

    void UpdatePerFrame(float deltaSeconds);

    void ResetRotationProgress();

    RotationExecutionStatus GetExecutionStatus() const;

    uint32_t GetCurrentStepIndex() const;

    uint32_t GetTotalStepCount() const;

    const RotationStep* GetStepPointer(uint32_t requestedStepIndex) const;

    const RotationStep* GetCurrentStepPointer() const;

    const RotationStep* GetNextStepPointer() const;

    const RotationStep* GetPreviousStepPointer() const;

    const RotationStep* GetLookaheadStepPointer(uint32_t lookaheadOffset) const;

    bool IsRedFlashActive() const;

    float GetRedFlashOpacity() const;

    BenchmarkEvaluation CalculateBenchmarkEvaluation() const;

    const std::vector<ExecutionMistake>& GetMistakeHistory() const;

    PromptPresentationStyle GetPresentationStyle() const;

    void SetPresentationStyle(PromptPresentationStyle newPresentationStyle);

    double GetNetworkLatencyMilliseconds() const;

    void SetNetworkLatencyMilliseconds(double latencyMilliseconds);

    const BuildProfile* GetActiveProfilePointer() const;

    const RotationCatalog& GetCatalogReference() const;

    RotationCatalog& GetMutableCatalogReference();

    uint32_t GetActiveProfileIndex() const;

    bool IsAutoDetectionEnabled() const;

    void SetAutoDetectionEnabled(bool enabledState);

    void AssignSkillToSlot(const std::string& skillName, SkillSlotCategory targetSlotCategory);

    SkillSlotCategory GetEffectiveSlotCategory(const RotationStep& rotationStep) const;

    void ClearCustomSlotAssignments();

    const std::unordered_map<std::string, SkillSlotCategory>& GetCustomSlotAssignments() const;

    std::vector<std::string> GetUniqueUtilitySkillNames() const;

    float GetSkillRemainingCooldownSeconds(const std::string& skillName) const;

    void SetSkillCooldown(const std::string& skillName, float cooldownSeconds);

    void ResetAllCooldowns();

    uint32_t GetActiveWeaponSetIndex() const;

    void SetActiveWeaponSetIndex(uint32_t weaponSetIndex);

    void ToggleActiveWeaponSet();

    bool IsSkillOnCooldown(const RotationStep& rotationStep) const;

    uint32_t GetDetectedProfessionNumber() const;

    uint32_t GetDetectedSpecializationNumber() const;

    bool DoesStepMatchActiveWeapon(const RotationStep& rotationStep) const;

    std::string GetRequiredWeaponNameForStep(const RotationStep& rotationStep) const;

    const std::string& GetLastAutoSkippedSkillName() const;

    void ClearLastAutoSkippedSkillName();

    bool IsInActiveGameplay();

    bool IsChannelingActive() const;

    float GetCastProgressPercentage() const;

    float GetActiveCastRemainingSeconds() const;

    const std::string& GetActiveCastingSkillName() const;

    bool IsAutoSkipCooldownSkillsEnabled() const;

    void SetAutoSkipCooldownSkillsEnabled(bool isEnabled);

    uint32_t DetermineStepWeaponSet(const RotationStep& rotationStep) const;

    SkillSlotCategory GetActiveAttunementCategory() const;

    void SetActiveAttunementCategory(SkillSlotCategory attunementCategory);

private:
    SkillSlotCategory activeAttunementCategory;
    std::string lastAutoSkippedSkillName;
    RotationCatalog rotationCatalog;
    MumbleLinkReader mumbleLinkReader;

    uint32_t activeProfileIndex;
    const BuildProfile* activeProfilePointer;

    bool autoDetectionEnabled;
    uint32_t lastDetectedProfession;
    uint32_t lastDetectedSpecialization;
    float autoDetectionCooldownSeconds;

    std::vector<RotationStep> rotationSteps;
    std::vector<ExecutionMistake> mistakeHistory;
    std::vector<double> recordedGapIntervals;
    std::unordered_map<std::string, SkillSlotCategory> customSkillSlotAssignments;
    std::unordered_map<std::string, float> skillCooldownTimers;

    uint32_t currentStepIndex;
    RotationExecutionStatus executionStatus;
    uint32_t activeWeaponSetIndex;

    float redFlashRemainingSeconds;
    float redFlashTotalDurationSeconds;

    double networkLatencyMilliseconds;
    std::chrono::steady_clock::time_point lastActionTimestamp;
    bool hasPreviousActionTimestamp;

    PromptPresentationStyle activePresentationStyle;

    bool autoSkipCooldownSkillsEnabled;
    float activeCastRemainingSeconds;
    float activeCastTotalSeconds;
    std::string activeCastingSkillName;
    bool isChannelingActive;

    std::chrono::steady_clock::time_point lastInputRegistrationTimestamp;
    bool hasPreviousInputTimestamp;

    void AdvanceToNextStep();

    void AdvanceToNextReadyStep();

    void TriggerRedFlash();

    uint64_t GetCurrentTimestampMilliseconds() const;

    float CalculateDefaultSkillCooldownSeconds(const std::string& skillName, SkillSlotCategory slotCategory) const;
};

#endif
