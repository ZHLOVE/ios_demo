LOCAL_PATH := $(call my-dir)

ifndef __NO_WEBRTC__
include $(CLEAR_VARS)
LOCAL_MODULE := webrtc
LOCAL_SRC_FILES := ../../webrtc55/obj/local/armeabi-v7a/libwebrtc.a
include $(PREBUILT_STATIC_LIBRARY)
endif

ifndef __NO_WEBRTC__
include $(CLEAR_VARS)
LOCAL_MODULE := GPWebrtc
LOCAL_SRC_FILES := ../../GPWebrtc/libs/armeabi-v7a/libGPWebrtc.so
include $(PREBUILT_SHARED_LIBRARY)
endif

include $(CLEAR_VARS)
LOCAL_MODULE := transmic

COMMON_DEF := -D__STDC_CONSTANT_MACROS -DLUA_USE_C89 -DWEBRTC_ANDROID -DWEBRTC_POSIX -D__UCLIBC__ -DSSL_USE_OPENSSL -DFEATURE_ENABLE_SSL \
                -DWEBRTC_APM_DEBUG_DUMP=0 -mfloat-abi=softfp -mfpu=neon -DWEBRTC_HAS_NEON \
                -DWEBRTC_AEC_DEBUG_DUMP=0 \
                -DWEBRTC_NS_FLOAT \
                -DWEBRTC_INITIALIZE_FFMPEG \
                -DWEBRTC_INTELLIGIBILITY_ENHANCER=1 \
                -DWEBRTC_BUILD_LIBEVENT \
                -DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE \
                -DMARKED_BY_ZY \
                -DLUA_USE_C89 \
				-D_ANDROID

ifdef __NO_WEBRTC__
COMMON_DEF += -D__NO_WEBRTC__
endif

ifdef TRANS_RELEASE
LOCAL_CFLAGS += -O3 -Wl,-s -DTRANS_RELEASE
LOCAL_CXXFLAGS += -O3 -Wl,-s -DTRANS_RELEASE
else
LOCAL_CFLAGS += -g3
LOCAL_CXXFLAGS += -g3
endif

LOCAL_CFLAGS += -Wall -DNO_CRYPTO -DCRYPTO=0 -fexceptions -DANDROID_NDK $(COMMON_DEF)
LOCAL_CXXFLAGS += -Wall -DNO_CRYPTO -DCRYPTO=0 -fexceptions -std=c++11 -DANDROID_NDK $(COMMON_DEF)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../comn
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ext/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../stats/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../core/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../librelay/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../schedule/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../schedule/jni/lua/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../stats/jni/statsjni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../schedule/jni/schedulejni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../librelay/jni/relayjni

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../webrtc55/src \
                    $(LOCAL_PATH)/../../webrtc55/src/webrtc \
                    $(LOCAL_PATH)/../../webrtc55/src/webrtc/base \
                    $(LOCAL_PATH)/../../webrtc55/src/webrtc/voice_engine/include \
                    $(LOCAL_PATH)/../../webrtc55/src/webrtc/modules/utility/include 

LOCAL_LDLIBS := -llog -lm -lz

ifndef __NO_WEBRTC__
LOCAL_LDLIBS += -lGLESv2 -lOpenSLES
LOCAL_STATIC_LIBRARIES += webrtc
LOCAL_SHARED_LIBRARIES += GPWebrtc
endif

LOCAL_SHARED_LIBRARIES += transcore

MIC_PATH := ../../mic/jni
LIBRELAY_PATH:= ../../librelay/jni

LOCAL_SRC_FILES += $(MIC_PATH)/mic_entry.cpp \
                   $(MIC_PATH)/MPBase.cpp \
                   $(MIC_PATH)/GraceTrans.cpp \
                   $(LIBRELAY_PATH)/jniUtils.cpp

LOCAL_SRC_FILES += $(MIC_PATH)/../../jni.cpp

include $(BUILD_SHARED_LIBRARY)

