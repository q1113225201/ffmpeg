#创建完支持C++项目默认添加的
cmake_minimum_required(VERSION 3.4.1)
add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp )
find_library( log-lib
              log )
target_link_libraries( native-lib
                       ${log-lib} )
#添加ffmpeg已编译好的so库依赖
add_library( #库名称
             avcodec-57
             #SHARED表示是共享库，一般.so文件，还有STATIC，一般.a文件
             SHARED
             #IMPORTED表示引用的不是生成的
             IMPORTED )
#对于已经编译好的so文件需要引入
set_target_properties( #库名称
                       avcodec-57
                       PROPERTIES IMPORTED_LOCATION
                       #库的路径
                       ../../../../libs/armeabi-v7a/libavcodec-57.so )
add_library( avfilter-6
             SHARED
             IMPORTED )
set_target_properties( avfilter-6
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavfilter-6.so )
add_library( avformat-57
             SHARED
             IMPORTED )
set_target_properties( avformat-57
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavformat-57.so )
add_library( avutil-55
             SHARED
             IMPORTED )
set_target_properties( avutil-55
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libavutil-55.so )
add_library( swresample-2
             SHARED
             IMPORTED )
set_target_properties( swresample-2
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libswresample-2.so )
add_library( swscale-4
             SHARED
             IMPORTED )
set_target_properties( swscale-4
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libswscale-4.so )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#外面引入的库文件需要头文件
include_directories(libs/include)
#把需要的so文件链接起来
target_link_libraries( native-lib swresample-2 avcodec-57 avfilter-6 swscale-4 avutil-55 avformat-57
                       ${log-lib} )