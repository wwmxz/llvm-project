// 返回值放入a0-a1 (x10-x11)
def RetCC_Myrisc : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0, A1]>>
]>;
// 默认调用约定
def CC_Myrisc : CallingConv<[
 // 整数参数优先放入a0-a2 (x10-x12)
    CCIfType<[i32], CCAssignToReg<[A0, A1, A2]>>,
// 剩余参数入栈
    CCAssignToStack<4, 4>

]>;
// 被调用者的保存寄存器
def CSR : CalleeSavedRegs<(add RA, S0, S1)>;