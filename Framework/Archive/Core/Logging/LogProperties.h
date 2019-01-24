//
// Created by Egor Orachyov on 25.06.2018.
//

#ifndef BERSERKENGINE_LOGPROPERTIES_H
#define BERSERKENGINE_LOGPROPERTIES_H

namespace Berserk
{

    /**
     *  Types of messages (defines how the text will be marked in the log file)
     */
    enum LogMessageType
    {
        LMT_INFO = 0,
        LMT_WARNING = 1,
        LMT_ERROR = 2
    };

} // namespace Berserk

#endif //BERSERKENGINE_LOGPROPERTIES_H
