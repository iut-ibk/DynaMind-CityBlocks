file(GLOB MODULE_CPPS *.cpp)
file(GLOB MODULE_H *.h)

include_directories(.)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(cityblock SHARED ${MODULE_CPPS} ${MODULE_H})

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        SET_TARGET_PROPERTIES(cityblock PROPERTIES COMPILE_FLAGS "-frounding-math")
ENDIF()

target_link_libraries(cityblock ${DYNAMIND_TOOLBOX_LIBRARY} ${DYNAMIND_EXTENSIONS_LIBRARY}  ${DYNAMIND_CORE_LIBRARY} ${QT_LIBRARIES})

INSTALL(TARGETS cityblock
        DESTINATION "lib/DynaMind"
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


