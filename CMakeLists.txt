cmake_minimum_required (VERSION 4.0)

##### 项目名称 #####
set (PROJECT_NAME Imger)
project (${PROJECT_NAME})

##### 自动搜寻QT安装路径 #####
if (WIN32)
	#QT常见安装路径, 格式如: D:/Qt/Qt5.13.2/5.13.2/
	file (GLOB COMMON_USED_QT_DIR_LIST
		C:/Qt/Qt[56].*.*/[56].*.*
		D:/Qt/Qt[56].*.*/[56].*.*
		E:/Qt/Qt[56].*.*/[56].*.*)

	foreach(COMMON_USED_DIR IN LISTS COMMON_USED_QT_DIR_LIST)
		if (EXISTS ${COMMON_USED_DIR})
			message(STATUS "QT dir found: " ${COMMON_USED_DIR})
			set(CMAKE_PREFIX_PATH ${COMMON_USED_DIR}/msvc2017_64/)	#设置编译器路径
			break()
		endif()
	endforeach()
endif()						
if ((NOT DEFINED CMAKE_PREFIX_PATH) OR (CMAKE_PREFIX_PATH STREQUAL ""))
	message (FATAL_ERROR "QT dir not found! Add your QT dir to macro COMMON_USED_QT_DIR_LIST manually.")	#自动搜寻失败
endif()

##### QT设置 #####
#set (CMAKE_PREFIX_PATH D:/Qt/Qt5.13.2/5.13.2/msvc2017_64/)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

set (QT_COMPNENTS_REQUIRED Widgets Gui Xml) #需要添加的Qt组件
find_package (QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPNENTS_REQUIRED})
find_package (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPNENTS_REQUIRED})

##### 目录 #####
set (DIR_FILE_MGR ${PROJECT_SOURCE_DIR}/fileMgr)
set (DIR_IMG_MGR ${PROJECT_SOURCE_DIR}/imgMgr)
set (DIR_IMG_VIEW ${PROJECT_SOURCE_DIR}/imgView)
set (DIR_CFG ${PROJECT_SOURCE_DIR}/cfg)

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
set (SRC_CFG ${DIR_CFG}/cfgDef.h ${DIR_CFG}/cfgDef.cpp
		${DIR_CFG}/cfgMgr.h ${DIR_CFG}/cfgMgr.cpp
		${DIR_CFG}/xmlHelper.h ${DIR_CFG}/xmlHelper.cpp)

##### 项目文件中的目录结构 #####
source_group (fileMgr FILES ${SRC_FILE_MGR})
source_group (imgMgr FILES ${SRC_IMG_MGR})
source_group (imgView FILES ${SRC_IMG_VIEW})
source_group (cfg FILES ${SRC_CFG})

##### 头文件包含目录 #####
include_directories (${PROJECT_SOURCE_DIR} ${DIR_FILE_MGR} ${DIR_IMG_MGR} ${DIR_IMG_VIEW} ${DIR_CFG})

##### 编译 #####
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options (/utf-8)	#解决vs报"常量中有换行符"问题

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${SRC_IMG_TYPE} ${SRC_FILE_MGR} ${SRC_IMG_MGR} ${SRC_IMG_VIEW} ${SRC_CFG} ${PROJECT_SOURCE_DIR}/res/res.qrc)

#连接动态库
foreach(COMP IN LISTS QT_COMPNENTS_REQUIRED)
	list(APPEND QT_LIB_TO_LINK Qt${QT_VERSION_MAJOR}::${COMP})
endforeach()
target_link_libraries(${PROJECT_NAME} ${QT_LIB_TO_LINK})

##TODO: 把动态库从Qt目录下复制过来