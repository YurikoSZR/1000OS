#### 环境配置
##### 运行
首先clone仓库 git clone git://g.csail.mit.edu/xv6-labs-2022
然后 cd xv6... 
make qemu 即可运行虚拟机并进入xv6系统
##### 拉取分支
````
git fetch
git checkout xxx
make clean
````
##### GDB 调试内核
````
riscv64-unknown-elf-gdb
target remote localhost:<PORT NUMBER>
file kernel/kernel
````
