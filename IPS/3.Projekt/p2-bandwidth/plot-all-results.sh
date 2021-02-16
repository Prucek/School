
D1=memcpy-bench-data
D2=memset-bench-data
D3=read-loop-bench-data
D4=write-loop-bench-data
D5=copy-loop-bench-data
gnuplot <<__END__
set term pdf; set output "mem-benchmark.pdf"
set grid
set logscale x
set xlabel "block size [KiB]"
set ylabel "memory bandwidth [MB/s]"
set style data lines
plot [0.1:5e5][0:] "$D1", "$D2", "$D3", "$D4", "$D5"
__END__
