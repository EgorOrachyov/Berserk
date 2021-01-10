/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Defines.hpp>
#include <BerserkCore/Assert.hpp>
#include <BerserkCore/Platform/Memory.hpp>

#define STBIW_ASSERT(x) BERSERK_ASSERT(x)

#define STBIW_MALLOC(sz)         Berserk::Platform::Memory::Allocate(sz)
#define STBIW_REALLOC(p,newsz)   Berserk::Platform::Memory::Reallocate(p, newsz)
#define STBIW_FREE(p)            Berserk::Platform::Memory::Deallocate(p)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stbimage/stb_image_write.hpp>