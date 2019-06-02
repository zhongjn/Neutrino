cmake_minimum_required (VERSION 3.8)

add_subdirectory(ThirdParty/mailio)
add_subdirectory(ThirdParty/sqlite3)

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
)

target_link_libraries(Neutrino mailio sqlite3)