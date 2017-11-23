binwidth=0.0001
bin(x,width)=width*floor(x/width)
plot 'probrepro.hist' using (bin($1,binwidth)):(1.0) smooth freq with boxes

