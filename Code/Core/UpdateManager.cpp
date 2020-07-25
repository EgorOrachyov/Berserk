/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <UpdateManager.h>

namespace Berserk {

    UpdateManager* UpdateManager::gUpdateManager = nullptr;

    static std::initializer_list<EUpdateStage> order = {
            EUpdateStage::PreTaskGen,
            EUpdateStage::TaskGen,
            EUpdateStage::PostTaskGen,
            EUpdateStage::PreUpdate,
            EUpdateStage::Update,
            EUpdateStage::PostUpdate,
            EUpdateStage::PreTaskSync,
            EUpdateStage::TaskSync,
            EUpdateStage::PostTaskSync
    };

    UpdateManager::UpdateManager() {
        if (gUpdateManager) {
            fprintf(stderr, "[BERSERK Core] Only single UpdateManager could be set as singleton");
            return;
        }

        gUpdateManager = this;

        for (auto stage: order) {
            mListeners.emplace(stage);
        }
    }

    UpdateManager::~UpdateManager() {
        for (auto stage: order) {
            auto& list = mListeners[stage];

            if (list.size() > 0) {
                BERSERK_ERROR("Unsubscribed listeners (%u) for stage %s", list.size(), getStageStringFromEnum(stage));
            }
        }
    }

    void UpdateManager::subscribe(UpdateStageListener &listener, EUpdateStage stage) {
        subscribe(listener, { stage });
    }

    void UpdateManager::subscribe(UpdateStageListener &listener, const std::initializer_list<EUpdateStage> &stages) {
        Entry entry(stages, &listener);
        mPendingSubscribe.move(entry);
    }

    void UpdateManager::unsubscribe(UpdateStageListener &listener, EUpdateStage stage) {
        unsubscribe(listener, { stage });
    }

    void UpdateManager::unsubscribe(UpdateStageListener &listener, const std::initializer_list<EUpdateStage> &stages) {
        Entry entry(stages, &listener);
        mPendingUnsubscribe.move(entry);
    }

    void UpdateManager::unsubscribeFromAll(UpdateStageListener &listener) {
        Entry entry(order, &listener);
        mPendingUnsubscribe.move(entry);
    }
    
    void UpdateManager::update() {
        // Remove listeners pending unsubscribe
        // Add listeners pending subscribe
        // Per stages traverse and update call

        for (auto& entry: mPendingUnsubscribe) {
            auto& stages = entry.first();
            auto  listener = entry.second();

            for (auto stage: order) {
                if (stages.getFlag(stage)) {
                    auto& list = mListeners[stage];

                    if (!list.contains(listener)) {
                        BERSERK_ERROR("Listener was not subscribed %p", listener);
                        continue;
                    }

                    list.removeElement(listener);
                }
            }
        }

        mPendingUnsubscribe.clear();

        for (auto& entry: mPendingSubscribe) {
            auto& stages = entry.first();
            auto  listener = entry.second();

            for (auto stage: order) {
                if (stages.getFlag(stage)) {
                    auto& list = mListeners[stage];

                    if (list.contains(listener)) {
                        BERSERK_ERROR("Listener already subscribed %p", listener);
                        continue;
                    }

                    list.add(listener);
                }
            }
        }

        mPendingSubscribe.clear();

        for (auto stage: order) {
            auto& list = mListeners[stage];

            for (auto listener: list) {
                listener->onStageExec(stage);
            }
        }
    }

    UpdateManager& UpdateManager::getSingleton() {
        return *gUpdateManager;
    }

    const char* UpdateManager::getStageStringFromEnum(Berserk::EUpdateStage stage) {
        switch (stage) {
            case EUpdateStage::PreTaskGen:
                return "PreTaskGen";
            case EUpdateStage::TaskGen:
                return "TaskGen";
            case EUpdateStage::PostTaskGen:
                return "PostTaskGen";
            case EUpdateStage::PreUpdate:
                return "PreUpdate";
            case EUpdateStage::Update:
                return "Update";
            case EUpdateStage::PostUpdate:
                return "PostUpdate";
            case EUpdateStage::PreTaskSync:
                return "PreTaskSync";
            case EUpdateStage::TaskSync:
                return "TaskSync";
            case EUpdateStage::PostTaskSync:
                return "PostTaskSync";
            default:
                return "Unknown";
        }
    }

}
