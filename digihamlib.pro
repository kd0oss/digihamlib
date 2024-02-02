QT -= gui
QT += network multimedia serialport

TEMPLATE = lib
DEFINES += DIGIHAMLIB_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CRCenc.cpp \
    Golay24128.cpp \
    M17Convolution.cpp \
    SHA256.cpp \
    YSFConvolution.cpp \
    YSFFICH.cpp \
    cbptc19696.cpp \
    cgolay2087.cpp \
    chamming.cpp \
    crs129.cpp \
 #  dcs.cpp \
    dcs.cpp \
    digihamlib.cpp \
 #   dmr.cpp \
 #   iax.cpp \
    dmr.cpp \
    imbe_vocoder/aux_sub.cc \
    imbe_vocoder/basicop2.cc \
    imbe_vocoder/ch_decode.cc \
    imbe_vocoder/ch_encode.cc \
    imbe_vocoder/dc_rmv.cc \
    imbe_vocoder/decode.cc \
    imbe_vocoder/dsp_sub.cc \
    imbe_vocoder/encode.cc \
    imbe_vocoder/imbe_vocoder.cc \
    imbe_vocoder/imbe_vocoder_impl.cc \
    imbe_vocoder/math_sub.cc \
    imbe_vocoder/pe_lpf.cc \
    imbe_vocoder/pitch_est.cc \
    imbe_vocoder/pitch_ref.cc \
    imbe_vocoder/qnt_sub.cc \
    imbe_vocoder/rand_gen.cc \
    imbe_vocoder/sa_decode.cc \
    imbe_vocoder/sa_encode.cc \
    imbe_vocoder/sa_enh.cc \
    imbe_vocoder/tbls.cc \
    imbe_vocoder/uv_synt.cc \
    imbe_vocoder/v_synt.cc \
    imbe_vocoder/v_uv_det.cc \
 #   m17.cpp \
    m17.cpp \
    mode.cpp \
 #   nxdn.cpp \
 #   p25.cpp \
    nxdn.cpp \
    p25.cpp \
    ref.cpp \
    xrf.cpp \
    ysf.cpp
 #   xrf.cpp \
 #   ysf.cpp

android:SOURCES += androidserialport.cpp
!ios:SOURCES += serialambe.cpp serialmodem.cpp

HEADERS += \
    CRCenc.h \
    DMRDefines.h \
    Golay24128.h \
    M17Convolution.h \
 #   M17Defines.h \
 #   MMDVMDefines.h \
    M17Defines.h \
    MMDVMDefines.h \
    SHA256.h \
    YSFConvolution.h \
    YSFFICH.h \
    cbptc19696.h \
    cgolay2087.h \
    chamming.h \
    crs129.h \
 #   dcs.h \
    dcs.h \
    digihamlib_global.h \
    digihamlib.h \
#    dmr.h \
 #   iax.h \
 #   iaxdefines.h \
    dmr.h \
    imbe_vocoder/aux_sub.h \
    imbe_vocoder/basic_op.h \
    imbe_vocoder/ch_decode.h \
    imbe_vocoder/ch_encode.h \
    imbe_vocoder/dc_rmv.h \
    imbe_vocoder/decode.h \
    imbe_vocoder/dsp_sub.h \
    imbe_vocoder/encode.h \
    imbe_vocoder/globals.h \
    imbe_vocoder/imbe.h \
    imbe_vocoder/imbe_vocoder.h \
    imbe_vocoder/imbe_vocoder_api.h \
    imbe_vocoder/imbe_vocoder_impl.h \
    imbe_vocoder/math_sub.h \
    imbe_vocoder/pe_lpf.h \
    imbe_vocoder/pitch_est.h \
    imbe_vocoder/pitch_ref.h \
    imbe_vocoder/qnt_sub.h \
    imbe_vocoder/rand_gen.h \
    imbe_vocoder/sa_decode.h \
    imbe_vocoder/sa_encode.h \
    imbe_vocoder/sa_enh.h \
    imbe_vocoder/tbls.h \
    imbe_vocoder/typedef.h \
    imbe_vocoder/typedefs.h \
    imbe_vocoder/uv_synt.h \
    imbe_vocoder/v_synt.h \
    imbe_vocoder/v_uv_det.h \
 #   m17.h \
    m17.h \
    mode.h \
#    nxdn.h \
#    p25.h \
    nxdn.h \
    p25.h \
    ref.h \
    xrf.h \
    ysf.h
 #   xrf.h \
 #   ysf.h

android:HEADERS += androidserialport.h
!ios:HEADERS += serialambe.h serialmodem.h

!contains(DEFINES, USE_EXTERNAL_CODEC2){
HEADERS += \
        codec2/codec2_api.h \
        codec2/codec2_internal.h \
        codec2/defines.h \
        codec2/kiss_fft.h \
        codec2/lpc.h \
        codec2/nlp.h \
        codec2/qbase.h \
        codec2/quantise.h
SOURCES += \
        codec2/codebooks.cpp \
        codec2/codec2.cpp \
        codec2/kiss_fft.cpp \
        codec2/lpc.cpp \
        codec2/nlp.cpp \
        codec2/pack.cpp \
        codec2/qbase.cpp \
        codec2/quantise.cpp
}
contains(DEFINES, USE_EXTERNAL_CODEC2){
LIBS += -lcodec2
}
!contains(DEFINES, VOCODER_PLUGIN){
HEADERS += \
        mbe/ambe3600x2400_const.h \
        mbe/ambe3600x2450_const.h \
        mbe/ecc_const.h \
        mbe/mbelib.h \
        mbe/mbelib_const.h \
        mbe/mbelib_parms.h \
        mbe/vocoder_plugin.h \
        mbe/vocoder_plugin_api.h \
        mbe/vocoder_tables.h
SOURCES += \
        mbe/ambe3600x2400.c \
        mbe/ambe3600x2450.c \
        mbe/ecc.c \
        mbe/mbelib.c \
        mbe/vocoder_plugin.cpp
}

android:ANDROID_VERSION_CODE = 79

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

TRANSLATIONS += \
    digihamlib_en_US.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

contains(DEFINES, USE_MD380_VOCODER){
        LIBS += -lmd380_vocoder -Xlinker --section-start=.firmware=0x0800C000 -Xlinker  --section-start=.sram=0x20000000
}
