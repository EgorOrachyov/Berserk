//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLGPUPROGRAM_H
#define BERSERKENGINE_GLGPUPROGRAM_H

#include "GLInclude.h"

#include "../Core/Essential/Types.h"
#include "../Core/Strings/CHARStaticString.h"

#include "../Core/Math/Vector2f.h"
#include "../Core/Math/Vector3f.h"
#include "../Core/Math/Vector4f.h"

#include "../Core/Math/Matrix2x2f.h"
#include "../Core/Math/Matrix3x3f.h"
#include "../Core/Math/Matrix4x4f.h"

/**
 * Types of OpenGL Shaders
 */
enum GLShaderType
{
    GLST_VERTEX             = GL_VERTEX_SHADER,
    GLST_FRAGMENT           = GL_FRAGMENT_SHADER,
    GLST_GEOMETRY           = GL_GEOMETRY_SHADER,
    GLST_TESS_CONTROL       = GL_TESS_CONTROL_SHADER,
    GLST_TESS_EVALUATION    = GL_TESS_EVALUATION_SHADER,
    GLST_COMPUTE            = GL_COMPUTE_SHADER
};

namespace Berserk
{

    /**
     * GPU OpenGL program to work with GL Shading Language. Allows to add shaders to
     * the program, compile an link it. Provides interface to pass uniform variables,
     * get locations of uniforms and attributes.
     */
    class GLGPUProgram
    {
    public:

        GLGPUProgram();
        ~GLGPUProgram();

        /**
         * Initialize gpu program descriptor
         */
        void init();

        /**
         * Unlink used shaders, destroy it and delete program handle
         */
        void destroy();

        /**
         * Load shader from file, compile and attach it to program
         *
         * @param filename Name of file with shader text
         * @param type Type of shader @see GLShaderType
         */
        void compileShader(const CHAR* filename, GLShaderType type);

        /**
         * Link all the attached shaders
         */
        void link();

        /**
         * Make this program currently used
         */
        void use();

        /**
         * Validate program to find out more info about usage
         */
        void validate();

        /**
         * Binds special location for the attribute
         *
         * @warning Call before linking
         *
         * @param location Desired location
         * @param name Attribute name in shader
         */
        void bindAttributeLocation(GLuint location, const CHAR* name);

        /**
         * Bind special location for the uniform
         *
         * @warning Call before linking
         *
         * @param location Desired location
         * @param name Uniform name in shader
         */
        void bindFragmentDataLocation(GLuint location, const CHAR* name);

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param i Value of int32
         */
        void setUniform(const CHAR* name, int32 i) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param i Value of uint32
         */
        void setUniform(const CHAR* name, uint32 i) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param f Value of float32
         */
        void setUniform(const CHAR* name, float32 f) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param v Value of 2f vec
         */
        void setUniform(const CHAR* name, const Vector2f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param v Value of 3f vec
         */
        void setUniform(const CHAR* name, const Vector3f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param v Value of 4f vec
         */
        void setUniform(const CHAR* name, const Vector4f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param m Value of 2x2 mat
         */
        void setUniform(const CHAR* name, const Matrix2x2f& m) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param m Value of 3x3 mat
         */
        void setUniform(const CHAR* name, const Matrix3x3f& m) const;

        /**
         * Push uniform to gpu program
         *
         * @param name Name of uniform variable
         * @param m Value of 4x4 mat
         */
        void setUniform(const CHAR* name, const Matrix4x4f& m) const;

        /**
         * Get location of the uniform by name
         *
         * @param name Uniform name in the shader text
         * @return Location
         */
        GLint getUniformLocation(const CHAR* name) const;

        /**
         * Get location of the attribute by name
         *
         * @param name Attribute name in the shader text
         * @return Location
         */
        GLint getAttributeLocation(const CHAR* name) const;

        /**
         * @return Is it successfully linked
         */
        bool isLinked() const;

        /**
         * @return Is it successfully created
         */
        bool isCreated() const;

        /**
         * @return Was last shader successfully compiled and attached
         */
        bool isCompiled() const;

        /**
         * Print to the console and log info about attributes
         *
         * @note debug function
         */
        void printActiveAttributes() const;

        /**
         * Print to the console and log info about uniforms
         *
         * @note debug function
         */
        void printActiveUniforms() const;

        /**
         * Print to the console and log info about uniforms in blocks
         *
         * @note debug function
         */
        void printActiveUniformsInBlocks() const;

    protected:

        /**
         * Get char* str by type id
         *
         * @param type Type of variable
         * @return Text interpretation of type
         */
        const CHAR* getStringType(GLenum type) const;

    protected:

        GLuint mHandle;             // OpenGL program descriptor
        int8 mIsCreated;            // Is program successfully created
        int8 mIsLinked;             // Are shaders successfully linked to the program
        int8 mIsCompiled;           // Was the last shader successfully compiled and attached

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLGPUPROGRAM_H