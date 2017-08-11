#创建完支持C++项目默认添加的
cmake_minimum_required(VERSION 3.4.1)
add_library( native-lib
             SHARED
             src/main/cpp/native-lib.cpp )
find_library( log-lib
              log )
target_link_libraries( native-lib
                       ${log-lib} )

add_library( ffmpeg-lib
             SHARED
             IMPORTED )
set_target_properties( ffmpeg-lib
                       PROPERTIES IMPORTED_LOCATION
                       ../../../../libs/armeabi-v7a/libffmpeg.so )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#外面引入的库文件需要头文件
include_directories(libs/include)
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../libs)
#把需要的so文件链接起来
target_link_libraries( native-lib ffmpeg-lib
                       ${log-lib} )