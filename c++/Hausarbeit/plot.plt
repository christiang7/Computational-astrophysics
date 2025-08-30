set terminal pdfcairo enhanced color
set xlabel "Frequenz w [1/s]"
set ylabel "P(w)"
set output "plot-doubleP.pdf"
plot "Fourier-doubleP.dat" using ($1):($2) with lines title 'P(w)'
#replot
set terminal pdfcairo enhanced color
