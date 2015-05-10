import sys
import longmodule
if len(sys.argv) < 5 or len(sys.argv)>7:
	print "Incorrect input!")
	print("Input string format: file1 operation(+, -, / , m, %) file2 resultfile [-b]");
	print("PowMod: file ^ powfile modfile resultfile [-b]");
	exit()

a = longmodule.LONG()
c = longmodule.LONG()
mod = longmodule.LONG()

name_a = sys.argv[1]
z = sys.argv[2]
name_c = sys.argv[3]

if z=="^":
	name_mod = sys.argv[4]
	name_res = sys.argv[5]
else:
	name_mod = 0
	name_res = sys.argv[4]

if sys.argv[-1] == "-b":
	a.FromBinFile(name_a)
	if a.IsEmpty():
		exit()
	c.FromBinFile(name_c)
	if c.IsEmpty():
		exit()
	if name_mod!=0:
		mod.FromBinFile(name_mod)
		if mod.IsEmpty():
			exit()
else:
	a.FromTextFile(name_a)
	if a.IsEmpty():
		exit()
	c.FromTextFile(name_c)
	if c.IsEmpty():
		exit()
	if name_mod!=0:
		mod.FromTextFile(name_mod)
		if mod.IsEmpty():
			exit()		
	
if z == "+":
	res = a + c
if z == "-":
	res = a - c
if z == "m":
	res = a * c
if z == "/":
	res = a / c
if z == "%":
	res = a % c
if z == "^":
	res = a.PowMod(c,mod)

if sys.argv[-1] == "-b":
	if res.ToBinFile(name_res) == False:
		print("Error by writing resultfile!")
else:
	if res.ToTextFile(name_res) == False:
		print("Error by writing resultfile!")


