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
             ffmpeg-lib
             #SHARED表示是共享库，一般.so文件，还有STATIC，一般.a文件
             SHARED
             #IMPORTED表示引用的不是生成的
             IMPORTED )
set_target_properties( #库名称
                       ffmpeg-lib
                       PROPERTIES IMPORTED_LOCATION
                       #库的路径
                       ../../../../libs/armeabi-v7a/libffmpeg.so )
#外面引入的库文件需要头文件
include_directories(libs/include)
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../libs)
#把需要的so文件链接起来
target_link_libraries( native-lib ffmpeg-lib
                       ${log-lib} )