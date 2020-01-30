/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Format.h>
#include <TestMacro.h>
#include <ErrorMacro.h>
#include <Platform/Memory.h>
#include <Math/Math.h>
#include <Math/Point2i.h>
#include <TPtrUnique.h>
#include <AllocPool.h>
#include <TArray.h>
#include <TimeValue.h>
#include <UUID.h>

#include <iostream>

using namespace Berserk;

void* operator new(size_t size) {
    printf("Allocate (by new): %lu\n", size);
    return malloc(size);
}

BERSERK_TEST_SECTION(CoreMinimals)
{
    BERSERK_TEST(Format)
    {
        const uint32 bufferSize = 30;
        char buffer[bufferSize] = {0};
        uint32 written = 0;

        written = Format::printf(buffer, bufferSize, "Message");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %f %s", 0.345f, "TString");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %s", "abcabcabcabcabcabcabcabc");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %20s", "Aligned");
        printf("%u %s\n", written, buffer);

        written = Format::printf(buffer, bufferSize, "Message %9.5f", 1241.001f);
        printf("%u %s\n", written, buffer);
    };

    BERSERK_TEST(Assert)
    {
        BERSERK_EXPECT_TRUE(1 > 0)
        BERSERK_EXPECT_TRUE(10 - 12 < -100)
        BERSERK_EXPECT_FALSE(100 * 0 == 10)
        BERSERK_EXPECT_FALSE(true)
    };

    BERSERK_TEST(Pointers)
    {
        TPtrUnique<uint64> i1 = objectNew<uint64>(10);
        TPtrUnique<uint64> i2 = objectNew<uint64>(15);

        auto r = *i1 + *i2;

        printf("%llu %llu %llu \n", r, *i1, *i2);

        auto i3 = i1;
        auto i4 = i2;

        printf("%p %p %i %i\n", i3.getPtr(), i4.getPtr(), i3 > i4, i3 < i4);
    };

    BERSERK_TEST(AllocPool)
    {
        AllocPool pool1(sizeof(uint64), 1);

        for (uint32 i = 0; i < 10; i++) {
            auto p = pool1.allocate(sizeof(uint64));
            printf("[%u]=%p\n", i, p);
        }
    };

    BERSERK_TEST(AllocStat)
    {
        auto allocs = Memory::getAllocCalls();
        auto frees = Memory::getFreeCalls();
        printf("Alloc calls: %llu Free calls: %llu \n", allocs, frees);
    };

    BERSERK_TEST(Function)
    {
        struct Data {
            uint32 buffer[6];
        };

        Data d1;
        Data d2;

        auto l1 = [&](){
            d1.buffer[0] = 1;
        };

        auto l2 = [=](){
            auto i = d1.buffer[0];
        };

        Function<void()> f1 = l1;
        Function<void()> f2 = l2;

        printf("Sizeof %lu\n", sizeof(Function<void()>));
    };

    BERSERK_TEST(TimeValue)
    {
        TimeValue start = TimeValue::now();

        //int32 i;
        //std::cin >> i;

        TimeValue end = TimeValue::now();

        printf("Sizeof(TimeValue): %u\n", (uint32) sizeof(TimeValue));
        printf("Time: %lfs\n", (end - start).getSeconds());
        printf("Time: %lfms\n", (end - start).getMilliseconds());
    };

    BERSERK_TEST(UUID)
    {
        UUID null = UUID::generateNull();
        printf("UUID: %s Empty? %i\n", null.toString().data(), null.isNull());

        UUID notNull = UUID::generate();
        printf("UUID: %s Empty? %i\n", notNull.toString().data(), notNull.isNull());

        const uint32 N = 10;
        UUID ids[N];

        for (auto& id: ids) {
            id = UUID::generate();

            auto asString = id.toString();
            auto fromString = UUID(asString);

            BERSERK_EXPECT_TRUE(id == fromString)
        }

        CStringStatic fromFile = "4d7b94bf58ba33b510848b6c76ebad52";
        UUID fromString = fromFile;

        printf("UUID from file: %s\n", fromString.toString().data());
    };

    BERSERK_TEST(SoftwereRenderer)
    {
        struct Canvas {
            char back = 0;
            char pen = 0;
            uint32 w = 80;
            uint32 h = 60;
            TArray<char> buffer;

            Canvas(uint32 width, uint32 height) {
                w = width;
                h = height;

                buffer.ensureCapacity(w * h);

                for (uint32 i = 0; i < w * h; i++) {
                    buffer.data()[i] = back;
                }
            }

            Canvas() {
                buffer.ensureCapacity(w * h);

                for (uint32 i = 0; i < w * h; i++) {
                    buffer.data()[i] = back;
                }
            }

            void setColor(char color) {
                pen = color;
            }

            void setFillColor(char color) {
                back = color;
            }

            void setPixel(uint32 x, uint32 y, char value) {
                if (x >= w) return;
                if (y >= h) return;

                uint32 index = y * w + x;
                buffer.data()[index] = value;
            }

            void setPixel(uint32 x, uint32 y) {
                if (x >= w) return;
                if (y >= h) return;

                uint32 index = y * w + x;
                buffer.data()[index] = pen;
            }

            void drawCircle(const Point2i& center, uint32 radius) {
                int32 x = 0;
                int32 y = radius;
                int32 d = 3 - 2 * radius;
                int32 cX = center[0];
                int32 cY = center[1];

                while (x < y) {
                    setPixel( x+cX,  y+cY);
                    setPixel( y+cX,  x+cY);
                    setPixel(-y+cX,  x+cY);
                    setPixel(-x+cX,  y+cY);
                    setPixel(-x+cX, -y+cY);
                    setPixel(-y+cX, -x+cY);
                    setPixel( y+cX, -x+cY);
                    //setPixel( x+cX, -y+cY);

                    if (d < 0) {
                        x += 1;
                        d = d + 4 * x + 6;
                    }
                    else {
                        x += 1;
                        y -= 1;
                        d = d + 4 * (x - y) + 10;
                    }
                }
            }

            void drawCircle(const Point2i& center, uint32 radius, uint32 width) {
                int32 x = 0;
                int32 y = radius;
                int32 d = 3 - 2 * radius;
                int32 cX = center[0];
                int32 cY = center[1];

                while (x < y) {
                    for (uint32 i = 0; i < width; i++) {
                        int32 yi = y - i;

                        setPixel( x+cX,  yi+cY);
                        setPixel( yi+cX,  x+cY);
                        setPixel(-yi+cX,  x+cY);
                        setPixel(-x+cX,  yi+cY);
                        setPixel(-x+cX, -yi+cY);
                        setPixel(-yi+cX, -x+cY);
                        setPixel( yi+cX, -x+cY);
                        setPixel( x+cX, -yi+cY);
                    }

                    if (d < 0) {
                        x += 1;
                        d = d + 4 * x + 6;
                    }
                    else {
                        x += 1;
                        y -= 1;
                        d = d + 4 * (x - y) + 10;
                    }
                }
            }

            void drawRect(const Point2i& topLeft, uint32 width, uint32 height) {
                uint32 y1 = topLeft[1];
                uint32 y2 = topLeft[1] + height - 1;

                for (uint32 i = topLeft[0]; i < topLeft[0] + width; i++) {
                    setPixel(i, y1);
                    setPixel(i, y2);
                }

                uint32 x1 = topLeft[0];
                uint32 x2 = topLeft[0] + width - 1;

                for (uint32 i = topLeft[1]; i < topLeft[1] + height; i++) {
                    setPixel(x1, i);
                    setPixel(x2, i);
                }
            }

            void out() const {
                for (uint32 y = 0; y < h; y++) {
                    for (uint32 x = 0; x < w; x++) {
                        auto c = buffer.data()[y * w + x];
                        if (c) printf("%c", c);
                        else printf(" ");
                    }
                    printf("\n");
                }
            }

            void fill() {
                Memory::set(buffer.data(), back, sizeof(char) * w * h);
            }

            void fill(char color) {
               for (uint32 i = 0; i < w * h; i++) {
                   buffer.data()[i] = color;
               }
            }

            void fillRect(const Point2i& topLeft, uint32 width, uint32 height) {
                auto s = Point2i::clamp(topLeft, Point2i(0,0), Point2i(w,h));
                auto l = Math::min(width, w - s[0]);

                for (uint32 y = s[1]; y < Math::min(s[1] + height, h); y++) {
                    Memory::set(buffer.data() + y * w + s[0], back, sizeof(char) * l);
                };
            }
        };

        Canvas canvas;
        canvas.setFillColor('o');
        //canvas.fillRect({ 0, 0 }, 80, 60);
        canvas.setColor('x');
        //canvas.drawCircle({ 10, 10 }, 10);
        //canvas.drawCircle({ 20, 20 }, 10);
        canvas.drawCircle({ 30, 30 }, 20, 2);
        //canvas.drawCircle({ 40, 40 }, 10);
        canvas.drawRect({5,5},   5, 5);
        canvas.drawRect({60,5},  5, 5);
        canvas.drawRect({5,40},  5, 5);
        canvas.drawRect({60,40}, 5, 5);
        //canvas.out();

        Canvas hd(1280,720);
        TimeValue start = TimeValue::now();
        canvas.fillRect({ 0, 0 }, 640, 360);
        canvas.fillRect({ 640, 360 }, 640, 360);
        TimeValue delta = TimeValue::now() - start;
        printf("Time: %fms\n", delta.getMilliseconds());

    };

}