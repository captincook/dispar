include_directories(
  gtest/
  gtest/include
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/lib/capstone/include
  )

if (NOT WIN)
  include_directories(
    /usr/include
    /usr/local/include
    )
endif()

add_library(
  gtest
  gtest/src/gtest-all.cc
  )

add_dispar_executable(
  ${testName}

  main.cc

  testutils.h
  testutils.cc

  Util.cc

  Project.cc

  Reader.cc
  CStringReader.cc

  CpuType.cc

  Disassembler.cc

  SymbolEntry.cc
  SymbolTable.cc
  Section.cc
  BinaryObject.cc

  formats/Format.cc
  formats/FormatLoader.cc
  formats/MachO.cc

  binaries.qrc
  )

add_test(
  NAME ${testName}
  COMMAND ${testName}
  )

if (NOT LINUX)
  target_link_libraries(
    ${testName}
    gtest
    ${libName}
    )
else()
  target_link_libraries(
    ${testName}
    gtest
    ${libName}
    pthread
    )
endif()

include_directories(
  gtest/include
  ../src
  )

qt5_use_modules(
  ${testName}
  Core
  )
