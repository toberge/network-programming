from pickle import loads
import sys

#data = sys.argv[1]
data = '80049528000000000000005d94284740000000000000004740000000000000008c012b944740100000000000008c012d94652e'
data = [0x80, 0x04, 0x95, 0x28, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x5d, 0x94, 0x28, 0x47, 0x40, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x8c, 0x01, 0x2b, 0x94, 0x47, 0x40, 0x10, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x01, 0x2d, 
0x94, 0x65, 0x2e]
print(loads(bytes(data)))
data = [0x80, 0x04, 0x4b, 0x00, 0x2e]
print(loads(bytes(data)))