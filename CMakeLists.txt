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

set(UI 
  UI/inbox.cpp
  UI/inbox.ui
  UI/mainwindow.cpp
  UI/mainwindow.ui
  UI/send.cpp
  UI/send.ui
)

# Add source to this project's executable.
add_executable(
  Neutrino 
  "Nullable.h"
  "Property.h"
  "CredentialInfo.cpp"
  "Folder.cpp"
  "IPEndPoint.cpp"
  "Mail.cpp" 
  "MailAddress.cpp"
  "MailManager.cpp"
  "Main.cpp"
  ${UI}
)

target_link_libraries(
  Neutrino
  mailio
  sqlite3
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
)
