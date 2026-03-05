//===-- MyriscRegisterInfo.td - Myrisc (RISC-V-like) Register Definitions --*- tablegen -*-===//
//
// RISC-V 32位寄存器文件：
// - x0 (zero)：硬连线0，不可修改
// - x1 (ra)：返回地址
// - x2 (sp)：栈指针
// - x8 (fp/s0)：帧指针
// - x10-x17 (a0-a7)：参数/返回值寄存器
// - 其余寄存器按RISC-V命名
//===----------------------------------------------------------------------===//

class MyriscReg<bits<5> Enc, string n> : Register<n> {
    let HWEncoding{4-0} = Enc;
    let Namespace = "Myrisc";
}

def ZERO  : MyriscReg<0, "zero">, DwarfRegNum<[0]>;
def RA  : MyriscReg<1, "ra">, DwarfRegNum<[1]>;
def SP  : MyriscReg<2, "sp">, DwarfRegNum<[2]>;
def T0 : MyriscReg<5, "t0">, DwarfRegNum<[5]>;
def T1 : MyriscReg<6, "t1">, DwarfRegNum<[6]>;
def T2 : MyriscReg<7, "t2">, DwarfRegNum<[7]>;
def S0 : MyriscReg<8, "s0">, DwarfRegNum<[8]>;
def S1 : MyriscReg<9, "s1">, DwarfRegNum<[9]>;
def A0 : MyriscReg<10, "a0">, DwarfRegNum<[10]>;
def A1 : MyriscReg<11, "a1">, DwarfRegNum<[11]>;
def A2 : MyriscReg<12, "a2">, DwarfRegNum<[12]>;
def A3 : MyriscReg<13, "a3">, DwarfRegNum<[13]>;
def A4 : MyriscReg<14, "a4">, DwarfRegNum<[14]>;
def A5 : MyriscReg<15, "a5">, DwarfRegNum<[15]>;
def A6 : MyriscReg<16, "a6">, DwarfRegNum<[16]>;
def A7 : MyriscReg<17, "a7">, DwarfRegNum<[17]>;

def GPR : RegisterClass<"Myrisc", [i32], 32, (add
    ZERO, RA, SP,
    A0, A1, A2,A3,A4,A5,A6,A7,
    T0, T1, T2,
    S0, S1
 )>;
