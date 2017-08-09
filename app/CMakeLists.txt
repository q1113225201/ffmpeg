#创建完支持C++项目默认添加的
cmake_minimum_required(VERSION 3.4.1)
add_library( native-lib
             SHARED
             ${CMAKE_SOURCE_DIR}/src/main/cpp/native-lib.cpp )
find_library( log-lib
              log )
target_link_libraries( native-lib
                       ${log-lib} )

#外面引入的库文件需要自定义的头文件路径
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI})
include_directories(libs/include)
#增加ffmpeg的so文件动态共享库，${ANDROID_ABI}表示so文件的ABI类型的路径
add_library( #库名称
             avcodec
             #SHARED表示是共享库，一般.so文件，还有STATIC，一般.a文件
             SHARED
             #IMPORTED表示引用的不是生成的
             IMPORTED )
#对于已经编译好的so文件需要引入
set_target_properties( #库名称
                       avcodec
                       PROPERTIES IMPORTED_LOCATION
                       #库的路径
                       ${distribution_DIR}/libavcodec-57.so )
add_library( avfilter
             SHARED
             IMPORTED )
set_target_properties( avfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavfilter-6.so )
add_library( avformat
             SHARED
             IMPORTED )
set_target_properties( avformat
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavformat-57.so )
add_library( avutil
             SHARED
             IMPORTED )
set_target_properties( avutil
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavutil-55.so )
add_library( swresample
             SHARED
             IMPORTED )
set_target_properties( swresample
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswresample-2.so )
add_library( swscale
             SHARED
             IMPORTED )
set_target_properties( swscale
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswscale-4.so )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#把需要的so文件链接起来
target_link_libraries( native-lib
                       swresample
                       avcodec
                       avfilter
                       swscale
                       avutil
                       avformat
                       ${log-lib} )