//===-- OneRegisterInfo.td - One (RISC-V-like) Register Definitions --*- tablegen -*-===//
//
// RISC-V 32位寄存器文件：
// - x0 (zero)：硬连线0，不可修改
// - x1 (ra)：返回地址
// - x2 (sp)：栈指针
// - x8 (fp/s0)：帧指针
// - x10-x17 (a0-a7)：参数/返回值寄存器
// - 其余寄存器按RISC-V命名
//===----------------------------------------------------------------------===//

// 寄存器基类
class OneReg<string n, list<Register> subregs = []> : Register<n> {
  let Namespace = "One";
  let SubRegs = subregs;
}

// 核心通用寄存器（x0-x31，RISC-V标准命名）
def X0  : OneReg<"x0">, DwarfRegNum<[0]>;  // zero
def X1  : OneReg<"x1">, DwarfRegNum<[1]>;  // ra
def X2  : OneReg<"x2">, DwarfRegNum<[2]>;  // sp
def X3  : OneReg<"x3">, DwarfRegNum<[3]>;  // gp
def X4  : OneReg<"x4">, DwarfRegNum<[4]>;  // tp
def X5  : OneReg<"x5">, DwarfRegNum<[5]>;  // t0
def X6  : OneReg<"x6">, DwarfRegNum<[6]>;  // t1
def X7  : OneReg<"x7">, DwarfRegNum<[7]>;  // t2
def X8  : OneReg<"x8">, DwarfRegNum<[8]>;  // s0/fp
def X9  : OneReg<"x9">, DwarfRegNum<[9]>;  // s1
def X10 : OneReg<"x10">, DwarfRegNum<[10]>;// a0
def X11 : OneReg<"x11">, DwarfRegNum<[11]>;// a1
def X12 : OneReg<"x12">, DwarfRegNum<[12]>;// a2
def X13 : OneReg<"x13">, DwarfRegNum<[13]>;// a3
def X14 : OneReg<"x14">, DwarfRegNum<[14]>;// a4
def X15 : OneReg<"x15">, DwarfRegNum<[15]>;// a5
def X16 : OneReg<"x16">, DwarfRegNum<[16]>;// a6
def X17 : OneReg<"x17">, DwarfRegNum<[17]>;// a7
def X18 : OneReg<"x18">, DwarfRegNum<[18]>;// s2
def X19 : OneReg<"x19">, DwarfRegNum<[19]>;// s3
def X20 : OneReg<"x20">, DwarfRegNum<[20]>;// s4
def X21 : OneReg<"x21">, DwarfRegNum<[21]>;// s5
def X22 : OneReg<"x22">, DwarfRegNum<[22]>;// s6
def X23 : OneReg<"x23">, DwarfRegNum<[23]>;// s7
def X24 : OneReg<"x24">, DwarfRegNum<[24]>;// s8
def X25 : OneReg<"x25">, DwarfRegNum<[25]>;// s9
def X26 : OneReg<"x26">, DwarfRegNum<[26]>;// s10
def X27 : OneReg<"x27">, DwarfRegNum<[27]>;// s11
def X28 : OneReg<"x28">, DwarfRegNum<[28]>;// t3
def X29 : OneReg<"x29">, DwarfRegNum<[29]>;// t4
def X30 : OneReg<"x30">, DwarfRegNum<[30]>;// t5
def X31 : OneReg<"x31">, DwarfRegNum<[31]>;// t6

// 寄存器别名（RISC-V语义化名称）
def ZERO : RegisterAlias<X0>;  // 零寄存器
def RA   : RegisterAlias<X1>;  // 返回地址
def SP   : RegisterAlias<X2>;  // 栈指针
def FP   : RegisterAlias<X8>;  // 帧指针
def A0   : RegisterAlias<X10>; // 参数/返回值0
def A1   : RegisterAlias<X11>; // 参数/返回值1
def A2   : RegisterAlias<X12>; // 参数2
def A3   : RegisterAlias<X13>; // 参数3
def A4   : RegisterAlias<X14>; // 参数4
def A5   : RegisterAlias<X15>; // 参数5
def A6   : RegisterAlias<X16>; // 参数6
def A7   : RegisterAlias<X17>; // 参数7
def S0   : RegisterAlias<X8>;  // 保存寄存器0
def S1   : RegisterAlias<X9>;  // 保存寄存器1
def T0   : RegisterAlias<X5>;  // 临时寄存器0

// 通用寄存器组（所有x0-x31）
def GPR : RegisterClass<"One", [i32], 32,
  (add X0, X1, X2, X3, X4, X5, X6, X7, X8, X9,
       X10, X11, X12, X13, X14, X15, X16, X17,
       X18, X19, X20, X21, X22, X23, X24, X25,
       X26, X27, X28, X29, X30, X31)>;

// 可写通用寄存器组（排除x0，因为x0硬连线0）
def GPROut : RegisterClass<"One", [i32], 32,
  (add X1, X2, X3, X4, X5, X6, X7, X8, X9,
       X10, X11, X12, X13, X14, X15, X16, X17,
       X18, X19, X20, X21, X22, X23, X24, X25,
       X26, X27, X28, X29, X30, X31)>;