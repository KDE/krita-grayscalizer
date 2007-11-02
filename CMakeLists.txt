include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/lib)

set(kritaGrayscalizer_PART_SRCS Grayscalizer.cc  GrayscalizerConfigurationWidget.cc )

kde3_add_ui_files(kritaGrayscalizer_PART_SRCS
    GrayscalizerConfigurationBaseWidget.ui
    )


kde3_automoc(${kritaGrayscalizer_PART_SRCS})

kde3_add_kpart(kritaGrayscalizer ${kritaGrayscalizer_PART_SRCS})

target_link_libraries(kritaGrayscalizer ${KRITAUI_LIBRARY} )

install(TARGETS kritaGrayscalizer  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES  kritaGrayscalizer.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
