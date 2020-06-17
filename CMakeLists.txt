cmake_minimum_required (VERSION 2.6)

project (bot)
set (bot_VERSION_MAJOR 0)
set (bot_VERSION_MINOR 1)
set (bot_VERSION_PATCH 0)
set (bot_VERSION_STRING
	${bot_VERSION_MAJOR}.${bot_fileio_VERSION_MINOR}.${bot_fileio_VERSION_PATCH} )

set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib /usr/lib /usr/local/lib )
set( CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /usr/include /usr/local/include )
set( CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" ) 

set ( INSTALL_BIN_PATH /usr/local/bin )
set ( INSTALL_LIB_PATH /usr/local/lib )
set ( INSTALL_AR_PATH  /usr/local/lib )
set ( INSTALL_HEADERS_PATH /usr/local/include )

set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib") 

# Make relative paths absolute (needed later on)

foreach(p LIB BIN HEADERS AR)
        set(var INSTALL_${p}_PATH)
        if(NOT IS_ABSOLUTE "${${var}}")
                set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
        endif()
endforeach()

find_package(discord) 

if ( discord_FOUND )
    message( STATUS "Found libdiscord" ) 
else() 
    message( FATAL_ERROR "Error : discord library not found" )
endif()


### Set include directories 
        
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	                 "${CMAKE_CURRENT_BINARY_DIR}" 
                     ) 

### File lists

file( GLOB src  "src/*.c" )

### Targets

add_executable( "bot" ${src} ) 

### Linking

target_link_libraries( "bot" 
                     "${DISCORD_LIBRARY}" curl )


### Install 

# install( TARGETS "bot"
#         RUNTIME DESTINATION ${INSTALL_BIN_PATH}  COMPONENT main
#         LIBRARY DESTINATION ${INSTALL_LIB_PATH}  COMPONENT main
#         ARCHIVE DESTINATION ${INSTALL_AR_PATH}   COMPONENT main ) 
# 
