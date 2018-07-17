//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERKENGINE_SYSTEM_H
#define BERSERKENGINE_SYSTEM_H

#include "Essential/Types.h"
#include "Strings/CString.h"
#include "Config/ConfigTable.h"

namespace Berserk
{

    class System
    {
    public:

        virtual void init(const ConfigTable& table) = 0;

        virtual void destroy() = 0;

        virtual void validate() = 0;

        virtual void preMainLoop() = 0;

        virtual void preUpdate() = 0;

        virtual void postUpdate() = 0;

        virtual void postMainLoop() = 0;

        virtual const CString& getName() const = 0;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SYSTEM_H