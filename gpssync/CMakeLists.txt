SUBDIRS(icons)

SET(kipiplugin_gpssync_PART_SRCS plugin_gpssync.cpp
                                 inputboxnocancel.cpp
                                 gpsbookmarkowner.cpp
                                 gpslistviewcontextmenu.cpp 
                                 gpslistviewitem.cpp 
                                 gpsbabelbinary.cpp 
                                 gpsdataparser.cpp 
                                 gpsmapwidget.cpp 
                                 gpseditdialog.cpp 
                                 gpstracklisteditdialog.cpp 
                                 gpstracklistviewitem.cpp 
                                 gpstracklistwidget.cpp
                                 gpssyncdialog.cpp 
                                 kmlexport.cpp 
                                 kmlgpsdataparser.cpp 
                                 kmlexportconfig.cpp 
   )

KDE4_ADD_PLUGIN(kipiplugin_gpssync ${kipiplugin_gpssync_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_gpssync 
                      ${KDE4_KPARTS_LIBS} 
                      ${KDE4_KHTML_LIBS} 
                      ${KIPI_LIBRARIES} 
                      ${KEXIV2_LIBRARIES} 
                      kipiplugins
                     )

INSTALL(TARGETS kipiplugin_gpssync DESTINATION ${PLUGIN_INSTALL_DIR})

INSTALL(FILES kipiplugin_gpssync.desktop DESTINATION ${SERVICES_INSTALL_DIR})

IF(KDE4_BUILD_TESTS)
    MACRO_OPTIONAL_ADD_SUBDIRECTORY(tests)
ENDIF(KDE4_BUILD_TESTS)
