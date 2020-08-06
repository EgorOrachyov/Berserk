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

    class FontImportOptions : public ResourceImportOptions {
    public:
        void setFontName(CString name) { fontName = std::move(name); }
        void setFontSize(const Size2i &size) { fontSize = size; }
        const CString& getFontName() const { return fontName; }
        const Size2i& getFontSize() const { return fontSize; }
        bool getAutoWidth() const { return autoWidth; }
    private:
        CString fontName;
        Size2i fontSize = { 10, 10 };
        bool autoWidth = true;
    };

}

#endif //BERSERK_FONTIMPORTOPTIONS_H
