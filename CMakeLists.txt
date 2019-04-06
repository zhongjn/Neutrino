# CMakeList.txt : CMake project for Neutrino, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (
  Neutrino 
  "Nullable.h"
  "Property.h"
  "CredentialInfo.h"
  "Mail.h" 
  "MailAddress.h" 
  "MailManager.h"
  "Main.cpp"
)

# TODO: Add tests and install targets if needed.
