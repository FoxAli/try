解压tar.xz文件：先 xz -d xxx.tar.xz 将 xxx.tar.xz解压成 xxx.tar 然后，再用 tar xvf xxx.tar来解包

1. 在终端中运行下面代码，安装openblas库文件，该库文件支持非稀疏矩阵的运算。

sudo apt-get install libopenblas-dev


2. 在终端中运行下面代码，安装lapack库文件，该库文件支持非稀疏矩阵的运算。

sudo apt-get install liblapack-dev


3. 在终端中运行下面代码，安装arpack库文件，该库文件支持稀疏矩阵的运算。

sudo apt-get install libarpack2-dev

4. 安装

sudo apt-get install libsuperlu-dev

5. 安装 atlas (可能没用)

sudo apt-get install libatlas-base-dev


6. 求取稀疏矩阵特征值需要使用arpack，在下载好的Armadillo库目录中配置文件include/armadillo_bits/config.hpp，使能下面的依赖库。只需取消对下面的语句的注释即可。

	#define ARMA_USE_LAPACK
  	#define ARMA_USE_BLAS
    	#define ARMA_USE_ARPACK
      	#define ARMA_USE_SUPERLU

	.configure
	make
	sudo make install


到此，Armadillo库便完全安装好。利用下面的代码可以编译自己的源代码：

g++ test.cpp -o test -O2 -I /home/xuehen/arma/armadillo-6.300.2/include -DARMA_DONT_USE_WRAPPER -lopenblas -llapack -larpack



编译QT5.15遇到的问题:
	1. 下载源代码 wget http://download.qt.io/archive/qt/5.15/5.15.0/single/qt-everywhere-src-5.15.0.tar.xz
	2. xz 解压: xz -d qt-everywhere-src-5.14.2.tar.xz
	3. rar解压: tar -xvf qt-everywhere-src-5.14.2.tar
	4. mkdir build , cd build
	5. ../configure -xcb -debug -nomake examples -nomake tests -skip qtwebengine -opensource
	6. make
	7. 遇到问题:（注意：以下问题的路径会有所不同.）
	   /mnt/e/linuxfile/qttest3/qt-everywhere-src-5.15.0/qtbase/src/gui/qvkgen_wrapper.sh vulkan/vk.xml /mnt/e/linuxfile/qttest3/qt-everywhere-src-5.15.0/qtbase/header.LGPL vulkan/qvulkanfunctions
	   /mnt/e/linuxfile/qttest3/qt-everywhere-src-5.15.0/qtbase/bin/qvkgen: error while loading shared libraries: libQt5Core.so.5: cannot open shared object file: No such file or directory
	   Makefile:2229: recipe for target 'vulkan/qvulkanfunctions.h' failed

	   打开/mnt/e/linuxfile/qttest3/qt-everywhere-src-5.15.0/qtbase/src/gui/qvkgen_wrapper.sh shell 可以看出来是/mnt/e/linuxfile/qttest3/qt-everywhere-src-5.15.0/qtbase/bin/qvkgen执行失败.

	   在路径下调用 ldd ./qvkgen 可以看见指向了/usr/lib/x86_64-linux-gnu/libQt5Core.so.5（路径可能不同）或者说找不到。
	   在路径下调用 readelf -d ./qvkgen 可以看见编译的库路径选项,证明是调用目标是../lib/libQt5Core.so.5
	   		 0x000000000000001d (RUNPATH)            Library runpath: [$ORIGIN/../lib]

	   查询得到，RUNPATH（../lib）优先级应该高于/usr/lib/x86_64-linux-gnu ，但是存在bug,当RUNPATH（../lib）的库存在ABI 标签的时候，会忽略加载。
	   在路径下调用 readelf -h ./qvkgen 可以看到确实存在ABI 标签 (OS/ABI:UNIX - System V)
	   调用命令移除ABI标签:sudo strip --remove-section=.note.ABI-tag ../lib/libQt5Core.so.5
	   调用命令c查看:readelf -h ../lib/libQt5Core.so.5 , 确定ABI标签已经被移除:( ABI Version:0)

	   执行make 继续编译.

												   







