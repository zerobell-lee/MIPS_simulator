# MIPS_simulator
MIPS Simulator

I was studying Computer Architecture Unit in 2016 2nd Semester.
This project was to figure out how MIPS works.
For the details, refer the .ppt file.

***
## Introduction

MIPS Simulator simulates simple MIPS.
You can load binary files and execute them.
In addition, you can view Registors' state so that you can monitor how the instructions go.

## How to use

MIPS Simulator is CLI program. So to use this simulator, you should type appropirate prompts in the Simulator.

`>>>l input` means Load input file. You can replace `input` to the name of file which you want to load.

`>>>m` means Show memories. You can view addresses and values of VM of MIPS Simulator. But you have to clarify FROM WHERE TO WHERE.

`>>>m 0 16` means to view values of VM from address 0 to address 16. Address argument should be a multiple of 4. Unless the argument is a multiple of 4, prompt will show Execption Occured.
You can put the argument with hex value. Just like `>>>m 0x0 0x10`

`>>>s` means Step. After you load binary instruction set, you can execute Step. In Step, MIPS simulator execute just a word instruction.

`>>>sr` means Set registors. You can use this instruction just like `>>>m`

`>>>sr 7 5` means to set a value of Registor 7 to 5. First argument means the number of Registor, and Second argument means the value that you want to set.

`>>>g` means Go. Unlike Step, Go loops sequential instructions until MIPS Simulator meets syscall.

`>>>j` means Jump. You can change the value of PC(Program Counter). `>>>j 0x10` means to jump 16th word so that MIPS simulator execute introduction from 16th line.

`>>>x` has two functions. When MIPS_simulator has loaded binary file, `>>>x` means to unload the binary file.
 When MIPS_simulator doesn't have loaded binary file, `>>>x` means to exit.