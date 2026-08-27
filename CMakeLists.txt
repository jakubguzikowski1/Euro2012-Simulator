cmake_minimum_required(VERSION 3.16)

project(Euro2012Simulator
    VERSION 1.0.0
    DESCRIPTION "UEFA Euro 2012 tournament engine and Qt 6 simulation GUI"
    LANGUAGES CXX
)

# ----- Language / tooling -----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Export compile_commands.json for clangd / IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----- Dependencies -----
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ----- Sources (layered: Qt-free logic + GUI) -----
set(EURO_LOGIC_SOURCES
    src/logic/Zawodnik.cpp
    src/logic/ZdarzenieMeczowe.cpp
    src/logic/CzlonekSztabu.cpp
    src/logic/Druzyna.cpp
    src/logic/Mecz.cpp
    src/logic/Turniej.cpp
)

set(EURO_LOGIC_HEADERS
    src/logic/Exceptions.h
    src/logic/Zawodnik.h
    src/logic/ZdarzenieMeczowe.h
    src/logic/CzlonekSztabu.h
    src/logic/Druzyna.h
    src/logic/Mecz.h
    src/logic/Turniej.h
)

set(EURO_GUI_SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
)

set(EURO_GUI_HEADERS
    src/gui/MainWindow.h
)

set(EURO_UI_FORMS
    src/gui/MainWindow.ui
)

add_executable(Euro2012Simulator
    ${EURO_LOGIC_SOURCES}
    ${EURO_LOGIC_HEADERS}
    ${EURO_GUI_SOURCES}
    ${EURO_GUI_HEADERS}
    ${EURO_UI_FORMS}
)

target_include_directories(Euro2012Simulator
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/logic
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
)

target_link_libraries(Euro2012Simulator
    PRIVATE
        Qt6::Widgets
)

# Target-based warnings (GCC/Clang + MSVC)
if(MSVC)
    target_compile_options(Euro2012Simulator PRIVATE /W4 /permissive-)
else()
    target_compile_options(Euro2012Simulator PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(Euro2012Simulator PROPERTIES
    OUTPUT_NAME "Euro2012Simulator"
    WIN32_EXECUTABLE FALSE
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src/gui"
)

install(TARGETS Euro2012Simulator RUNTIME DESTINATION bin)
