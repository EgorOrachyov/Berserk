/**********************************************************************************/
/* This file is part of:                                                          */
/* Berserk Engine project                                                         */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018-2019 Egor Orachyov                                          */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERKTESTS_TESTMACRO_H
#define BERSERKTESTS_TESTMACRO_H

#define TEST_RUN()                                  static void run()
#define TEST_FUNCTION_OBJECT(object)                static void object##test()
#define TEST_FUNCTION_CALL(object)                  printf("\n%s\n\n", #object); object##test(); \
                                                    printf("\n/////////////////////////////////////////////////\n");
#define TEST_FUNCTION_CALL_DESC(object, desc)       printf("%s: %s\n", #object, desc); object##test();
#define TEST_SECTION_NAME(name)                     printf("\n/////////////////////////////////////////////////\n"); \
                                                    printf("// %29s               //\n", name); \
                                                    printf("/////////////////////////////////////////////////\n"); \
                                                    printf("\n/////////////////////////////////////////////////\n");
#define TEST_MALLOC_STAT()                          printf("Alloc calls: %u | Free calls: %u\n", \
                                                    Alloc::getSingleton().getCallsAllocate(), \
                                                    Alloc::getSingleton().getCallsFree());


#endif //BERSERKTESTS_TESTMACRO_H
