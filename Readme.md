# Part1 Indirect Call Detection

## LLVM Pass
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

### Modification in aflgo/instrument


### Modification in aflgo/example

## Build and Test llvm-itargets-pass

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
