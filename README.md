## 在root_dir编译过程
## just run this
aclocal
autoconf
automake  --add-missing
autoheader
glibtoolize

mkdir build
cd build
../configure && make
./bin/test

## imgs
![tcp_server](img/tcp_server.png)
