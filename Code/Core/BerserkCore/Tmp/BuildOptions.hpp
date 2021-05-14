#ifndef BERSERK_BUILDOPTIONS_HPP
#define BERSERK_BUILDOPTIONS_HPP

/** Standard build mode with debug features and verbose data descriptions */
#define BERSERK_DEBUG

/** Defined if must build as release code without debug features */
/* #undef BERSERK_RELEASE */

/** Defined if must build editor features  */
/* #undef BERSERK_EDITOR */

/** Defined on MACOS  platform */
/* #undef BERSERK_TARGET_MACOS */

/** Defined on LINUX  platform */
#define BERSERK_TARGET_LINUX

/** Defined if must build OpenGL RHI device for rendering features */
#define BERSERK_WITH_OPENGL

/** Defined if uses WhereAmI library for executable path detection */
#define BERSERK_WITH_WHEREAMI

#endif // BERSERK_BUILDOPTIONS_HPP
