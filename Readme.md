Encoder:

use "g++ main.cpp parser.cpp encoder.cpp" to compile

use "./a.out < XXX.v > encode.v" to excute encoder



Miter:

use "g++ miter.cpp" to compile

use "cat encode_g.v encode_r.v > tmp.v" to connect two files

"./a.out < tmp.v > miter.v" to add miter