
Test various performance-related problems


test-volatile: interesting paradox --
  volatile access is faster if we call extra empty function

test-branch-predict: branch predictor effects (Intel i5 only)
  (We need conditional jump instruction in the loop. Newer GCC compilers
   optimize the jump using CMOV instruction, which is not predicted.
   We need extra GCC options to switch the optimization off. See Makefile)

test-align: unaligned access timing difference

test-containers: Insert operation cost for various containers

