cmake_minimum_required(VERSION 3.1)
project(BerserkTestsEngine VERSION 1.0)

add_executable(TestEngine TestEngine.cpp)
target_link_libraries(TestEngine PUBLIC BerserkMain)

add_executable(TestRenderCore TestRenderCore.cpp)
target_link_libraries(TestRenderCore PUBLIC BerserkMain)

add_executable(TestRenderBase TestRenderBase.cpp)
target_link_libraries(TestRenderBase PUBLIC BerserkMain)

add_executable(TestRenderBatch TestRenderBatch.cpp)
target_link_libraries(TestRenderBatch PUBLIC BerserkMain)

add_executable(TestRenderGraphics TestRenderGraphics.cpp)
target_link_libraries(TestRenderGraphics PUBLIC BerserkMain)

add_executable(TestRenderMesh TestRenderMesh.cpp)
target_link_libraries(TestRenderMesh PUBLIC BerserkMain)

add_executable(TestConsole TestConsole.cpp)
target_link_libraries(TestConsole PUBLIC BerserkMain)