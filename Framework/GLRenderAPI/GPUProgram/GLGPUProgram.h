//
// Created by Egor Orachyov on 04.07.2018.
//

#ifndef BERSERKENGINE_GLGPUPROGRAM_H
#define BERSERKENGINE_GLGPUPROGRAM_H

#include "Essential/GLShaderType.h"

#include "Essential/Types.h"

#include "Math/Vector2f.h"
#include "Math/Vector3f.h"
#include "Math/Vector4f.h"

#include "Math/Matrix2x2f.h"
#include "Math/Matrix3x3f.h"
#include "Math/Matrix4x4f.h"

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
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param i Value of int32
         */
        void setUniform(GLint location, int32 i) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param i Value of uint32
         */
        void setUniform(GLint location, uint32 i) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param f Value of float32
         */
        void setUniform(GLint location, float32 f) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param v Value of 2f vec
         */
        void setUniform(GLint location, const Vector2f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param v Value of 3f vec
         */
        void setUniform(GLint location, const Vector3f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param v Value of 4f vec
         */
        void setUniform(GLint location, const Vector4f& v) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param m Value of 2x2 mat
         */
        void setUniform(GLint location, const Matrix2x2f& m) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param m Value of 3x3 mat
         */
        void setUniform(GLint location, const Matrix3x3f& m) const;

        /**
         * Push uniform to gpu program
         *
         * @param location Location of uniform variable
         * @param m Value of 4x4 mat
         */
        void setUniform(GLint location, const Matrix4x4f& m) const;

        /**
         *
         * @param type
         * @param count
         * @param indices
         */
        void setSubroutines(GLShaderType type, GLsizei count, GLuint* indices) const;

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
         *
         * @param name
         * @param type
         * @return
         */
        GLuint getSubroutineIndex(const CHAR* name, GLShaderType type) const;

        /**
         *
         * @param name
         * @param type
         * @return
         */
        GLint getSubroutineUniformLocation(const CHAR* name, GLShaderType type) const;

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
         * Check location
         *
         * @param location Location of the shader param
         * @return True if location is valid (exist in shader)
         */
        bool isLocationValid(GLint location) const;

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

        /**
         * Get char* std by shader type
         *
         * @param type Type of the shader
         * @return Text interpretation of shader extension
         */
        const CHAR* getStringExtension(GLShaderType type) const;

    protected:

        GLuint mHandle;             // OpenGL program descriptor
        int8 mIsCreated;            // Is program successfully created
        int8 mIsLinked;             // Are shaders successfully linked to the program
        int8 mIsCompiled;           // Was the last shader successfully compiled and attached

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLGPUPROGRAM_H