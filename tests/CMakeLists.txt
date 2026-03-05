add_executable(EngineTests
    main.cpp
    MathTests.cpp
)

target_link_libraries(EngineTests PRIVATE gtest gmock EngineCore)

include(GoogleTest)
gtest_discover_tests(EngineTests)
