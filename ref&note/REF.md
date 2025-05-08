# RISC-V寄存器表
## 通用寄存器（整数寄存器）

RISC-V 有 32 个通用寄存器（x0-x31），遵循以下 ABI（应用二进制接口）命名和用途：

| ABI 名称 | 寄存器编号 | 用途 |
|----------|------------|------|
| zero     | x0         | 硬连线零：始终为 0，写入无效。 |
| ra       | x1         | 返回地址：保存函数调用返回地址。 |
| sp       | x2         | 栈指针：指向当前栈顶。 |
| gp       | x3         | 全局指针：指向全局数据区（可选，由编译器优化使用）。 |
| tp       | x4         | 线程指针：指向线程局部存储（TLS）的基地址。 |
| t0-t2    | x5-x7      | 临时寄存器：用于临时值，调用者不保存。 |
| s0/fp    | x8         | 帧指针（可选）：指向当前函数的栈帧基址（与 s0 共享）。 |
| s1       | x9         | 保存寄存器：被调用者需保存。 |
| a0-a1    | x10-x11    | 函数参数/返回值：传递函数参数和返回值。 |
| a2-a7    | x12-x17    | 函数参数：传递函数参数（调用者保存）。 |
| s2-s11   | x18-x27    | 保存寄存器：被调用者需保存。 |
| t3-t6    | x28-x31    | 临时寄存器：调用者不保存。 |

## 特权寄存器（CSR，Control and Status Registers）

RISC-V 特权架构定义了以下关键 CSR（按特权模式分类）：

### (1) 机器模式（M-mode）寄存器

| CSR 名称 | 地址  | 用途 |
|----------|-------|------|
| mstatus  | 0x300 | 机器模式状态寄存器（全局中断使能、特权模式等）。 |
| misa     | 0x301 | 机器 ISA 信息（支持的扩展、XLEN 等）。 |
| mie      | 0x304 | 机器模式中断使能寄存器（控制哪些中断被启用）。 |
| mip      | 0x344 | 机器模式中断等待寄存器（记录挂起的中断）。 |
| mtvec    | 0x305 | 机器模式陷阱向量基地址（陷阱处理程序的入口）。 |
| mepc     | 0x341 | 机器模式异常程序计数器（保存异常发生时的 PC）。 |
| mcause   | 0x342 | 机器模式异常原因（记录异常或中断的具体原因）。 |
| mtval    | 0x343 | 机器模式陷阱值（存储与异常相关的附加信息，如非法地址）。 |
| mscratch | 0x340 | 机器模式临时寄存器（通常用于保存临时值或上下文切换）。 |
| mcycle   | 0xB00 | 机器模式周期计数器（自启动以来的时钟周期数）。 |
| minstret | 0xB02 | 机器模式指令完成计数器（已完成的指令数）。 |
| mhartid  | 0xF14 | 机器模式硬件线程 ID（标识当前硬件线程）。 |

### (2) 监管者模式（S-mode）寄存器

| CSR 名称 | 地址  | 用途 |
|----------|-------|------|
| sstatus  | 0x100 | 监管者模式状态寄存器（类似 mstatus，但权限较低）。 |
| stvec    | 0x105 | 监管者模式陷阱向量基地址。 |
| sepc     | 0x141 | 监管者模式异常程序计数器。 |
| scause   | 0x142 | 监管者模式异常原因。 |
| stval    | 0x143 | 监管者模式陷阱值。 |
| satp     | 0x180 | 监管者地址转换和保护寄存器（用于启用分页机制）。 |
| sscratch | 0x140 | 监管者模式临时寄存器。 |

### (3) 用户模式（U-mode）寄存器

| CSR 名称 | 地址  | 用途 |
|----------|-------|------|
| ustatus  | 0x000 | 用户模式状态寄存器（仅包含部分字段，如全局中断使能）。 |
| utvec    | 0x005 | 用户模式陷阱向量基地址（需支持用户模式中断扩展）。 |
| uepc     | 0x041 | 用户模式异常程序计数器。 |
| ucause   | 0x042 | 用户模式异常原因。 |
| utval    | 0x043 | 用户模式陷阱值。 |
| uscratch | 0x040 | 用户模式临时寄存器。 |

### (4) 其他重要 CSR

| CSR 名称  | 地址 | 用途 |
|-----------|------|------|
| mvendorid | 0xF11| 厂商 ID（标识硬件供应商）。 |
| marchid   | 0xF12| 架构 ID（标识处理器架构）。 |
| mimpid    | 0xF13| 实现 ID（标识处理器具体实现）。 |
| time      | 0xC01| 实时时钟计数器（内存映射，非 CSR，但通过 CSR 地址访问）。 |
| mtime/mtimecmp | -   | 机器模式定时器和比较寄存器（通常为内存映射，非 CSR）。 |

```
csrr rd,csr //从csr寄存器中读取数据到rd寄存器
csrw csr,rs1 //将rs1寄存器中的数据写入csr寄存器
csrrw rd,csr,rs  // rd<-csr<-rs1
```
## 特权模式层级

RISC-V 特权模式从高到低为：

- 机器模式（M-mode）：最高权限，处理硬件异常和中断。
- 监管者模式（S-mode）：操作系统内核运行在此模式。
- 用户模式（U-mode）：应用程序运行在此模式（最低权限）。

## 注意事项

- 可选寄存器：部分 CSR（如用户模式相关寄存器）需要特定扩展支持（如 N 扩展支持用户模式中断）。
- 内存映射寄存器：某些寄存器（如 mtime 和 mtimecmp）可能通过内存地址访问，而非 CSR 指令。
- 权限限制：低特权模式（如 U-mode）无法直接访问高特权模式（如 M-mode）的 CSR。

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
* stvec:保存异常向量(异常的类型)
* satp: 用于保存一级页表的地址，用于虚拟内存机制

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


#### 中断机制的实现
* 异常向量表（Exception Vector Table）是CPU用于处理中断和异常的核心数据结构，其结构和存储方式因架构而异。以下是其核心机制及典型实现：
  1，异常向量表是一个预定义的内存区域，每个条目对应特定类型的异常或中断。每个条目包含处理程序的入口地址或跳转指令，确保CPU触发异常时能快速定位处理函数。
  2.专用寄存器：CPU通过IDTR（x86）、VBAR（ARM）、EBASE（MIPS）等寄存器定位中断表基地址。
    偏移计算：中断号×条目大小得到偏移量，最终地址=基地址+偏移量。
    权限与安全：中断表通常位于内核空间，硬件通过特权级（如x86的DPL）防止非法访问。
  3. 特别注意TLB异常的中断号通常有CPU指定，OS需要遵守相关规范(MMU在TLB未命中时会触发TLB异常)
  4. ARM中中断向量表直接存储跳转指令，RISCV中存储中断处理函数的入口地址，也可以存储跳转指令，可选模式

#### 上下文切换的数据结构
````
// Saved registers for kernel context switches.
struct context {
  uint64 ra;
  uint64 sp;

  // callee-saved
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};
struct trapframe {
  /*   0 */ uint64 kernel_satp;   // kernel page table
  /*   8 */ uint64 kernel_sp;     // top of process's kernel stack
  /*  16 */ uint64 kernel_trap;   // usertrap()
  /*  24 */ uint64 epc;           // saved user program counter
  /*  32 */ uint64 kernel_hartid; // saved kernel tp
  /*  40 */ uint64 ra;
  /*  48 */ uint64 sp;
  /*  56 */ uint64 gp;
  /*  64 */ uint64 tp;
  /*  72 */ uint64 t0;
  /*  80 */ uint64 t1;
  /*  88 */ uint64 t2;
  /*  96 */ uint64 s0;
  /* 104 */ uint64 s1;
  /* 112 */ uint64 a0;
  /* 120 */ uint64 a1;
  /* 128 */ uint64 a2;
  /* 136 */ uint64 a3;
  /* 144 */ uint64 a4;
  /* 152 */ uint64 a5;
  /* 160 */ uint64 a6;
  /* 168 */ uint64 a7;
  /* 176 */ uint64 s2;
  /* 184 */ uint64 s3;
  /* 192 */ uint64 s4;
  /* 200 */ uint64 s5;
  /* 208 */ uint64 s6;
  /* 216 */ uint64 s7;
  /* 224 */ uint64 s8;
  /* 232 */ uint64 s9;
  /* 240 */ uint64 s10;
  /* 248 */ uint64 s11;
  /* 256 */ uint64 t3;
  /* 264 */ uint64 t4;
  /* 272 */ uint64 t5;
  /* 280 */ uint64 t6;
};
````

