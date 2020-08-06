/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FONTIMPORTOPTIONS_H
#define BERSERK_FONTIMPORTOPTIONS_H

#include <IO/ResourceImportOptions.h>
#include <String/CString.h>
#include <Math/Point2i.h>

namespace Berserk {

    /** Used to import font */
    class FontImportOptions : public ResourceImportOptions {
    public:
        /** Set font size in pixels */
        void setFontSize(const Size2i &size) { fontSize = size; }

        /** Set font height in pixels */
        void setFontHeight(uint32 h) { fontSize[1] = h; }

        /** Set dynamic width calculation for the font */
        void setDynamicWidth(bool enable) { dynamicWidth = true; }

        /** @return True if width is dynamically computed */
        bool getIsAutoWidth() const { return dynamicWidth; }

        /** @return True if load ascii symbols */
        bool getIsAsciiCharSet() const { return asciiCharSet; }

        /** @return Font size in pixels */
        const Size2i& getFontSize() const { return fontSize; }

    private:
        Size2i fontSize = { 40, 40 };
        bool dynamicWidth = true;
        bool asciiCharSet = true;
    };

}

#endif //BERSERK_FONTIMPORTOPTIONS_H
