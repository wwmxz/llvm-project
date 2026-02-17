
class OneInst<dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin> : Instruction {
    field bits<32> SoftFail = 0;  // 软失败标记，指令执行失败时的容错标志：不处理、直接报错
    let Size = 4;                 // 指令长度固定为4字节（RISC-V 32位指令）
    let Namespace = "One";        // 指令所属命名空间，避免与其他架构冲突
    let Itinerary = itin;         // 指令调度周期（流水线信息）
    dag OutOperandList = outs;    // 输出操作数列表（如目标寄存器）
    dag InOperandList = ins;      // 输入操作数列表（如源寄存器/立即数）
    let AsmString = asmstr;       // 汇编字符串格式（用于汇编打印）
    let Pattern = pattern;        // 代码生成模式（映射LLVM IR到机器指令）
    field bits<32> Inst;          // 32位指令编码的存储字段
    bits<7> Opcode = 0;           // 7位操作码字段
    let Inst{6-0} = Opcode;       // 操作码固定在指令的0-6位（RISC-V标准）
}
class PseudoInst<dag outs, dag ins, list<dag> pattern, string asmstr = "",InstrItinClass itin=IIPseudo>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let isPseudo = 1; // 标记为伪指令（不直接对应硬件编码）
    let isCodeGenOnly = 1;// 仅用于代码生成阶段，最终会被展开为真实指令
}
// 1. R型指令格式（Register-Register）
class R<bits<7> opcode, bits<7> funct7, bits<3> funct3, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;

    bits<5> rd;
    bits<5> rs1;
    bits<5> rs2;

    let Inst{31-25} = funct7;
    let Inst{24-20} = rs2;
    let Inst{19-15} = rs1;
    let Inst{14-12} = funct3;
    let Inst{11-7} = rd;
}
// 2. I型指令格式（Immediate/Load/Jalr）
class I<bits<7> opcode, bits<3> funct3, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;

    bits<5> rd;
    bits<5> rs1;
    bits<12> imm;

    let Inst{31-20} = imm;
    let Inst{19-15} = rs1;
    let Inst{14-12} = funct3;
    let Inst{11-7} = rd;
}

class U<bits<7> opcode, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;

    bits<5> rd;
    bits<20> imm;

    let Inst{31-12} = imm;
    let Inst{11-7} = rd;
}
// 4. J型指令格式（Jump）
class J<bits<7> opcode, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;
    bits<20> imm20;
    bits<5> rd;

    let Inst{31} = imm20{19};
    let Inst{30-21} = imm20{9-0};
    let Inst{20} = imm20{10};
    let Inst{19-12} = imm20{18-11};
    let Inst{11-7} = rd;
}
// 5. B型指令格式（Branch）
class B<bits<7> opcode, bits<3> funct3, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;
    bits<5> rs2;
    bits<5> rs1;
    bits<12> imm12;

    let Inst{31} = imm12{11};
    let Inst{30-25} = imm12{9-4};
    let Inst{24-20} = rs2;
    let Inst{19-15} = rs1;
    let Inst{14-12} = funct3;
    let Inst{11-8} = imm12{3-0};
    let Inst{7} = imm12{10};
}
// 6. S型指令格式（Store）
class S<bits<7> opcode, bits<3> funct3, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
    : OneInst<outs, ins, asmstr, pattern,itin> {
    let Opcode = opcode;

    bits<5> rs1;
    bits<5> rs2;
    bits<12> imm;

    let Inst{31-25} = imm{11-5};
    let Inst{24-20} = rs2;
    let Inst{19-15} = rs1;
    let Inst{14-12} = funct3;
    let Inst{11-7} = imm{4-0};
}



