
# Set the output to a png file
set terminal x11
#set terminal png size 500,500
# The file we'll write to
#set output 'test1.png'
# The graphic title
set title 'Voltage'

# derivative functions.  Return 1/0 for first point, otherwise delta y or (delta y)/(delta x)
d(y) = ($0 == 0) ? (y1 = y, 1/0) : (y2 = y1, y1 = y, y1-y2)


#while (1) {
p "test1.txt" u 2:3 every ::1320::2320,\
"test1.txt" u 2:4 every ::1320::2320,\
#"test1.txt" u ($2):(d($2)) title '1-variable derivative', 
#p "test1.txt" u 2:3 every ::2000::2200 w lp,\
    #"test1.txt" u 2:3 w lp,\


;pause -1  "Hit return to continue"
#}

