.extern _ZN5Riscv16interruptHandlerEv
.extern _ZN5Riscv14consoleHandlerEv
.extern _ZN5Riscv12timerHandlerEv

.align 4
.global SuperTrap
.type SuperTrap, @function

SuperTrap:
    j ecall
    j timer
    sret
    sret
    sret
    sret
    sret
    sret
    sret
    j console

ecall:
    addi sp, sp, -256
    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv16interruptHandlerEv

    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret

timer:
    addi sp, sp, -256
    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv12timerHandlerEv

    .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret


 console:
     addi sp, sp, -256
     .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
     sd x\index, \index * 8(sp)
     .endr

     call _ZN5Riscv14consoleHandlerEv

     .irp index, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
     ld x\index, \index * 8(sp)
     .endr
     addi sp, sp, 256

     sret