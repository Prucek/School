
P=test-volatile2

echo Running test:

echo "no CALL:"
gcc -g -O2 $P.c -o $P              && time ./$P
objdump --section=".text" -S $P >${P}.as

echo "with CALL:"
gcc -g -O2 -DCALL $P.c -o $P       && time ./$P
objdump --section=".text" -S $P >${P}-call.as

echo Done.
rm -f $P

# meld ${P}.as ${P}-call.as

