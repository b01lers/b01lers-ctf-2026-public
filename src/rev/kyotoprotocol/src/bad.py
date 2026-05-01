import math
import random

from PIL import Image

random.seed(13)

def sigmoid(x):
	return 1 / (1 + math.exp(-x))

def isWhite(pix):
	if (pix[0] > 127):
		return 1
	return 0



data = ""
jump_table = []

for frame in range(2190):
	print("processing frame: " + str(frame+1))
	im = Image.open('bad_frames/out-' + str(frame+1) + '.png','r')
	size = im.size
	mat = []
	pixelmat = list(im.getdata())
	for col in range(size[1]):
		count = 0
		pixel = 0
		c = []
		for row in range(size[0]):
			pix = isWhite(pixelmat[col*size[0]+row])
			if (pix == pixel):
				count = count + 1
			else:
				c.append("\\x{:02x}".format(count))
				count = 1
				pixel = pix
		c.append("\\x{:02x}".format(count))
		mat.append(c)
	mat = ["".join(col) for col in mat]
	mat = "".join(mat)
	jump_table.append(len(data)//4)
	data += mat
	# file = open('ph_output/f' + str(frame+1) + '.lua', 'w+')
	# file.write("badappledata[" + str(frame+1) + "] = {" + mat + "}")
	# file.close()	# file.write("badappledata[" + str(frame+1) + "] = {" + mat + "}")
	# file.close()
print(jump_table)
print(len(data) // 4)
file = open('badapple.txt', 'w+')
file.write(data)
file.close()