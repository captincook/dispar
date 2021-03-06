# Qt 5
# Added some paths to help find the modules on some systems.
if (NOT WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "/usr/local/Qt-5.6.0"
    "/usr/local/Qt-5.4.1"
    "/usr/local/Qt-5.4.0"
    "/usr/local/opt"
    "/usr/local/opt/qt5"
    "/opt/local/libexec/qt5"
    )
else()
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "C:/qt-everywhere-opensource-src-5.6.0-w64/qtbase/lib/cmake"
    "C:/qt-everywhere-opensource-src-5.4.1-w64/qtbase/lib/cmake"
    "C:/qt-everywhere-opensource-src-5.4.0-w64/qtbase/lib/cmake"
    )
endif()

set(CMAKE_AUTOMOC ON) # moc when necessary.
set(CMAKE_AUTORCC ON) # rcc when necessary.

# As moc files are generated in the binary dir, tell CMake to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(FOUND_QT YES)
