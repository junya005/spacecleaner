cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(QTSRPG CXX)

set(TARGET_NAME QTSRPG)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options("/utf-8")
endif()

set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets")

# 依存パッケージの検索
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME})

# ソースコードディレクトリの追加
add_subdirectory(engine)
add_subdirectory(src)

# ターゲットにライブラリをリンク
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    glad::glad
    imgui::imgui
    assimp::assimp
    OpenGL::GL
)

# 注意: assetsディレクトリが存在しない場合、コピーは失敗します。事前にassetsディレクトリを作成してください。
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_SRC_DIR}"
            "${ASSETS_DEST_DIR}"
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()
