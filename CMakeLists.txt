# CMakeのバージョン
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# リリースビルド用オプション
SET(CMAKE_C_FLAGS_RELEASE "-Wall -O2")

# デバッグビルド用オプション
SET(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -pg -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")

# ビルドのタイプはリリース
SET(CMAKE_BUILD_TYPE Debug)

# 環境変数の登録


# プロジェクト名
PROJECT(objectPoseEstimation)

# 実行に必要なライブラリのパッケージを見つける
FIND_PACKAGE(OpenCV)
FIND_PACKAGE(Boost)

# バージョン番号
SET(serial "1.0.0")

# ビルドするためにインクルードするヘッダーファイルの場所
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("/usr/local/include")
INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# ライブラリの場所
LINK_DIRECTORIES("/usr/local/lib")

SET(RANDOM_FOREST CRForest.cpp util.cpp CRTree.cpp HoG.cpp)

# 実行ファイルを生成
ADD_EXECUTABLE(objectPoseEstimation objectPoseEstimation.cpp ${RANDOM_FOREST})
ADD_EXECUTABLE(learning learning.cpp ${RANDOM_FOREST})

# ライブラリとリンクする
TARGET_LINK_LIBRARIES(objectPoseEstimation ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(learning ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

# バージョン番号の適用
SET_TARGET_PROPERTIES(objectPoseEstimation PROPERTIES VERSION ${serial})
SET_TARGET_PROPERTIES(learning PROPERTIES VERSION ${serial})
