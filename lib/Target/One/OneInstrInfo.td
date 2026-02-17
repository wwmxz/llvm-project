include "OneInstrFormats.td"

def retglue : SDNode<"OneISD::RET_GLUE", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//class ImmLeaf<ValueType vt, code pred, SDNodeXForm xform = NOOP_SDNodeXForm,
//              SDNode ImmNode = imm>
//  : PatFrag<(ops), (vt ImmNode), [{}], xform>

// class PatLeaf<dag frag, code pred = [{}], SDNodeXForm xform = NOOP_SDNodeXForm>
// : PatFrag<(ops), frag, pred, xform>;

//  Operand<i32>: llvm IR层面使用32位 整数表示，对应Cpu0架构的通用寄存器宽度
//  [{ return isInt<12>(Imm);}]：作为预言pred,如果满足条件则匹配为imm12的操作数类型

def imm12 : Operand<i32>, ImmLeaf<i32, [{ return isInt<12>(Imm);}]>;
def imm20 : Operand<i32>, ImmLeaf<i32, [{ return isInt<20>(Imm);}]>;
// def imm12Pat : ImmLeaf<i32, [{ return isInt<12>(Imm);}]>;
// def imm12Pat : PatLeaf<(imm), [{ return isInt<12>(N->getSExtValue());}]>

// load store
// 操作的是地址 -> base + offset
// 对地址建模 -> 内存操作

// def mem : Operand<iPTR> {
//    /// 具体有哪些操作数
//    let MIOperandInfo = (ops GPR, imm12);
//    /// 指明自定义的打印函数 offset(GPR)这种形式
//    let PrintMethod = "printMemOperand";
// }

// def ptr : Operand<iPTR> {
//    /// 具体有哪些操作数
//    let MIOperandInfo = (ops GPR, imm12);
//    /// 指明自定义的打印函数 offset(GPR)这种形式
//    let PrintMethod = "printPtrOperand";
// }
//ComplexPattern：复杂模式操作数（非单一寄存器/立即数，而是组合寻址）；
//- `iPTR`：操作数类型为指针；
//- `2`：匹配 2 个基础操作数（如 `GPR + imm12`）；
//- `"SelectAddrFI"`：C++ 选择函数（用于解析帧索引到实际地址）；
// SDNPWantParent -> 传递引用frameIndex的节点
// frameindex -> 传递 FrameIndexSDNode
def addrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;


// load 和 store
//- `multiclass`：TableGen 批量定义模板（类似 C++ 模板类，避免重复写 FI/R 版本）；
//- `PatFrag op`：IR 操作片段（如 `store` 是普通存储，`truncstorei16` 是截断为 16 位存储）；
//- `!strconcat(asm, "\t$rd, $imm12($rs1)")`：拼接汇编字符串（如 `ld $a0, 4($sp)`）；
multiclass LOAD<bits<7> opcode, bits<3> funct3, PatFrag op, string asm> {
    def FI : I<opcode, funct3, (outs GPR: $rd), (ins GPR:$rs1, imm12:$imm), !strconcat(asm, "\t$rd, $imm($rs1)"), [(set GPR:$rd, (op (addrFI GPR:$rs1, imm12:$imm)))],IILoad>;
    def R : I<opcode, funct3, (outs GPR: $rd), (ins GPR: $rs1), !strconcat(asm, "\t$rd, 0($rs1)"), [(set GPR:$rd, (op GPR:$rs1))],IILoad> {
        let imm = 0;
    }
}
defm LOADW:LOAD<0b0000011, 0b010, load, "lw">;

defm LOADH:LOAD<0b0000011, 0b001, extloadi16, "lh">;
defm LOADSH:LOAD<0b0000011, 0b001, sextloadi16, "lh">;
defm LOADZH:LOAD<0b0000011, 0b001, zextloadi16, "lh">;

defm LOADB:LOAD<0b0000011, 0b000, extloadi8, "lb">;
defm LOADSB:LOAD<0b0000011, 0b000, sextloadi8, "lb">;
defm LOADZB:LOAD<0b0000011, 0b000, zextloadi8, "lb">;

multiclass STORE<bits<7> opcode, bits<3> funct3, PatFrag op, string asm> {
    def FI : S<opcode, funct3,
    (outs), (ins GPR: $rs2, GPR:$rs1, imm12:$imm),
    !strconcat(asm, "\t$rs2, $imm($rs1)"),
    [(op GPR:$rs2, (addrFI GPR:$rs1, imm12:$imm))],
    IIStore>;
    def R : S<opcode, funct3,
    (outs), (ins GPR: $rs2, GPR: $rs1),
    !strconcat(asm, "\t$rs2, 0($rs1)"),
    [(op GPR:$rs2, GPR:$rs1)],
    IIStore> {
        let imm = 0;
    }
}

defm STOREW: STORE<0b0100011, 0b010, store, "sw">;
defm STOREH: STORE<0b0100011, 0b001, truncstorei16, "sh">;
defm STOREB: STORE<0b0100011, 0b000, truncstorei8, "sb">;

class ArithLogicI<bits<7> opcode, bits<3> funct3, string inst, SDNode node> : I<
    opcode, funct3,
    (outs GPR:$rd),
    (ins GPR:$rs1, imm12: $imm),
    !strconcat(inst, "\t$rd, $rs1, $imm"),
    [(set GPR:$rd, (node GPR:$rs1, imm12: $imm))],
    IIAlu
    >;

def ADDI : ArithLogicI<0b0010011, 0b000, "addi", add>;
def ANDI : ArithLogicI<0b0010011, 0b111, "andi", and>;
def ORI  : ArithLogicI<0b0010011, 0b110, "ori", or>;
def XORI  : ArithLogicI<0b0010011, 0b100, "xori", xor>;
def SLLI  : ArithLogicI<0b0010011, 0b001, "slli", shl>;
def SRLI  : ArithLogicI<0b0010011, 0b101, "srli", srl>;
def SRAI  : ArithLogicI<0b0010011, 0b101, "srai", sra>;

//def ADDI : I<(outs GPR:$rd),
//                (ins GPR:$rs1, imm12: $imm12),
//                "addi $rd, $rs1, $imm12", [(set GPR:$rd, (add GPR: $rs1, imm12: $imm12))]>;


class ArithLogicR<bits<7> funct7, bits<3> funct3, string inst, SDNode node> : R<
    0b0110011, funct7, funct3,
    (outs GPR:$rd),
    (ins GPR:$rs1, GPR:$rs2),
    !strconcat(inst, "\t$rd, $rs1, $rs2"),
    [(set GPR:$rd, (node GPR:$rs1, GPR:$rs2))],
    IIAlu
    >;
def ADD : ArithLogicR<0b0000000, 0b000, "add", add>;
def SUB : ArithLogicR<0b0100000, 0b000, "sub", sub>;
def MUL : ArithLogicR<0b0000001, 0b000, "mul", mul>;
def DIV : ArithLogicR<0b0000001, 0b100, "div", sdiv>;
def REM : ArithLogicR<0b0000001, 0b110, "rem", srem>;
def AND : ArithLogicR<0b0000000, 0b111, "and", and>;
def OR  : ArithLogicR<0b0000000, 0b110, "or", or>;
def XOR : ArithLogicR<0b0000000, 0b100, "xor", xor>;
def SLL : ArithLogicR<0b0000000, 0b001, "sll", shl>;
def SRL : ArithLogicR<0b0000000, 0b101, "srl", srl>;
def SRA : ArithLogicR<0b0100000, 0b101, "sra", sra>;

//def ADD : I<(outs GPR:$rd),
//                (ins GPR:$rs1, GPR: $rs2),
//                "add $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR: $rs1, GPR: $rs2))]>;

def : Pat<(i32 (imm12:$imm12)), (ADDI ZERO, imm12:$imm12)>;


let isCall = 1 in {
    def JALR : I<0b1100111, 0b000, (outs GPR:$rd),
                (ins GPR:$rs1, imm12:$imm),
                "jalr $rd, $rs1, $imm",
                [],IIBranch>;
}

let isBarrier = 1, isReturn = 1, isTerminator = 1 in {
  def PseudoRET : PseudoInst<
              (outs),
              (ins),
              [(retglue)]>,
              PseudoInstExpansion<(JALR ZERO, RA, 0)>;
}

/// 1. Call 的 SDNode定义
/// 2. SDNode pattern Instruction(Call的instruction)

def OneSDT_Call : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def one_call  : SDNode<"OneISD::Call", OneSDT_Call,
                            [SDNPHasChain, SDNPOutGlue,
                             SDNPOptInGlue, SDNPVariadic]>;

// def tglobaladdr : SDNode<"ISD::TargetGlobalAddress",  SDTPtrLeaf, [],
//                            "GlobalAddressSDNode">;

// def call_symbol : Operand<iPTR>;

def CALL : PseudoInst<(outs), (ins GPR:$rs1), [(one_call GPR:$rs1)]>, PseudoInstExpansion<(JALR RA, GPR:$rs1, 0)> {
    let isCall = 1;
}

// def HI: SDNode<"OneISD::HI", SDTIntUnaryOp>;
// def LO: SDNode<"OneISD::LO", SDTIntUnaryOp>;

def LUI : U<0b0110111, (outs GPR:$rd), (ins imm20: $imm), "lui \t$rd, $imm", [],IIAlu>;

// def : Pat<(HI tglobaladdr: $in), (LUI tglobaladdr:$in)>;
// def : Pat<(add GPR: $hi, (LO tglobaladdr: $lo)), (ADDI GPR:$hi, tglobaladdr:$lo)>;

def : InstAlias<"ret", (JALR ZERO, RA, 0)>;
def : InstAlias<"li $rd, $imm",  (ADDI GPR:$rd, ZERO, imm12:$imm)>;

def SLT : R<0b0110011, 0b0000000, 0b010, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2), "slt \t$rd, $rs1, $rs2", []>;

def : Pat<(setgt GPR:$rs1, GPR:$rs2), (SLT GPR:$rs2, GPR:$rs1)>;
def : Pat<(setlt GPR:$rs1, GPR:$rs2), (SLT GPR:$rs1, GPR:$rs2)>;
def : Pat<(seteq GPR:$rs1, GPR:$rs2), (SLT (XOR GPR:$rs1, GPR:$rs2), 1)>;
def : Pat<(setne GPR:$rs1, GPR:$rs2), (SLT ZERO, (XOR GPR:$rs1, GPR:$rs2))>;
def : Pat<(setge GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs1, GPR:$rs2), 1)>;
def : Pat<(setle GPR:$rs1, GPR:$rs2), (XORI (SLT GPR:$rs2, GPR:$rs1), 1)>;

def brtarget : Operand<OtherVT>;

let isTerminator = 1 in {
    def J : J<0b1101111, (outs), (ins brtarget:$imm20), "jal\tzero ${imm20}", [(br bb:$imm20)],IIBranch> {
        let rd = 0;
    }

}

class BRANCH<bits<3> funct3, string asm> : B<0b1100011, funct3, (outs), (ins GPR:$rs1, GPR:$rs2, brtarget:$imm12),
    !strconcat(asm, "\t$rs1, $rs2, $imm12"), [],IIBranch>;


def BLT : BRANCH<0b100, "blt">;
def BEQ : BRANCH<0b000, "beq">;
def BNE : BRANCH<0b001, "bne">;
def BGE : BRANCH<0b101, "bge">;

def : Pat<(brcond (i32 (setlt GPR:$rs1, GPR:$rs2)), bb:$dst), (BLT GPR:$rs1, GPR:$rs2, brtarget:$dst)>;
def : Pat<(brcond (i32 (setgt GPR:$rs1, GPR:$rs2)), bb:$dst), (BLT GPR:$rs2, GPR:$rs1, brtarget:$dst)>;
def : Pat<(brcond (i32 (seteq GPR:$rs1, GPR:$rs2)), bb:$dst), (BEQ GPR:$rs1, GPR:$rs2, brtarget:$dst)>;
def : Pat<(brcond (i32 (setne GPR:$rs1, GPR:$rs2)), bb:$dst), (BNE GPR:$rs1, GPR:$rs2, brtarget:$dst)>;
def : Pat<(brcond (i32 (setge GPR:$rs1, GPR:$rs2)), bb:$dst), (BGE GPR:$rs1, GPR:$rs2, brtarget:$dst)>;
def : Pat<(brcond (i32 (setle GPR:$rs1, GPR:$rs2)), bb:$dst), (BGE GPR:$rs2, GPR:$rs1, brtarget:$dst)>;
def : Pat<(br bb:$dst), (J brtarget:$dst)>;

// def : I<(outs GPR:$rd), (ins ptr:$p), "addi \t$rd, $p", [(set GPR:$rd, addrFI:$p)]>;

def : Pat<(addrFI GPR:$rs1, imm12:$imm12), (ADDI GPR:$rs1, imm12:$imm12)>;