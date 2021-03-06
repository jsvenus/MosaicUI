cmake_minimum_required(VERSION 2.8)

set(USE_OPENGL OFF)
include(${CMAKE_CURRENT_LIST_DIR}/../extern/video_capture/build/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../extern/tracker/build/CMakeLists.txt)

option(USE_SIMPLE_GRID "Use simple grid animation" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(fex_sd ${bd}/projects/featurex/src) 
set(gfx_sd ${bd}/projects/gfx/src)
set(vid_sd ${bd}/projects/video/src)
set(mos_sd ${bd}/projects/mosaic/src)
set(grid_sd ${bd}/projects/grid/src)
set(ts_sd ${bd}/projects/topshop/src)
set(track_sd ${bd}/projects/tracking/src)

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../src
  ${CMAKE_CURRENT_LIST_DIR}/../include
  ${extern_include_dir}
  ${CMAKE_CURRENT_LIST_DIR}/../extern/tinylib/src
  ${bd}/projects/featurex/include
  ${bd}/projects/gfx/include
  ${bd}/projects/video/include
  ${bd}/projects/mosaic/include
  ${bd}/projects/grid/include
  ${bd}/projects/topshop/include
  ${bd}/projects/tracking/include
  )

if(APPLE)
  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_avfoundation AVFoundation)
  find_library(fr_coremedia CoreMedia)
  find_library(fr_cocoa Cocoa)
  find_library(fr_opengl OpenGL)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)
  find_library(fr_opencl OpenCL)
  find_library(fr_audio AudioUnit)
  find_library(fr_caudio CoreAudio)
  find_library(fr_audio_toolbox AudioToolbox)

  set(app_libs
    ${extern_lib_dir}/libuv.a
    ${extern_lib_dir}/libpng.a
    ${extern_lib_dir}/libjpeg.a
    ${extern_lib_dir}/libz.a
    ${extern_lib_dir}/libglfw3.a

    ${extern_lib_dir}/libavcodec.a
    ${extern_lib_dir}/libavformat.a
    ${extern_lib_dir}/libavutil.a
    ${extern_lib_dir}/libavresample.a

    ${extern_lib_dir}/librxp_player.a
    ${extern_lib_dir}/librxp_player_driver_cpp.a
    ${extern_lib_dir}/libtheora.a
    ${extern_lib_dir}/libvorbis.a
    ${extern_lib_dir}/libogg.a
    ${extern_lib_dir}/libcurl.a
    ${extern_lib_dir}/libjansson.a

    # Polaroid
    ${extern_lib_dir}/libcairo.a
    ${extern_lib_dir}/libpixman-1.a
    ${extern_lib_dir}/libfreetype.a

    # for the tracking
    ${extern_lib_dir}libopencv_core.a
    ${extern_lib_dir}libopencv_imgproc.a
    ${extern_lib_dir}libopencv_video.a
    ${extern_lib_dir}libopencv_features2d.a

    ${fr_corefoundation}               # uv
    ${fr_cocoa}                        # uv
    ${fr_iokit}                        # uv
    ${fr_opengl}                       # glfw
    ${fr_corevideo}                    # glfw
    ${fr_avfoundation}                 # videocapture
    ${fr_coremedia}                    # videocapture
    ${fr_opencl}                       # tracking

    bz2 
    ssl
    crypto
    ldap
    )
elseif(UNIX)

endif()

# Feature Extractor and Matching library
set(fex_lib_sources
  ${fex_sd}/Featurex.cpp
  ${fex_sd}/AverageColorGPU.cpp
  ${fex_sd}/Config.cpp
  ${fex_sd}/Descriptor.cpp
  ${fex_sd}/Comparator.cpp
  ${fex_sd}/AnalyzerCPU.cpp
  ${fex_sd}/AnalyzerGPU.cpp
  ${fex_sd}/TilesPool.cpp
)

# OpenGL wrapper/utilss.
set(gfx_lib_sources
  ${gfx_sd}/Image.cpp
  ${gfx_sd}/Blur.cpp
  ${gfx_sd}/FBO.cpp
  ${gfx_sd}/BlurFBO.cpp
  ${gfx_sd}/Timer.cpp
  ${gfx_sd}/AsyncDownload.cpp
  ${gfx_sd}/AsyncUpload.cpp
)

# Video playback of RTMP
set(vid_lib_sources
  ${vid_sd}/Stream.cpp
  ${vid_sd}/Player.cpp
  ${vid_sd}/Jitter.cpp
  ${vid_sd}/YUV420P.cpp
)

# The grid
set(grid_lib_sources
  ${grid_sd}/Grid.cpp
  ${grid_sd}/SimpleGrid.cpp
  ${grid_sd}/SimpleLayer.cpp
  ${grid_sd}/SimpleSettings.cpp
  )

# The mosaic visualizer (uses all of above)
set(mosaic_lib_sources
  ${mos_sd}/Mosaic.cpp
  ${mos_sd}/Config.cpp
  ${mos_sd}/VideoInput.cpp
  ${mos_sd}/ImageLoader.cpp
  ${mos_sd}/DirWatcher.cpp
)

# The tracking related code (and visualisations)
set(tracking_lib_sources
  ${track_sd}/Tracking.cpp
  ${track_sd}/Tiles.cpp
  ${track_sd}/InteractiveGrid.cpp
)

# The TopShop application - "main application"
set(topshop_lib_sources
  ${ts_sd}/TopShop.cpp
  ${ts_sd}/Config.cpp
  ${ts_sd}/ImageCollector.cpp
  ${ts_sd}/ImageProcessor.cpp
  ${ts_sd}/GridApp.cpp
  ${ts_sd}/GridAppSimple.cpp
  ${ts_sd}/RemoteState.cpp
  ${ts_sd}/ImageJSON.cpp
)

macro(create_test name)
  add_executable(test_${name} 
    ${bd}/projects/tests/test_${name}.cpp 
    ${extern_source_dir}/glad.c
    )
  target_link_libraries(test_${name} ${app_libs} gfx fex vid mosaic grid videocapture shop tracker tracking)
  install(TARGETS test_${name} DESTINATION bin/test/)
endmacro()

# The graphics library
add_library(gfx ${gfx_lib_sources})
install(TARGETS gfx ARCHIVE DESTINATION lib)

# The feature-extractor library
add_library(fex ${fex_lib_sources})
install(TARGETS fex ARCHIVE DESTINATION lib)

# The video library 
add_library(vid ${vid_lib_sources})
install(TARGETS vid ARCHIVE DESTINATION lib)

# The grid library 
add_library(grid ${grid_lib_sources})
install(TARGETS grid ARCHIVE DESTINATION lib)

# The mosaic library
add_library(mosaic ${mosaic_lib_sources})
install(TARGETS mosaic ARCHIVE DESTINATION lib)

# The tracking library
add_library(tracking ${tracking_lib_sources})
install(TARGETS tracking ARCHIVE DESTINATION lib)

# The TopShop library
add_library(shop ${topshop_lib_sources})
install(TARGETS shop ARCHIVE DESTINATION lib)

# The final Mosaic application
add_executable(AppMosaic ${ts_sd}/app_mosaic.cpp  ${extern_source_dir}/glad.c)
target_link_libraries(AppMosaic ${app_libs} gfx fex vid mosaic grid videocapture shop tracking tracker)
install(TARGETS AppMosaic DESTINATION bin)

if(USE_SIMPLE_GRID)

  # The "simple" left grid application
  add_executable(AppGridLeft ${ts_sd}/app_simple_grid.cpp  ${extern_source_dir}/glad.c)
  set_target_properties (AppGridLeft PROPERTIES COMPILE_DEFINITIONS "APP_GRID_LEFT")
  target_link_libraries(AppGridLeft ${app_libs} gfx fex vid mosaic grid videocapture shop tracking tracker)
  install(TARGETS AppGridLeft DESTINATION bin)

  # The "simple" right grid application
  add_executable(AppGridRight ${ts_sd}/app_simple_grid.cpp  ${extern_source_dir}/glad.c)
  set_target_properties (AppGridRight PROPERTIES COMPILE_DEFINITIONS "APP_GRID_RIGHT")
  target_link_libraries(AppGridRight ${app_libs} gfx fex vid mosaic grid videocapture shop tracking tracker)
  install(TARGETS AppGridRight DESTINATION bin)

else()

  # The left grid application
  add_executable(AppGridLeft ${ts_sd}/app_grid.cpp  ${extern_source_dir}/glad.c)
  set_target_properties (AppGridLeft PROPERTIES COMPILE_DEFINITIONS "APP_GRID_LEFT")
  target_link_libraries(AppGridLeft ${app_libs} gfx fex vid mosaic grid videocapture shop tracking tracker)
  install(TARGETS AppGridLeft DESTINATION bin)

  # The right grid application
  add_executable(AppGridRight ${ts_sd}/app_grid.cpp  ${extern_source_dir}/glad.c)
  set_target_properties (AppGridRight PROPERTIES COMPILE_DEFINITIONS "APP_GRID_RIGHT")
  target_link_libraries(AppGridRight ${app_libs} gfx fex vid mosaic grid videocapture shop tracking tracker)
  install(TARGETS AppGridRight DESTINATION bin)

endif()

# The image processor (can be used when left/right apps are not used)
add_executable(AppImageProcessor ${ts_sd}/app_image_processor.cpp)
target_link_libraries(AppImageProcessor ${app_libs} gfx fex vid mosaic grid shop tracking tracker)
install(TARGETS AppImageProcessor DESTINATION bin)

# The polaroid creator
add_executable(AppPolaroid ${bd}/projects/polaroid/main.cpp)
target_link_libraries(AppPolaroid ${app_libs})
install(TARGETS AppPolaroid DESTINATION bin)

# The json generator that can be used to generate fake json meta data for a complete directory
add_executable(AppMetaData ${bd}/projects/metadata/main.cpp)
target_link_libraries(AppMetaData ${app_libs})
install(TARGETS AppMetaData DESTINATION bin)


# The tests used during development
#create_test(mosaic)
#create_test(fex_load_image)
#create_test(libav_rtmp)
#create_test(video_stream_player)
#create_test(offline_analyzer)
#create_test(online_analyzer)
#create_test(async_upload)
#create_test(image_loader)
#create_test(grid)
#create_test(simple_grid)
#create_test(png_rgba)
#create_test(ogg_player)
#create_test(tracker)
#create_test(image_json)
#create_test(cairo_png)
#create_test(cairo_jpg)







