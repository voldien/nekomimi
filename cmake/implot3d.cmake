INCLUDE(FetchContent)

IF(NOT TARGET implot3d)
    FetchContent_Declare(implot3d_source
        GIT_REPOSITORY https://github.com/pthom/implot3d.git
        GIT_TAG v0.4
    )

    FetchContent_GetProperties(implot3d_source)

    IF(NOT implot3d_source_POPULATED)
        FetchContent_Populate(implot3d_source)

        ADD_LIBRARY(implot3d INTERFACE
            ${implot3d_source_SOURCE_DIR}/implot3d.cpp
            ${implot3d_source_SOURCE_DIR}/implot3d_items.cpp
            ${implot3d_source_SOURCE_DIR}/implot3d_meshes.cpp
        )

        TARGET_COMPILE_DEFINITIONS(implot3d INTERFACE IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
        TARGET_INCLUDE_DIRECTORIES(implot3d INTERFACE
            ${implot3d_source_SOURCE_DIR}
            $<BUILD_INTERFACE:${implot3d_source_SOURCE_DIR}>
        )
    ELSE()
        MESSAGE(WARNING "Could not find implot3D source code")
    ENDIF()
ENDIF()
