#### CSR寄存器
CSR：Control and Status Register 控制状态寄存器,用于保存CPU设置
RISCV Assembly:
```
csrr rd,csr //从csr寄存器中读取数据到rd寄存器
csrw csr,rs1 //将rs1寄存器中的数据写入csr寄存器
csrrw rd,csr,rs  // rd<-csr<-rs1
```
* 常见的csr寄存器及其作用:
* s-scratch: 临时存储，用于保护异常处理时的堆栈指针
* s-tval: 异常发生时，保存异常的地址
* s-epc: 保存异常发生时，程序计数器的值
* s-status: 保存CPU状态，包括xstatus寄存器
* s-cause: 保存异常原因

#### GCC编译优化选项与内联汇编

* GCC宏：定义一个变量并返回最后一个表达式
```
(
    {
        type val;
        val = xxxx;
        val;       
    }
)
````

* 可变参数宏的使用


#### 上下文切换细节补充：
* ra 寄存器保存函数调用后执行的第一条指令,故将进程的内核栈中ra部分设置为进程入口在切换上下文时就可以实现进程的切换。










#### GNU Obj 系列工具的使用
##### objcopy
用于复制和转换目标文件（如可执行文件、目标文件、静态库等）。它可以修改二进制文件的内容、格式和结构，常用于嵌入式开发、固件生成、调试符号处理等场景。
````
# 将 ELF 文件转换为原始二进制镜像（如用于嵌入式固件）
objcopy -O binary input.elf output.bin

# 将二进制文件转换为 ELF 格式（指定架构）
objcopy -I binary -O elf32-i386 -B i386 data.bin data.elf

# 提取特定段（如 .text 代码段）
objcopy -j .text -O binary input.elf text.bin

# 删除特定段（如删除调试段 .debug）
objcopy -R .debug input.elf stripped.elf

# 重命名段
objcopy --rename-section .old_section=.new_section input.elf output.elf

# 删除符号表（减小文件体积，但失去调试能力）
objcopy --strip-all input.elf stripped.elf

# 仅保留调试符号（生成独立的符号文件）
objcopy --only-keep-debug input.elf debug.sym

# 添加自定义符号（常用于低级调试）
objcopy --add-symbol 'my_symbol=0x1000' input.elf output.elf

# 用特定值填充未初始化段（如 BSS 段）
objcopy --set-section-flags .bss=alloc,load,contents \
        --fill 0xFF --pad-to 0x1000 input.elf padded.elf

# 覆盖某个地址的数据（如修补固件）
echo "ABCD" | objcopy --update-section .data=addr=0x2000 input.elf patched.elf

````
##### objdump


#### 