cmake_minimum_required (VERSION 3.8)

#cmake_policy(SET CMP0074 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(ThirdParty/mailio)

# Add source to this project's executable.
add_executable (
  Neutrino 
  "Nullable.h"
  "Property.h"
  "CredentialInfo.cpp"
  "Mail.cpp" 
  "MailAddress.cpp" 
  "MailManager.cpp"
  "Main.cpp"
)

target_link_libraries(Neutrino mailio)

