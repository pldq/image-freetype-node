# build.ps1

choco install -y pkgconfiglite

Set-Location ..

$build_dir = (Get-Location).Path + "\build"
$generate_dir = "$build_dir\generated"

if (Test-Path $build_dir) {
    Remove-Item -Recurse -Force $build_dir
}

cmake -B "${generate_dir}\freetype\" `
  -DBUILD_SHARED_LIBS=true `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_INSTALL_PREFIX="${build_dir}\freetype" `
  vendor/freetype
cmake --build "${generate_dir}\freetype\" --target install

$env:FREETYPE_DIR = "${build_dir}\freetype"

cmake -B "${generate_dir}\harfbuzz\" `
  -DHB_HAVE_FREETYPE=ON `
  -DBUILD_SHARED_LIBS=true `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_INSTALL_PREFIX="${build_dir}\harfbuzz" `
  vendor/harfbuzz
cmake --build "${generate_dir}\harfbuzz\" --target install

$env:PKG_CONFIG_PATH = "$env:PKG_CONFIG_PATH;${build_dir}\freetype\lib\pkgconfig\;${build_dir}\harfbuzz\lib\pkgconfig\"

cmake -B "${generate_dir}\opencv\" `
   -DOPENCV_EXTRA_MODULES_PATH=vendor/opencv_contrib/modules/freetype `
   -DBUILD_SHARED_LIBS=ON `
   -DBUILD_JAVA=OFF `
   -DBUILD_FAT_JAVA_LIB=OFF `
   -DBUILD_TESTS=OFF `
   -DBUILD_PERF_TESTS=OFF `
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
   -DBUILD_opencv_world=ON `
   -DBUILD_opencv_video=OFF `
   -DBUILD_opencv_videoio=OFF `
   -DBUILD_EXAMPLES=OFF `
   -DWITH_FREETYPE=ON `
   -DCMAKE_INSTALL_PREFIX="${build_dir}\opencv" `
   vendor/opencv
cmake --build "${generate_dir}\opencv\" --target install