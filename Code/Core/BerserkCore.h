/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_BERSERKCORE_H
#define BERSERK_BERSERKCORE_H

#include <Console/ConsoleManager.h>
#include <IO/Archive.h>
#include <IO/ArchiveFile.h>
#include <IO/Json.h>
#include <IO/Ini.h>
#include <IO/ResourceImporter.h>
#include <IO/OutputDevice.h>
#include <Math/Aabbf.h>
#include <Math/Mat4x4f.h>
#include <Math/Vec4f.h>
#include <Math/Random.h>
#include <Math/Quatf.h>
#include <Math/TRange.h>
#include <Math/Region2i.h>
#include <Platform/System.h>
#include <Platform/Atomic.h>
#include <Platform/Input.h>
#include <Threading/ThreadOld.h>
#include <String/CString.h>
#include <String/CStringStatic.h>
#include <String/CStringBuilder.h>
#include <Containers/TArrayStatic.h>
#include <Containers/TArray.h>
#include <Containers/TMap.h>
#include <Containers/TSet.h>
#include <ErrorMacro.h>
#include <TPtrShared.h>
#include <TPtrUnique.h>
#include <TRef.h>
#include <TimeValue.h>
#include <UUID.h>
#include <Image.h>
#include <Engine.h>

#endif //BERSERK_BERSERKCORE_H
