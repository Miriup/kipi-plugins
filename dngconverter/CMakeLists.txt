
# =======================================================
# DNG SDK and XMP SDK use C++ exceptions
ADD_DEFINITIONS(${KDE4_ENABLE_EXCEPTIONS})

# =======================================================
# XMP SDK source code.
KDE4_NO_ENABLE_FINAL(kipiplugin_dngwriter)

INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/md5
                    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore
                    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/include/client-glue
                   )

# Set platteforms env.
IF(WIN32)
    ADD_DEFINITIONS(
                    # Set Windows rules.
                    -DWIN_ENV=1
                   )
ELSE(WIN32)
#    NOTE: see B.K.O #195735: do not enable Mac flags provided by Adobe.
#          Sound like all compile fine like under Linux.
#    IF(APPLE)
#        ADD_DEFINITIONS(
#                        # Set MACOS-X rules.
#                        -DMAC_ENV=1
#                       )
#    ELSE(APPLE)
        ADD_DEFINITIONS(
                        # Set Linux & co rules.
                        -DUNIX_ENV=1
                       )
#    ENDIF(APPLE)
ENDIF(WIN32)

SET(libmd5_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/md5/MD5.cpp)

SET(libxmp_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/common/XML_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/common/UnicodeConversions.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPCore_Impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/WXMPIterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/WXMPMeta.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/WXMPUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPIterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPMeta-GetSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPMeta-Parse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPMeta-Serialize.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPMeta.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPUtils-FileInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/XMPUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/ExpatAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/xmp_sdk/XMPCore/ParseRDF.cpp
   )

# =======================================================
# DNG SDK source code

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk )

# Set platteforms env.
IF(WIN32)
    ADD_DEFINITIONS(
                    # Set Windows rules.
                    -DqWinOS=1
                    -DqMacOS=0
                   )
ELSE(WIN32)
#    NOTE: see B.K.O #195735: do not enable Mac flags provided by Adobe.
#          Sound like all compile fine like under Linux.
#    IF(APPLE)
#        ADD_DEFINITIONS(
#                        # Set MACOS-X rules.
#                        -DqWinOS=0
#                        -DqMacOS=1
#                       )
#    ELSE(APPLE)
        ADD_DEFINITIONS(
                        # Set Linux & co rules.
                        -DqWinOS=0
                        -DqMacOS=0

                        # Must be set to 1, else do not compile under Linux.
                        -DqDNGUseStdInt=1
                       )
#    ENDIF(APPLE)
ENDIF(WIN32)

# Check processor endianness
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
IF(NOT IS_BIG_ENDIAN)
    ADD_DEFINITIONS(-DqDNGLittleEndian=1)
ENDIF(NOT IS_BIG_ENDIAN)

# Thread safe support under Mac and Linux using pthread library
IF(NOT WIN32)
    ADD_DEFINITIONS(-DqDNGThreadSafe=1)
ENDIF(NOT WIN32)

# Level of debug info in the console.
ADD_DEFINITIONS(-DqDNGDebug=0)

SET(libdng_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_1d_function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_date_time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_ifd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_simple_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_1d_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_exceptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_memory_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_rational.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_spline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_xmp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_abort_sniffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_exif.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_image_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_preview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_misc_opcodes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_mosaic_info.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_read_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_xmp_sdk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_area_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_file_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_info.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_rect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_xy_coord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_bottlenecks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_bad_pixels.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_filter_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_iptc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_negative.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_reference.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_string_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_camera_profile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_fingerprint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_lens_correction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_linearization_info.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_opcode_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_opcodes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_orientation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_render.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_tag_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_color_space.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_globals.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_gain_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_lossless_jpeg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_parse_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_resample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_temperature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_color_spec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_host.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_pixel_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_shared.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_tile_iterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_tone_curve.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_hue_sat_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/extra/dng_sdk/dng_pthread.cpp
   )

# =======================================================
# DNGWritter interface

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter )

SET(libdngwriter_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/dngwriter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/dngwriter_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dngwriter/dngwriterhost.cpp
    ${libxmp_SRCS}
    ${libmd5_SRCS}
    ${libdng_SRCS}
   )

# =======================================================

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/plugin )

SET(dngconverter_common_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/plugin/actionthread.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/plugin/batchdialog.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/plugin/settingswidget.cpp
                             ${libdngwriter_SRCS}
   )

ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(plugin)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(dnginfo)
