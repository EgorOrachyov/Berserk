//
// Created by Egor Orachyov on 31.01.2019.
//

#ifndef BERSERK_CORETEST_H
#define BERSERK_CORETEST_H

#include "Public/Logging/LogMacros.h"
#include "Public/Logging/LogManager.h"

#include "Public/Misc/Assert.h"
#include "Public/Misc/Include.h"
#include "Public/Misc/Alignment.h"

#include "Public/Memory/Allocator.h"
#include "Public/Memory/ListAllocator.h"
#include "Public/Memory/PoolAllocator.h"
#include "Public/Memory/StackAllocator.h"
#include "Public/Memory/LinearAllocator.h"

#include "Public/Strings/StaticString.h"
#include "Public/Strings/StringPool.h"
#include "Public/Strings/StringStream.h"
#include "Public/Strings/StringUtility.h"

#include "Public/Info/Version.h"

#include "Public/XML/XMLDocument.h"

#include "Public/Containers/HashMap.h"
#include "Public/Containers/ArrayList.h"
#include "Public/Containers/SharedList.h"
#include "Public/Containers/LinkedList.h"


void LogTest()
{
    using namespace Berserk;

    LogManager& log = LogManager::getSingleton();

    log.setFileWriting(true);

    log.addEmptyLine();
    log.setVerbosity(LogVerbosity::Display);
    log.addHeader("Test Log Verbosity 'Display'");

    FATAL("Fatal macro test");
    ERROR("Error macro test");
    WARNING("Warning macro test");
    INFO("Info macro test");
    PUSH("Push message into log");

    OPEN_BLOCK("Block test");
    PUSH_BLOCK("Message in block");
    PUSH_BLOCK("Message in block");
    PUSH_BLOCK("Message in block");
    PUSH_BLOCK("Message in block");
    CLOSE_BLOCK("Block test");

    log.addEmptyLine();
    log.setVerbosity(LogVerbosity::Warning);
    log.addHeader("Test Log Verbosity 'Warning'");

    FATAL("Fatal macro test");
    ERROR("Error macro test");
    WARNING("Warning macro test");
    INFO("Info macro test");
    PUSH("Push message into log");

    log.addEmptyLine();
    log.setVerbosity(LogVerbosity::Error);
    log.addHeader("Test Log Verbosity 'Error'");

    FATAL("Fatal macro test");
    ERROR("Error macro test");
    WARNING("Warning macro test");
    INFO("Info macro test");
    PUSH("Push message into log");

    log.addEmptyLine();
    log.setVerbosity(LogVerbosity::Fatal);
    log.addHeader("Test Log Verbosity 'Fatal'");

    FATAL("Fatal macro test");
    ERROR("Error macro test");
    WARNING("Warning macro test");
    INFO("Info macro test");
    PUSH("Push message into log");

    log.addEmptyLine();
    log.setVerbosity(LogVerbosity::NoLogging);
    log.addHeader("Test Log Verbosity 'NoLogging'");

    FATAL("Fatal macro test");
    ERROR("Error macro test");
    WARNING("Warning macro test");
    INFO("Info macro test");
    PUSH("Push message into log");

}

void AlignmentTest()
{
    int sizes[] = { 8, 16, 17, 32, 341, 299, 1024, 1025 };
    int newsize;

    printf("\nAlignment test\n");

    newsize = sizes[0];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[0], newsize);

    newsize = sizes[1];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[1], newsize);

    newsize = sizes[2];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[2], newsize);

    newsize = sizes[3];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[3], newsize);

    newsize = sizes[4];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[4], newsize);

    newsize = sizes[5];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[5], newsize);

    newsize = sizes[6];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[6], newsize);

    newsize = sizes[7];
    ALIGN(newsize);
    printf("Size: %i | Aligned: %i \n", sizes[7], newsize);

}

void AssertTest()
{
    using namespace Berserk;

    ASSERT(16 == 16, "Ups!!");
    ASSERT(12 == 12, "Ups!!");
}

void AllocatorTest()
{
    using namespace Berserk;

    struct Data {
        uint64 value[8];
    };

    void* p[64];

    printf("\nPool allocator\n");
    PoolAllocator pool(sizeof(Data), 64);
    for (int32 i = 0; i < 256; i++) {
        printf("Alloc[%i] %p | usage: %u | total: %u \n",i, pool.alloc(), pool.getUsage(), pool.getTotalSize());
    }

    printf("\nLinear allocator\n");
    LinearAllocator linear(sizeof(Data) * 64);
    for (int32 i = 0; i < 64; i++) {
        printf("Alloc[%i] %p | usage: %u | total: %u \n",i, linear.alloc(sizeof(Data)), linear.getUsage(), linear.getTotalSize());
    }

    printf("\nStack allocator\n");
    StackAllocator stack(Buffers::KiB);
    for (int32 i = 0; i < 10; i++) {
        p[i] = stack.alloc(sizeof(Data));
        printf("Alloc[%i] %p | usage: %u | total: %u \n", i, p[i], stack.getUsage(), stack.getTotalSize());
    }

    for (int32 i = 9; i >= 0; i--) {
        stack.free(p[i]);
        printf("Free[%i] | usage: %u | total: %u \n", i, stack.getUsage(), stack.getTotalSize());
    }

    printf("\nList allocator\n");
    ListAllocator list(Buffers::KiB);
    for (int32 i = 0; i < 64; i++) {
        p[i] = list.alloc(sizeof(Data));
        printf("Alloc[%i] %p | usage: %u | total: %u \n", i, p[i], list.getUsage(), list.getTotalSize());
    }

    for (int32 i = 0; i < 64; i++) {
        list.free(p[i]);
        printf("Free[%i] | usage: %u | total: %u \n", i, list.getUsage(), list.getTotalSize());
    }

    list.blocks("After free");

    printf("\nAllocator data\n");
    printf("Total mem usage: %lu | Alloc calls: %u | Free calls: %u \n",
           Allocator::getSingleton().getTotalMemoryUsage(),
           Allocator::getSingleton().getAllocCalls(),
           Allocator::getSingleton().getFreeCalls()
    );

    printf("\n");
}

void OptionTest()
{
    printf("Version: %s | %d %d \n", BERSERK_VERSION, BERSERK_VERSION_MAJOR, BERSERK_VERSION_MINOR);
}

void StringUtilityTest()
{
    using namespace Berserk;

    typedef Strings<char,'\0'> CString;

    char string1[Buffers::SIZE_256] = {'\0'};
    char string2[Buffers::SIZE_256] = {'\0'};

    char* s1 = (char*)string1;
    char* s2 = (char*)string2;

    printf("\nTest: %s \nstring1: %s\nstring2: %s\n", "Test", s1, s2);

    CString::strcat(s1, "Hello, ?");
    CString::strncat(s2, "World! How are you", 19);
    printf("\nTest: %s \nstring1: %s\nstring2: %s\n", "Append", s1, s2);

    auto l1 = CString::strlen(s1);
    auto l2 = CString::strlen(s2);
    printf("\nTest: %s \nstring1: %s length: %u\nstring2: %s length: %u\n", "Length", s1, l1, s2, l2);

    CString::strcpy(s1, "Maybe, start another test?");
    CString::strncpy(s2, "Or not...", 10);
    printf("\nTest: %s \nstring1: %s\nstring2: %s\n", "Copy", s1, s2);

    auto p1 = CString::strstr(s1, "start");
    auto p2 = CString::strstr(s2, "fly");
    printf("\nTest: %s \nstring1: %s pos: %i\nstring2: %s pos: %i\n", "Find sub string", s1, p1, s2, p2);

    CString::strcpy(s1, "Hello, world!");
    CString::strcpy(s2, "Hello, world!  How are you");

    auto cmp1 = CString::strcmp(s1, s2);
    auto cmp2 = CString::strncmp(s1, s2, CString::strlen(s1));
    printf("\nTest: %s \nstring1: %s cmp: %i\nstring2: %s cmp: %i\n", "Compare", s1, cmp1, s2, cmp2);

    CString::strins(s2, s1, 14);
    printf("\nTest: %s \nstring1: %s\nstring2: %s insert in pos: %i\n", "Insert", s1, s2, 14);

    printf("\n");
}

void StaticStringTest()
{
    using namespace Berserk;

    typedef StringStream<char,'\0',64> Name;

    Name hello("Hello, ");
    Name world("World!");
    Name long_str("Akjnksdj asdfnkjan asdfjanak ajnf;an jn;an;jk alkfnvlkfdav dfbvkf dkfjvkdfjbvkdf kfvnkdf");
    Name to_insert(" xxxxxxxx");

    auto hello_world = hello + world;
    auto abcd = hello;
    auto abcde = hello;
    auto abcdef = hello;

    abcd = "abcd";
    abcde = "abcde";
    abcdef = "abcdef";

    printf("%s %s \n", hello.get(), world.get());
    printf("%s \n", hello_world.get());
    printf("%i \n", abcd >= abcde);
    printf("%i \n", abcd <= abcde);
    printf("%i \n", hello == hello);
    printf("%i \n", abcdef == hello);
    printf("%i \n", abcdef >= abcdef);
    printf("%i \n", abcd >= abcdef);

    printf("%s \n", long_str.get());
    long_str.insert(to_insert, 18);
    printf("%s \n", long_str.get());

}

void DynamicStringTest()
{
    using namespace Berserk;

    StringPool& pool = StringPool::getSingleton();


}

void XMLTest()
{
    using namespace Berserk;

    XMLDocument document("../Engine/Config/Test/Hero.xml", ".xml");

    auto hero = document.getNode("hero");
    auto plot = hero.getAttribute("plot");
    auto location = hero.getAttribute("location");

    printf("\nXML Test\n");
    printf("Node: %s\n", hero.getName());
    printf("Attribute: name: %s value: %s\n", plot.getName(), plot.getValue());
    printf("Attribute: name: %s value: %s\n", location.getName(), location.getValue());

    for (XMLNode node = hero.getChild(); !node.isEmpty(); node = node.getNext())
    {
        printf("Node: name: %s\n", node.getName());
        for (XMLNode child = node.getChild(); !child.isEmpty(); child = child.getNext())
        {
            auto attribute = child.getFirst();
            printf("  Node: child: %s\n", child.getName());
            printf("  Attribute: name: %s value: %s \n", attribute.getName(), attribute.getValue());
        }
    }

}

void ArrayListTest()
{
    using namespace Berserk;

    ArrayList<int64> array;

    printf("\nArray List\n");

    for (int64 i = 0; i < 25; i++)
    {
        array += (i * i);
        printf("Value[%i] = %li\n", i, array[i]);
    }

    auto i = 0;
    for (auto item = array.iterate(); item; item = array.next())
    {
        printf("Value[%u] = %li\n", i++, *item);
    }
}

void SharedListTest()
{
    using namespace Berserk;

    printf("\nShared List\n");

    PoolAllocator pool(PoolAllocator::MIN_CHUNK_SIZE, PoolAllocator::MIN_CHUNK_COUNT);
    SharedList<int64> list(&pool);

    for (int64 i = 0; i < 13; i++)
    { list += i * i * i; }

    for (int64 i = 1; i < 13; i++)
    { list.addHead(i * i * i); }

    for (uint32 i = 0; i < list.getSize(); i++)
    { printf("Value[%u] = %li\n", i, list[i]); }

    for (uint32 i = 0; i < 5; i++)
    { list.remove(i); }

    for (uint32 i = 0; i < list.getSize(); i++)
    { printf("Value[%u] = %li\n", i, list[i]); }

    printf("Elements count: %u | Total: %u \n", list.getSize(), list.getTotalSize());
}

void LinkedListTest()
{
    using namespace Berserk;

    printf("\nLinked List\n");

    LinkedList<int64> list;

    for (int64 i = 0; i < 13; i++)
    { list += i * i * i; }

    for (int64 i = 1; i < 13; i++)
    { list.addHead(i * i * i); }

    for (uint32 i = 0; i < list.getSize(); i++)
    { printf("Value[%u] = %li\n", i, list[i]); }

    for (uint32 i = 0; i < 5; i++)
    { list.remove(i); }

    uint32 i = 0;
    for (auto item = list.iterate(); item; item = list.next())
    { printf("Value[%u] = %li\n", i++, *item); }

    printf("Elements count: %u | Total: %u \n", list.getSize(), list.getTotalSize());
}

Berserk::uint32 hashing(Berserk::CName& key)
{
    return Berserk::Crc32::hash(key.get(), key.length());
}

void HashMapTest()
{
    using namespace Berserk;

    printf("\nHash Map\n");

    HashMap<CName, uint64> map((Crc32::Hashing)hashing);

    map.add(CName("TextureSpecular"), 0);
    map.add(CName("TextureDiffuse"), 2);
    map.add(CName("TextureNormal"), 6);
    map.add(CName("TextureBump"), 1);

    printf("\n");
    printf("Key: %s | Value: %lu \n", "TextureBump",     *map[CName("TextureBump")]);
    printf("Key: %s | Value: %lu \n", "TextureNormal",   *map[CName("TextureNormal")]);
    printf("Key: %s | Value: %lu \n", "TextureDiffuse",  *map[CName("TextureDiffuse")]);
    printf("Key: %s | Value: %lu \n", "TextureSpecular", *map[CName("TextureSpecular")]);

    for (auto e = map.iterate(); e; e = map.next())
    {
        printf("Iterate: Key: %s | Value: %lu \n", map.key()->get(), *map.value());
    }

    printf("\n");
    printf("Remove: Key: %s \n", "TextureBump");   map.remove(CName("TextureBump"));
    printf("Remove: Key: %s \n", "TextureNormal"); map.remove(CName("TextureNormal"));

    for (auto e = map.iterate(); e; e = map.next())
    {
        printf("Iterate: Key: %s | Value: %lu \n", map.key()->get(), *map.value());
    }

    printf("\n");
    printf("Rewrite: Key: %s \n", "TextureDiffuse");   map.add(CName("TextureDiffuse"), 14);
    printf("Add:     Key: %s \n", "TextureNormal");    map.add(CName("TextureNormal"),  17);

    for (auto e = map.iterate(); e; e = map.next())
    {
        printf("Iterate: Key: %s | Value: %lu \n", map.key()->get(), *map.value());
    }
}

#endif //BERSERK_CORETEST_H
