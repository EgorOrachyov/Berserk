//
// Created by Egor Orachyov on 09.06.2018.
//

#ifndef BERSERKENGINE_UTILITYSTRING_H
#define BERSERKENGINE_UTILITYSTRING_H

#include "CStaticString.h"
#include "WStaticString.h"

#include "CString.h"
#include "WString.h"

namespace Berserk
{

    /** Utility label @see CString */
    typedef CString CText;

    /** Utility label @see WString */
    typedef WString WText;

    /** Utility label @see CStaticString */
    typedef CStaticString CName;

    /** Utility label @see WStaticString */
    typedef WStaticString WName;

    #define CNAME(text) CStaticString(text)

    #define WNAME(text) WStaticString(L ## text)

    #define CTEXT(text) CString(text)

    #define WTEXT(text) WString(L ## text)

} // namespace Berserk

#endif //BERSERKENGINE_UTILITYSTRING_H
