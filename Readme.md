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

### What files in aflgo/instrument do?

### What we have modified in aflgo/instrument?

### What we have modified in aflgo/examples?

## Test llvm-itargets-pass
