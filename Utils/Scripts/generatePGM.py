import array
import random
import sys

# define the width  (columns) and height (rows) of your image
width = 320
height = 240

# declare 1-d array of unsigned char and assign it random values
buff=array.array('B')

for i in range(0, width*height):
  buff.append(random.randint(0,3))


# open file for writing 
filename = 'x.pgm'

try:
  fout=open(filename, 'wb')
except IOError:
  print ("C\"annot open file “, filename, “Exiting … \n")
  sys.exit()


# define PGM Header
pgmHeader = str.encode('P5' + '\n' + str(width) + '  ' + str(height) + '  ' + str(255) + '\n')

# write the header to the file
fout.write(pgmHeader)

# write the data to the file 
buff.tofile(fout)

# close the file
fout.close()