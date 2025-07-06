qt_add_executable(app
  main.cpp

  #controller
  controller/library/LibController.cpp
  controller/preview/PreviewController.cpp
  controller/timeline/TlController.cpp

  #view
  view/library/LibItem.cpp
  view/library/LibraryView.cpp
  view/library/LibraryView.ui
  view/preview/PreviewView.cpp
  view/preview/PreviewView.ui
  view/timeline/ClipItem.cpp
  view/timeline/TlView.cpp
  view/timeline/TrackItem.cpp
  view/Uncut.cpp
  view/Uncut.ui
  
  #model
  model/library/LibModel.cpp
  model/timeline/TlModel.cpp

  #services
  services/decoder/AudioDecoder.cpp
  services/decoder/FFMPEGDecoder.cpp
  services/decoder/VideoDecoder.cpp
  services/decoder/VideoDecodeWorker.h
  services/decoder/AudioDecodeWorker.h
  services/decoder/VideoDisplayWorker.h
  services/exchange/ExchangeService.cpp

  #utils
  utils/TimeHelper.h
  utils/UiHelper.cpp

)

target_link_libraries(app
  PRIVATE Qt6::Widgets
  PRIVATE SDL3::SDL3
  PRIVATE ${FFMPEG_LIBRARIES}
)



target_include_directories(app
  PUBLIC ${SDL3_INCLUDE_DIRS}

  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC ${FFMPEG_INCLUDE_DIRS}
)


set_target_properties(app PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)


add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_DLL_PATH}"
    $<TARGET_FILE_DIR:app>)
