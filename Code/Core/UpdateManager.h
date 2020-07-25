/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_UPDATEMANAGER_H
#define BERSERK_UPDATEMANAGER_H

#include <TEnumMask.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>

namespace Berserk {

    enum class EUpdateStage : uint8 {
        /** Called before task generation stage */
        PreTaskGen = 0,
        /** Called for task generation */
        TaskGen,
        /** Called after task generation stage */
        PostTaskGen,
        /** Called before main update */
        PreUpdate,
        /** Called for main update */
        Update,
        /** Called after update stage */
        PostUpdate,
        /** Called before task sync stage */
        PreTaskSync,
        /** Called for task sync */
        TaskSync,
        /** Called after task sync */
        PostTaskSync
    };

    /** Allows to subscribe to update manager */
    class UpdateStageListener {
    public:
        /**
         * Executed on specified stage, passed as argument
         * @param stage Stage type of execution
         */
        virtual void onStageExec(EUpdateStage stage) = 0;
    };

    /**
     * @brief Engine update manager
     *
     * Manages all the engine execution process and update/execution
     * stages. Allows to add entries and call the in the chosen engine
     * update stages.
     *
     * This module manager is executed on the main thread, and this manager
     * provides features for updating in such stages as pre,post,task generation,etc.
     */
    class UpdateManager {
    public:

        UpdateManager();
        ~UpdateManager();

        /** Subscribe listener to chosen stage */
        void subscribe(UpdateStageListener& listener, EUpdateStage stage);

        /** Subscribe listener to chosen list of stages */
        void subscribe(UpdateStageListener& listener, const std::initializer_list<EUpdateStage>& stages);

        /** Unsubscribe listener for chosen stage */
        void unsubscribe(UpdateStageListener& listener, EUpdateStage stage);

        /** Unsubscribe listener for chosen list of stages */
        void unsubscribe(UpdateStageListener& listener, const std::initializer_list<EUpdateStage>& stages);

        /** Unsubscribe listener for all stages, he is listening to */
        void unsubscribeFromAll(UpdateStageListener& listener);

        /** @return Engine update manager instance */
        static UpdateManager& getSingleton();

        /** Total number of the stages in update cycle */
        static const uint32 TOTAL_STAGES = 9;

        /** @return String stage name */
        static const char* getStageStringFromEnum(EUpdateStage stage);

    private:

        /** Updates manager state */
        void update();

        friend class Main;

        using Entry = TPair<TEnumMask<EUpdateStage>,UpdateStageListener*>;
        using List = TArray<UpdateStageListener*>;
        using Map = TMap<EUpdateStage, List, THashRaw<EUpdateStage>>;

        Map mListeners;
        TArray<Entry> mPendingSubscribe;
        TArray<Entry> mPendingUnsubscribe;

        static UpdateManager* gUpdateManager;

    };

}

#endif //BERSERK_UPDATEMANAGER_H