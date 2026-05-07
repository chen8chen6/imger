cmake_minimum_required (VERSION 4.0)

##### 项目名称 #####
project (Imger)

##### QT设置 #####
set (CMAKE_PREFIX_PATH D:/Qt/Qt5.13.2/5.13.2/msvc2017_64/)	#TODO:改成自动搜寻
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package (QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui)		#TODO: 组件名想办法改成只需要修改1次
find_package (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui)

##### 目录 #####
set (DIR_FILE_MGR ${PROJECT_SOURCE_DIR}/fileMgr)
set (DIR_IMG_MGR ${PROJECT_SOURCE_DIR}/imgMgr)
set (DIR_IMG_VIEW ${PROJECT_SOURCE_DIR}/imgView)

##### 源文件 #####
set (SRC_IMG_TYPE ${PROJECT_SOURCE_DIR}/imgType.h ${PROJECT_SOURCE_DIR}/imgType.cpp)
set (SRC_FILE_MGR ${DIR_FILE_MGR}/fileMgr.h ${DIR_FILE_MGR}/fileMgr.cpp)
set (SRC_IMG_MGR ${DIR_IMG_MGR}/imgMgr.h ${DIR_IMG_MGR}/imgMgr.cpp
		${DIR_IMG_MGR}/cacheAll.h ${DIR_IMG_MGR}/cacheAll.cpp 
		${DIR_IMG_MGR}/cacheByNum.h ${DIR_IMG_MGR}/cacheByNum.cpp
		${DIR_IMG_MGR}/imgLoader.h ${DIR_IMG_MGR}/imgLoader.cpp)
set (SRC_IMG_VIEW ${DIR_IMG_VIEW}/imgView.h ${DIR_IMG_VIEW}/imgView.cpp ${DIR_IMG_VIEW}/imgView.ui
		${DIR_IMG_VIEW}/imgDspArea.h ${DIR_IMG_VIEW}/imgDspArea.cpp
		${DIR_IMG_VIEW}/dspStrategy.h ${DIR_IMG_VIEW}/dspStrategy.cpp)

##### 项目文件中的目录结构 #####
source_group (fileMgr FILES ${SRC_FILE_MGR})
source_group (imgMgr FILES ${SRC_IMG_MGR})
source_group (imgView FILES ${SRC_IMG_VIEW})

##### 头文件包含目录 #####
include_directories (${PROJECT_SOURCE_DIR} ${DIR_FILE_MGR} ${DIR_IMG_MGR} ${DIR_IMG_VIEW})

##### 编译 #####
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options (/utf-8)	#解决vs报"常量中有换行符"问题

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(Imger main.cpp ${SRC_IMG_TYPE} ${SRC_FILE_MGR} ${SRC_IMG_MGR} ${SRC_IMG_VIEW})

#连接动态库
target_link_libraries(Imger Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)

##TODO: 把动态库从Qt目录下复制过来




