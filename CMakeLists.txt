cmake_minimum_required(VERSION 2.4)

PROJECT( neat )

FILE(GLOB neat_sources *.cpp)

ADD_EXECUTABLE( neat ${neat_sources} )

# new stuff added by Alby
#   - automatically adds SFML when Xcode is the generator
#   - sets flags for C++11 if appropriate (SFML 2)
MESSAGE("-- Generator: ${CMAKE_GENERATOR}")
IF(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    MESSAGE("-- Platform: OSX")
    # set compiler to use C++11
    IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
        # add SFML
        # from https://github.com/LaurentGomila/SFML/wiki/Tutorial%3A-Build-your-SFML-project-with-CMake
        SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
        MESSAGE("-- Checking for SFML 2 or higher...")
        FIND_PACKAGE(SFML 2 COMPONENTS system window graphics)
        IF(SFML_FOUND)
            INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
            TARGET_LINK_LIBRARIES(neat ${SFML_LIBRARIES})
#            SET(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
#            SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
#            SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
        ELSE()
            MESSAGE("-- Checking for an earlier version of SFML...")
            FIND_PACKAGE(SFML 1 REQUIRED system window graphics)
            IF(SFML_FOUND)
                INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})
                TARGET_LINK_LIBRARIES(neat ${SFML_LIBRARIES})
            ENDIF()
        ENDIF()
    ELSEIF(${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -g")
    ENDIF()
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    MESSAGE("-- Platform: Linux")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -g")
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Visual Studio 11")
    MESSAGE("-- Platform: Windows")
ENDIF()

