# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright © 2019 by The qTox Project Contributors
# Copyright © 2024-2025 The TokTok team.

#############################################################################
#
# :: Installation
#
#############################################################################

if(NOT PROJECT_ORG)
  set(PROJECT_ORG "chat.tox")
endif()

if(APPLE)
  set_target_properties(${BINARY_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/platform/macos/Info.plist"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
  )

  set(BUNDLE_PATH "${CMAKE_BINARY_DIR}/${BINARY_NAME}.app")

  list(APPEND PROJECT_ICONS "resources/images/icons/${BINARY_NAME}.icns")
  list(APPEND PROJECT_ICONS "resources/images/icons/${BINARY_NAME}_profile.icns")

  foreach(icon ${PROJECT_ICONS})
    install(FILES ${icon} DESTINATION ${BUNDLE_PATH}/Contents/Resources/)
  endforeach()

  # Search for createdmg. If it doesn't exist anywhere, download it.
  if(EXISTS "${CMAKE_SOURCE_DIR}/platform/macos/createdmg")
    set(CREATE_DMG "${CMAKE_SOURCE_DIR}/platform/macos/createdmg")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/third_party/ci-tools/platform/macos/createdmg")
    set(CREATE_DMG "${CMAKE_SOURCE_DIR}/third_party/ci-tools/platform/macos/createdmg")
  elseif(EXISTS "${CMAKE_SOURCE_DIR}/../ci_tools/platform/macos/createdmg")
    set(CREATE_DMG "${CMAKE_SOURCE_DIR}/../ci_tools/platform/macos/createdmg")
  else()
    set(CREATE_DMG "${CMAKE_BINARY_DIR}/createdmg")
    file(DOWNLOAD "https://raw.githubusercontent.com/TokTok/ci-tools/master/platform/macos/createdmg" "${CREATE_DMG}")
    file(DOWNLOAD "https://raw.githubusercontent.com/TokTok/ci-tools/master/platform/macos/gplv3.rtf" "${CMAKE_BINARY_DIR}/gplv3.rtf")
  endif()

  install(CODE "
  message(STATUS \"Creating dmg image\")
  execute_process(COMMAND ${CREATE_DMG} ${PROJECT_NAME} ${CMAKE_SOURCE_DIR} ${BUNDLE_PATH})
  " COMPONENT Runtime
  )
elseif(WIN32)
  install(CODE "
  message(STATUS \"Installer can be created using platform/windows/${BINARY_NAME}64.nsi\")
  " COMPONENT Runtime
  )
else()
  include(GNUInstallDirs)
  install(TARGETS ${BINARY_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(FILES "platform/linux/Parley.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
  install(FILES "img/icons/qtox.svg" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps" RENAME Parley.svg)
  install(FILES LICENSE README.md DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/doc/Parley")
endif()
