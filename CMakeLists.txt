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

# 要复制到执行目录的文件
set(copy_files
)

# 要复制到执行目录的目录
set(copy_diretories
	spambase
	attachments
)


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

foreach(file ${copy_files})
	add_custom_command(
		TARGET Neutrino POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${CMAKE_SOURCE_DIR}/${file}
				${CMAKE_CURRENT_BINARY_DIR})
endforeach()

foreach(diretory ${copy_diretories})
	add_custom_command(
			TARGET Neutrino POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${diretory}
					${CMAKE_CURRENT_BINARY_DIR}/${diretory})
endforeach()