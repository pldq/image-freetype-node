#!/bin/bash

apt update && apt install -y cmake g++ wget unzip pkg-config

cd ..

target=$1
build_dir=$(pwd)/deps/${target}
generate_dir=${build_dir}/generated


rm -rf "${build_dir}"

generate_freetype="${generate_dir}/freetype/"
build_freetype="${build_dir}/freetype/"
cmake -B "${generate_freetype}" \
  -DBUILD_SHARED_LIBS=false \
  -DCMAKE_POSITION_INDEPENDENT_CODE=true \
  -DFT_DISABLE_HARFBUZZ=ON \
  -DCMAKE_BUILD_TYPE=${target} \
  -DDISABLE_FORCE_DEBUG_POSTFIX=true \
  -DCMAKE_INSTALL_PREFIX="${build_freetype}" \
  vendor/freetype
cmake --build "${generate_freetype}" -j --config ${target} --target install
export FREETYPE_DIR=${build_freetype}

generate_harfbuzz="${generate_dir}/harfbuzz/"
build_harfbuzz="${build_dir}/harfbuzz/"
cmake -B "${generate_harfbuzz}" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=true \
  -DHB_HAVE_FREETYPE=ON \
  -DBUILD_SHARED_LIBS=false \
  -DCMAKE_BUILD_TYPE=${target} \
  -DCMAKE_INSTALL_PREFIX="${build_harfbuzz}" \
  vendor/harfbuzz
cmake --build "${generate_harfbuzz}" -j --config ${target} --target install
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:${build_freetype}lib/pkgconfig/:${build_harfbuzz}/lib/pkgconfig/

generate_opencv="${generate_dir}/opencv/"
build_opencv="${build_dir}/opencv/"
cmake -B "${generate_opencv}" \
   -DOPENCV_EXTRA_MODULES_PATH=vendor/opencv_contrib/modules/freetype \
   -DBUILD_PACKAGE=OFF \
   -DBUILD_PROTOBUF=OFF \
   -DBUILD_SHARED_LIBS=ON \
   -DBUILD_JAVA=OFF \
   -DBUILD_FAT_JAVA_LIB=OFF \
   -DBUILD_TESTS=OFF \
   -DBUILD_PERF_TESTS=OFF \
   -DBUILD_ITT=OFF \
   -DBUILD_opencv_apps=OFF \
   -DBUILD_opencv_calib3d=OFF \
   -DBUILD_opencv_dnn=OFF \
   -DBUILD_opencv_features2d=OFF \
   -DBUILD_opencv_flann=OFF \
   -DBUILD_opencv_highgui=OFF \
   -DBUILD_opencv_imgcodecs=ON \
   -DBUILD_opencv_imgproc=ON \
   -DBUILD_opencv_java_bindings_generator=OFF \
   -DBUILD_opencv_gapi=OFF \
   -DBUILD_opencv_js_bindings_generator=OFF \
   -DBUILD_opencv_objdetect=OFF \
   -DBUILD_opencv_ml=OFF \
   -DBUILD_opencv_objc_bindings_generator=OFF \
   -DBUILD_opencv_photo=OFF \
   -DBUILD_opencv_python_bindings_generator=OFF \
   -DBUILD_opencv_python_tests=OFF \
   -DBUILD_opencv_stitching=OFF \
   -DBUILD_opencv_ts=OFF \
   -DBUILD_opencv_world=OFF \
   -DBUILD_opencv_video=OFF \
   -DBUILD_opencv_videoio=OFF \
   -DBUILD_EXAMPLES=OFF \
   -DWITH_ADE=OFF \
   -DWITH_FFMPEG=OFF \
   -WITH_FLATBUFFERS=OFF \
   -DWITH_GTK=OFF \
   -DWITH_OBSENSOR=OFF \
   -DWITH_PROTOBUF=OFF \
   -DWITH_FREETYPE=ON \
   -DCMAKE_BUILD_TYPE=${target} \
   -DCMAKE_INSTALL_PREFIX="${build_opencv}" \
   vendor/opencv
cmake --build "${generate_opencv}" -j 2 --config ${target} --target install