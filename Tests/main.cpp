#include "CoreSystem/FilesTest.h"
#include "CoreSystem/ArrayTest.h"
#include "CoreSystem/LinkedListTest.h"
#include "CoreSystem/LoggingTest.h"
#include "CoreSystem/ResourcesTest.h"
#include "CoreSystem/MemoryTest.h"
#include "CoreSystem/StringsTest.h"
#include "CoreSystem/ContainersTest.h"
#include "CoreSystem/ThreadingTest.h"
#include "CoreSystem/ConsoleTest.h"
#include "CoreSystem/MathTest.h"
#include "RapidXMLTest/RapidXMLTest.h"
#include "EngineSystem/GlfwWindowSystemTest.h"
#include "EngineSystem/ObjectsTest.h"
#include "EngineSystem/RenderingTest.h"
#include "EngineSystem/SerializeTest.h"
#include "OpenGL/GLRHITest.h"

int main()
{
    //ArrayTest::run();
    //LinkedListTest::run();
    //FilesTest::run();
    //LoggingTest::run();
    //ResourcesTest::run();
    //MemoryTest::run();
    //StringsTest::run();
    //ContainersTest::run();
    //ThreadingTest::run();
    //ConsoleTest::run();
    //MathTest::run();
    //RapidXMLTest::run();
    //GlfwWindowSystemTest::run();
    //GLRHITest::run();
    //ObjectsTest::run();
    RenderingTest::run();
    //SerializeTest::run();

    return 0;
}