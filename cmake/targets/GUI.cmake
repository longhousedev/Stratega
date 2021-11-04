#set(GUI_SOURCE_FILES
#        main.cpp
#        )
#
#list(TRANSFORM GUI_SOURCE_FILES PREPEND "${SUBPROJ_GUI_SRC_DIR}/")
#
#add_executable (gui ${GUI_SOURCE_FILES})
#
#target_include_directories(gui PUBLIC ${SUBPROJ_STRATEGA_INCLUDE_DIR})
#
#target_link_libraries(gui
#        PUBLIC
#        Stratega
#        PRIVATE
#        # other platforms use Conan's `sfml`
#        "$<$<NOT:$<PLATFORM_ID:Linux>>:CONAN_PKG::sfml>"
#        # for linux we have to use the targets of `sfml`'s components individually
#        "$<$<PLATFORM_ID:Linux>:sfml-system>"
#        "$<$<PLATFORM_ID:Linux>:sfml-graphics>"
#        "$<$<PLATFORM_ID:Linux>:sfml-window>"
#        )
#
##Copy Assets folder
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#
#set_target_properties(gui PROPERTIES
#         LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/lib
#         RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/bin
#         ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/lib
#         EXECUTABLE_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/bin
#
#         LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_DIRECTORY}
#         RUNTIME_OUTPUT_DIRECTORY_RELEASE ${RUNTIME_OUTPUT_DIRECTORY}
#         ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ARCHIVE_OUTPUT_DIRECTORY}
#         EXECUTABLE_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_DIRECTORY}
#         
#         LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_DIRECTORY}
#         RUNTIME_OUTPUT_DIRECTORY_DEBUG ${RUNTIME_OUTPUT_DIRECTORY}
#         ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ARCHIVE_OUTPUT_DIRECTORY}
#         EXECUTABLE_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_DIRECTORY}
#        )