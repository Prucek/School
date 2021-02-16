
for i in 1 5 10 50 100 500 1000; do
    echo "Size=$i MiB"
    for j in `seq 1 10`; do
        mbw $i >mbw-$i
    done
done
