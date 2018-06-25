//
// Created by Egor Orachyov on 10.06.2018.
//

#ifndef BERSERKENGINE_APPLICATIONCONTEXT_H
#define BERSERKENGINE_APPLICATIONCONTEXT_H

#include "../Core/Logging/LogManager.h"
#include "../Core/Strings/StringsFactory.h"
#include "../Core/Profiling/MemoryProfiler.h"

// todo: add initializing of global log, mem managers and string factory via application context

namespace Berserk
{

    class ApplicationContext
    {
    public:

        ApplicationContext();
        ~ApplicationContext();

        void Init();

        void Run();
    };

} // namespace Berserk

#endif //BERSERKENGINE_APPLICATIONCONTEXT_H
