
if [ $# = 0 ]; then
    echo "Usage: $0 data-file"
    exit 1
fi

DATA="$1"

gnuplot <<__END__
#set term postscript color; set output "$DATA.ps"
set term pdf; set output "$DATA.pdf"
set grid
set title "$DATA"
set nokey
set logscale x
set xlabel "block size [KiB]"
set ylabel "memory bandwidth [MB/s]"
set style data lines
plot [][0:] "$DATA" using 1:2 lw 2
__END__
