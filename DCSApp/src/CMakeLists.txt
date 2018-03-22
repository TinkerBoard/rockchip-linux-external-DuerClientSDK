if(BUILD_TTS_SDK)
    add_definitions(-DLOCALTTS)
    set(LocalSynthesizerPlayer "MediaPlayer/LocalSynthesizerPlayer.cpp")
    set(LocalTtsProxy "MediaPlayer/LocalTtsProxy.cpp")
    set(TTS_SDK_LIB "ASRTTSManager")
endif()

if(BUILD_CRAB_SDK)
   set(CRAB_SDK_LIB CrabSdk)
   include_directories("${BREAKPAD_INCLUDE_DIR}")
   include_directories("${BREAKPAD_INCLUDE_DIR}/breakpad")
endif()

if(${Platform} STREQUAL Sengled
        OR ${Platform} STREQUAL Ingenic)
    set(vendor_AUDIO_INPUT_SOURCE_FILES "Vendor/${Platform}/src/AudioInputWrapper.cpp")
    set(DCSApplication_SOURCE_FILES "Vendor/${Platform}/src/DCSApplication.cpp")
    include_directories("Vendor/${Platform}/include")
    link_directories("Vendor/${Platform}/lib")
else()
    set(DCSApplication_SOURCE_FILES "DCSApplication.cpp")
endif()

if(${Platform} MATCHES "Box86")
    set(DuerLinkWrapper_SOURCE_FILES "${Platform}/DuerLinkWrapper.cpp")
else()
    set(DuerLinkWrapper_SOURCE_FILES "DuerLinkWrapper.cpp")
endif()

if(BUILD_USE_ALSA_RECORDER)
    add_definitions(-DUSE_ALSA_RECORDER)
    set(ALSARECORDER_SRC_FILE "AlsaRecorder.cpp")
    message("use alsa recorder")
else()
    set(ALSARECORDER_SRC_FILE "")
    message("use port audio")
endif()

message("USE_ALSA_RECORDER=${BUILD_USE_ALSA_RECORDER}")
message("${ALSARECORDER_SRC_FILE}")

if(KITTAI_KEY_WORD_DETECTOR)
    set(KEYWORD_OBSERVER_CODE VoiceAndTouchWakeUpObserver.cpp)
endif()

add_executable(duer_linux
        main.cpp
        PortAudioMicrophoneWrapper.cpp
	${ALSARECORDER_SRC_FILE}
        ApplicationManager.cpp
        ${DCSApplication_SOURCE_FILES}
        SoundController.cpp
        DeviceIoWrapper.cpp
        Configuration.cpp
        DuerLinkMtkInstance.cpp
        ${DuerLinkWrapper_SOURCE_FILES}
        SystemUpdateRevWrapper.cpp
        ThreadPoolExecutor.cpp
        ${KEYWORD_OBSERVER_CODE}
        ActivityMonitorSingleton.cpp
        RecordAudioManager.cpp
        MediaPlayer/AlsaController.cpp
        MediaPlayer/StreamPool.cpp
        MediaPlayer/RapidPcmPlayer.cpp
        MediaPlayer/PcmStreamPlayer.cpp
        MediaPlayer/Mp3StreamPlayer.cpp
        MediaPlayer/TtsPlayerProxy.cpp
        MediaPlayer/AlertsPlayerProxy.cpp
        MediaPlayer/LocalPlayerProxy.cpp
        MediaPlayer/AudioDecoder.cpp
        MediaPlayer/SimpleAudioplayerImpl.cpp
        MediaPlayer/PcmBufPool.cpp
        MediaPlayer/AudioPlayerImpl.cpp
        MediaPlayer/MediaPlayerProxy.cpp
        MediaPlayer/OffsetManager.cpp
        MediaPlayer/BlueToothPlayerProxy.cpp
        MediaPlayer/PcmResampler.cpp
        get_event.cpp
        DCSKeyHandler.cpp
        ${LocalTtsProxy}
        ${LocalSynthesizerPlayer}
        ${vendor_AUDIO_INPUT_SOURCE_FILES})

target_include_directories(duer_linux PUBLIC
    "${DCSApp_SOURCE_DIR}/include"
    "${DCSApp_SOURCE_DIR}/include/MediaPlayer"
    "${DeviceIo_SOURCE_DIR}/include"
    "${duerLink_SOURCE_DIR}/include"
    "${PORTAUDIO_INCLUDE_DIR}"
    "${FFMPEG_INCLUDE_DIR}"
    "${ASRTTSManager_SOURCE_DIR}/include"
    "${DeviceTools_SOURCE_DIR}/include"
    "${CrabSdk_SOURCE_DIR}/include"
    )

if(BUILD_ONE_LIB)
    set(DCS_SDK_LIB DcsSdk DeviceTools)
else()
    set(DCS_SDK_LIB DcsClient AuthDelegate)
endif()

target_link_libraries(duer_linux
        ${DCS_SDK_LIB}
        DeviceIo
        duerLink
        "${PORTAUDIO_LIB_PATH}"
        "-L${FFMPEG_LIB_PATH}"
        asound
        avcodec
        avutil
        avformat
        swresample
        rt
        m
        pthread
        nghttp2
        LoggerUtils
        gomp
        bd_alsa_audio_client
        utils_bd_ipc
        BDSpeech
        Configuration
        "${TTS_SDK_LIB}"
        "${CRAB_SDK_LIB}"
        )

# use gperftools
if(BUILD_GPERFTOOLS)
    message("启用gperftools")
    target_link_libraries(duer_linux tcmalloc)
endif()

# for unbuntu platform
#target_link_libraries(duer_linux "-L${FFMPEG_LIB_PATH}" asound avcodec avutil avformat
#swresample -L/usr/lib/atlas-base f77blas cblas lapack_atlas atlas)

if(${Platform} STREQUAL Rk3229)
	target_link_libraries(duer_linux sqlite3)
endif()

if(${Platform} STREQUAL Sengled)
   target_link_libraries(duer_linux sqlite3 z sai_preprocess)
endif()

if(${Platform} STREQUAL Ingenic)
   target_link_libraries(duer_linux atomic four_mic_front_end_alg_client hardware utils cJSON network_manager)
endif()

install(TARGETS duer_linux DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

