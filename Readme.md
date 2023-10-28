# Part1: Indirect Call Detection
[Part1: Indirect Call Detection](https://github.com/LittleLerry/llvm-pass-itargets/)
[PART2: Runtime Indirect Call Detection](https://github.com/LittleLerry/llvm-pass-icg/)

## What's the final goal?

Basically, we want to modify AFLGO to construct a more complete Control Flow Graph (CFG) than what 
we obtain from LLVM. The algorithm we will use consists of the following steps:

(1) Generate the target file using `llvm-pass-itargets` and run AFLGO;
(2) During runtime, detect extra indirect calls and obtain proper output with the given inputs;
(3) Combine the output in step 2 with the current CFG/CG to construct a new CFG/CG;
(4) If necessary, stop AFLGO to recalculate distances;
(5) Go back to step (1) and repeat the process;

The loop should be terminated at an appropriate time.

## What's this part do?

Generate the target file.

## Function call types, CFG and CG

In programming languages, direct calls are the typical form of function calls, where the 
target function is known at compile-time. However, in certain cases, the target function may be 
determined dynamically based on runtime conditions or computed values. An indirect call refers 
to a type of function call where the target function is determined dynamically at runtime.

A Control Flow Graph (CFG) is a graphical representation of the control flow or flow of 
execution within a program. And a Call Graph (CG) is a representation of the calling relationships 
between functions in a program.

## Methodology
LLVM passes are compiler optimizations which operate on LLVM's intermediate representation (IR). 
Each pass takes an LLVM module as input, applies a transformation to it, and returns a 
new module. Basically, we will use a LLVM Pass to detect all indirect calls in the program 
when we compiling it, where an indirect call refers to a function pointer or a virtual function 
being used to call function. The following code shows the core idea of Part1.

```c++
for (auto &F : M) {
    for (auto &BB : F) {
        for (auto &I : BB) {				
            if (CallInst *CI = dyn_cast<CallInst>(&I)) {
                Function *calledFunction = CI->getCalledFunction();
                    if (!calledFunction) {
                        DebugLoc Loc = CI->getDebugLoc();
                        unsigned line = Loc->getLine();
                        StringRef file = Loc->getFilename();
                        bbtargets << file.substr(file.find_last_of("/\\") + 1).str() << ":" << line << "\n";
                    }
            }				
        }
    }
}
```

We iterate over all the instructions in the module. We use `CallInst *CI = dyn_cast<CallInst>(&I)` 
to test if an instruction is a call instruction or not. If so, we further test it with 
`CI->getCalledFunction()`. For direct calls, we can always obtain called functions. However, for 
indirect calls, we cannot obtain them until the runtime. Once we detect an indirect call instruction, 
we record its information in a file. Those records will be targets being fed to AFLGO.

## Integrated into AFLGO

Please note that this part has been intergrated into AFLGO, which means that some depricated methods 
may be invoked.

## Build and test llvm-pass-itargets

First you need to properly install the original version of aflgo. Then, replace the whole `/instrument` 
and `/example` with the two directories in this project. Then you want to recomplie the `aflgo-clang`, 
`aflgo-clang++` and dynamic libs using following:
```bash
# suppose you have properly installed aflgo and have replaced correspondding files
# and suppose the $PWD is aflgo main folder
cd instrument
make
```
You may notice some error messages in your shell, it's totally fine because the our modifed passes cannot 
pass the test cases provided. After doing this, all stuff have been prepared. The lib will be used 
automatically by AFLGO.

### Test for a simple case
Generate BBtargets.txt using:
```bash
# suppose the $PWD is aflgo main folder
export AFLGO=$PWD
cd example
./indirect_call_test.sh
```
The BBtargets.txt will be generated.

### Test for libxml2
Generate BBtargets.txt using:
```bash
# suppose the $PWD is aflgo main folder
export AFLGO=$PWD
cd example
./libxml2_test.sh
```
The BBtargets.txt will be generated.
