#!/usr/bin/env python
import os
import sys
import struct
import array
import shutil
import subprocess
from struct import pack, unpack
from Crypto.PublicKey import RSA
from Crypto.Util.number import long_to_bytes
KeyType='RSA2048'
MagicNum='SIMLOCK_MAGIC_S'
KeyUpDir = "/SimlocKeys/"
KeyIDTable = [0x1011, 0x1012, 0x1013, 0x1014, 0x1015]
KeysDirs = ["authstartkey", "authendkey", "simlockey", "imeikey", "reskey"]

def GetKeyFiles(fdir):
	kf = ""
	fls=os.listdir(fdir)
	j = 0
	for i in fls:
		if os.path.splitext(i)[1] == ".pem":
			#print "Find key: " + i
			if kf == "":
				kf = i
			j = j + 1
		if j > 1:
			kf = ""
	return j, kf

def get_head_offset(tos_img,tos_len):
	secpos = tos_img.find(MagicNum.encode('utf-8'))
	if secpos < 0:
		print ("there is no head!!!")
		return 0
	else:
		print ("Find magic num: " + MagicNum)
		return  secpos

def InsertKey(kpath,kid,img):
	f = open(kpath, 'rb')
	if f:
		k = RSA.importKey(f.read())
		f.close
		Mlen = (k.publickey().size()+7)//8
		Nlen = len(long_to_bytes(k.publickey().n))
		Elen = len(long_to_bytes(k.publickey().e))
		if sys.version_info[0] == 3:
			img.write(struct.pack("<I", kid))
			img.write(Mlen.to_bytes(4, 'little'))
			img.write(KeyType.encode())
		else:
			img.write('%s' % struct.pack("<I", kid))
			img.write('%s' % struct.pack("<I", Mlen))
			img.write(KeyType)
		img.seek(1,1)
		if sys.version_info[0] == 3:
			img.write(struct.pack("<I", Nlen))
		else:
			img.write('%s' % struct.pack("<I", Nlen))

		for x in array.array("B", long_to_bytes(k.publickey().n)):
			data_n = struct.pack("<B",x)
			img.write(data_n)
		img.seek(512-Nlen, 1)
		if sys.version_info[0] == 3:
			img.write(struct.pack("<I", Elen))
		else:
			img.write('%s' % struct.pack("<I", Elen))
		for x in array.array("B", long_to_bytes(k.publickey().e)):
			data_e = struct.pack("<B",x)
			img.write(data_e)
		img.seek(512-Elen, 1)
		#print ("Insert key["+hex(kid)+"]:\""+os.path.basename(kpath)+"\" success")
		return 1
	else:
		print ("Can\'t find key: \""+kpath+"\"")
		return 0

def main():
	keypath = sys.argv[2]
	destfile = sys.argv[3]
	shutil.copy(sys.argv[1], destfile)
	dfile = destfile.rsplit("/", 1)
	print("Insert simlock Key:[" + dfile[1] + "]")
	if os.path.exists(destfile):
		f1 = open(destfile,'rb+')
		bindata = f1.read()
		bin_len = len(bindata)
		offset = get_head_offset(bindata,bin_len)
		if offset == 0:
			print ("Can\'t find head magic num")
		else:
			print ("Magic Offset is " + str(offset))
			f1.seek(offset+16,0)
			for n in range(0,5):
				knum, kfile = GetKeyFiles(keypath+KeyUpDir+KeysDirs[n])
				if knum == 1:
					InsertKey(keypath+KeyUpDir+KeysDirs[n]+"/"+kfile, KeyIDTable[n], f1)
			print ("Insert simlock key success")
		f1.close
	else:
		print ("Cp bin is not accessible!!!")

if __name__ == "__main__":
	main()
