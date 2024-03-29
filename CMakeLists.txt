cmake_minimum_required(VERSION 3.27.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE RELEASE)

project(Game)

file(GLOB src src/*.cpp)

add_executable(${PROJECT_NAME} ${src})

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-mingw-dynamic/share")

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

# find_package(SDL2_ttf CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

find_package(SDL2_mixer CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)

# find_package(sdl2-gfx CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_gfx)

find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE PNG::PNG)

find_package(tinyxml2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)

find_package(tmxparser REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE tmxparser)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE JsonCpp::JsonCpp)

target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

# add_subdirectory(tests)