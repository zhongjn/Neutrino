cmake_minimum_required (VERSION 3.8)

add_subdirectory(ThirdParty/mailio)
add_subdirectory(ThirdParty/sqlite3)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

aux_source_directory(. src)
aux_source_directory(./UI src_ui)

# Add source to this project's executable.
add_executable(
  Neutrino 
  ${src}
  ${src_ui}
)

target_link_libraries(
  Neutrino
  mailio
  sqlite3
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
)
