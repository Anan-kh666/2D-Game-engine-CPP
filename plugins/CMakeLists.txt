# This list will hold the targets for all detected plugins
set(FORGE2D_PLUGINS "" CACHE INTERNAL "List of Forge2D Plugins")

# Scan the plugins directory for any subdirectories containing a CMakeLists.txt
file(GLOB PLUGIN_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(dir ${PLUGIN_DIRS})
    if(IS_DIRECTORY ${dir})
        if(EXISTS "${dir}/CMakeLists.txt")
            get_filename_component(PLUGIN_NAME ${dir} NAME)
            message(STATUS "Found Forge2D Premium Plugin: ${PLUGIN_NAME}")
            
            # Add the plugin subdirectory
            add_subdirectory(${dir})
            
            # Append to the list of plugin targets 
            # (Assuming the plugin target name matches its folder name)
            set(FORGE2D_PLUGINS ${FORGE2D_PLUGINS} ${PLUGIN_NAME} CACHE INTERNAL "List of Forge2D Plugins")
        endif()
    endif()
endforeach()
