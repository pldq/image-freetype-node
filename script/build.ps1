# build.ps1

choco install -y pkgconfiglite

Push-Location ..

$build_dir = (Get-Location).Path + "\build"
$generate_dir = "$build_dir\generated"
$target = "Debug"

if (Test-Path $build_dir) {
    Remove-Item -Recurse -Force $build_dir
}

cmake -B "${generate_dir}\freetype\" `
  -DBUILD_SHARED_LIBS=false `
  -DCMAKE_POSITION_INDEPENDENT_CODE=true `
  -DFT_DISABLE_HARFBUZZ=ON `
  -DCMAKE_BUILD_TYPE="${target}" `
  -DDISABLE_FORCE_DEBUG_POSTFIX=true `
  -DCMAKE_INSTALL_PREFIX="${build_dir}\freetype" `
  vendor/freetype
cmake --build "${generate_dir}\freetype\" -j --config "${target}" --target install

$env:FREETYPE_DIR = "${build_dir}\freetype"

cmake -B "${generate_dir}\harfbuzz\" `
  -DCMAKE_POSITION_INDEPENDENT_CODE=true `
  -DHB_HAVE_FREETYPE=ON `
  -DBUILD_SHARED_LIBS=false `
  -DCMAKE_BUILD_TYPE="${target}" `
  -DCMAKE_INSTALL_PREFIX="${build_dir}\harfbuzz" `
  vendor/harfbuzz
cmake --build "${generate_dir}\harfbuzz\" -j --config "${target}" --target install

$env:PKG_CONFIG_PATH = "$env:PKG_CONFIG_PATH;${build_dir}\freetype\lib\pkgconfig\;${build_dir}\harfbuzz\lib\pkgconfig\"

cmake -B "${generate_dir}\opencv\" `
   -DOPENCV_EXTRA_MODULES_PATH=vendor/opencv_contrib/modules/freetype `
   -DBUILD_PACKAGE=OFF `
   -DBUILD_PROTOBUF=OFF `
   -DBUILD_SHARED_LIBS=ON `
   -DBUILD_JAVA=OFF `
   -DBUILD_FAT_JAVA_LIB=OFF `
   -DBUILD_TESTS=OFF `
   -DBUILD_PERF_TESTS=OFF `
   -DBUILD_ITT=OFF `
   -DBUILD_opencv_apps=OFF `
   -DBUILD_opencv_calib3d=OFF `
   -DBUILD_opencv_dnn=OFF `
   -DBUILD_opencv_features2d=OFF `
   -DBUILD_opencv_flann=OFF `
   -DBUILD_opencv_highgui=OFF `
   -DBUILD_opencv_imgcodecs=ON `
   -DBUILD_opencv_imgproc=ON `
   -DBUILD_opencv_java_bindings_generator=OFF `
   -DBUILD_opencv_gapi=OFF `
   -DBUILD_opencv_js_bindings_generator=OFF `
   -DBUILD_opencv_objdetect=OFF `
   -DBUILD_opencv_ml=OFF `
   -DBUILD_opencv_objc_bindings_generator=OFF `
   -DBUILD_opencv_photo=OFF `
   -DBUILD_opencv_python_bindings_generator=OFF `
   -DBUILD_opencv_python_tests=OFF `
   -DBUILD_opencv_stitching=OFF `
   -DBUILD_opencv_ts=OFF `
   -DBUILD_opencv_world=OFF `
   -DBUILD_opencv_video=OFF `
   -DBUILD_opencv_videoio=OFF `
   -DBUILD_EXAMPLES=OFF `
   -DWITH_ADE=OFF `
   -DWITH_FFMPEG=OFF `
   -WITH_FLATBUFFERS=OFF `
   -DWITH_GTK=OFF `
   -DWITH_OBSENSOR=OFF `
   -DWITH_PROTOBUF=OFF `
   -DWITH_FREETYPE=ON `
   -DCMAKE_INSTALL_PREFIX="${build_dir}\opencv" `
   -C script/generate_find_package_args.cmake `
   -DCMAKE_BUILD_TYPE="${target}" `
   vendor/opencv
cmake --build "${generate_dir}\opencv\" -j --config "${target}" --target install

Pop-Location