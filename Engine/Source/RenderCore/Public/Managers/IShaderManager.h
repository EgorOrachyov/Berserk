//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_ISHADERMANAGER_H
#define BERSERK_ISHADERMANAGER_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Platform/IShader.h"
#include "XMLNode.h"
#include "MemorySizer.h"

namespace Berserk::Resources
{

    /**
     * An interface which provides access to the shader manager implemented in 3D Rendering System.
     * Responsible for creating and loading shaders in OS / Engine system. Also provides reliable
     * mechanism for importing shaders in real-time mode from meta-info.xml files.
     * Handles reference counting and memory allocation for resources.
     */
    class ENGINE_API IShaderManager
    {
    public:

        /** De-initialize manager */
        virtual ~IShaderManager() = default;

        /** Rename chosen shader with new name */
        virtual void renameShader(IShader* shader, const char* name) = 0;

        /** Delete specified resource */
        virtual void deleteShader(IShader* shader) = 0;

        /** @return New empty (if it does not exist) shader with specified name */
        virtual IShader* createShader(const char* name) = 0;

        /** @return Shader with specified name */
        virtual IShader* findShader(const char* name) = 0;

        /** @return Pointer to resource with incrementing reference count */
        virtual IShader* getShader(const char* name) = 0;

        /** @return Shader from .mtl file with specified name (in path) */
        virtual IShader* loadShader(const char* path) = 0;

        /** @return Loaded shader with specified name from XML node */
        virtual IShader* loadShaderFromXML(const char* name, XMLNode& node) = 0;

        /** @return Memory usage on CPU (RAM) side */
        virtual uint32 getMemoryUsage() = 0;

        /** Profile memroy usage by this manager */
        virtual void getMemoryUsage(MemorySizer* sizer) = 0;

    };

} // namespace Berserk::Resources

#endif //BERSERK_ISHADERMANAGER_H