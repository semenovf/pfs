### CMake memo

#### Find features

1. [FindBoost](https://cmake.org/cmake/help/v3.0/module/FindBoost.html)
2. [FindQt4](https://cmake.org/cmake/help/v3.0/module/FindQt4.html)
   [CMake with Qt4](https://wiki.qt.io/Using_CMake_build_system)
   [Qt 5.9 > CMake Manual](http://doc.qt.io/qt-5/cmake-manual.html)

#### Check features

1. [CheckIncludeFileCXX](https://cmake.org/cmake/help/v3.0/module/CheckIncludeFileCXX.html)

#### File system operations

1. Check file exists

```cmake
if(EXISTS <filepath>)
   ...
else()
   ...
endif()
```
