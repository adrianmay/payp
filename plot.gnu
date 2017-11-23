set y2tics
plot "res" using 1:2 title 'Pop' with lines,\
     "res" using 1:4 title 'Repro' axes x1y2 with lines,\
     "res" using 1:3 title 'Bal' with lines
