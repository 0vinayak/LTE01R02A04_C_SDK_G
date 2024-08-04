# !/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import re
import datetime
import sys
import json
import argparse

class BuildCheck(object):
    def __init__(self):
        self._des = []
        pass
    
    def partition_check(self,path):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            temp = int("1000000",16)
            Align4K = int("1000",16)
            
            #####   macros   ######
            APP_IMG_ADDR = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_ADDRESS'].replace('0x',''),16)  #APP IMG 地址
            APP_IMG_SIZE = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_SIZE'].replace('0x',''),16)     #APP IMG 大小
              
            FS_SYS_ADDR = int(partiton_data['macros']['CONFIG_FS_SYS_FLASH_ADDRESS'].replace('0x',''),16)   #FS 地址
            FS_SYS_SIZE = int(partiton_data['macros']['CONFIG_FS_SYS_FLASH_SIZE'].replace('0x',''),16)      #FS 大小
            #####   descriptions   ####
            DES_FS_OFFEST = int(partiton_data['descriptions'][0]['offset'].replace('0x',''),16)             #FS 偏移
            DES_FS_SIZE = int(partiton_data['descriptions'][0]['size'].replace('0x',''),16)                 #FS 大小
            Fs_Align = int(partiton_data['descriptions'][0]['erase_block'].replace('0x',''),16)             #FS 对齐大小
            
            if(partiton_data['macros']['CONFIG_APPIMG_FLASH_ADDRESS'][2] == partiton_data['macros']['CONFIG_FS_SYS_FLASH_ADDRESS'][2]) :
                if (APP_IMG_ADDR+APP_IMG_SIZE > FS_SYS_ADDR):
                    raise Exception("partition size error")
                if ("CONFIG_APPIMG_FLASH2_ENABLED" in partiton_data['macros']) == True :
                    flash4M_size = int("400000",16)
                    if (FS_SYS_SIZE + APP_IMG_SIZE >flash4M_size) or (FS_SYS_SIZE + FS_SYS_ADDR > (APP_IMG_ADDR + flash4M_size)): #APP IMG 及 FS 在外挂4M FLASH 上，SIZE大小分配错误
                        raise Exception("partition 4Mflash oversize")
                else :
                    FS_MODEM_ADDR = int(partiton_data['macros']['CONFIG_FS_MODEM_FLASH_ADDRESS'].replace('0x',''),16)
                    if (FS_MODEM_ADDR < FS_SYS_SIZE + FS_SYS_ADDR):
                        raise Exception("partition 8Mflash oversize")
            else : 
                if ("CONFIG_FS_EXT_ENABLED" in partiton_data['macros']) == True :
                    if(partiton_data['macros']['CONFIG_FS_EXT_ENABLED'] == "on") :
                        print("using ext flash")#do nothing
                else:
                    print("no ext flash")#do nothing
            
            if (FS_SYS_SIZE != DES_FS_SIZE) or (FS_SYS_ADDR % temp != DES_FS_OFFEST):
                raise Exception("partition file system size error")
            
            if (APP_IMG_SIZE % Align4K != 0) or (FS_SYS_SIZE % Fs_Align !=0) or (APP_IMG_ADDR % Align4K != 0) or (FS_SYS_ADDR % Fs_Align !=0):     #检测地址和大小对齐
                raise Exception("partition size align error")

            f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1
      
    def kernel_size_check(self,path,kernel_img):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            APP_IMG_SIZE = int(partiton_data['macros']['CONFIG_APP_FLASH_SIZE'].replace('0x',''),16)
            stats = os.stat(kernel_img)
            coreimg_size = stats.st_size
            if(coreimg_size > APP_IMG_SIZE):
                raise Exception("kernel img oversize")
            remain_size = APP_IMG_SIZE-coreimg_size
            print("kernel total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,remain:0x{:04x}|{:.2f}KB".format(APP_IMG_SIZE,APP_IMG_SIZE/1024,coreimg_size,coreimg_size/1024,remain_size,remain_size/1024))
            f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1      
        
    def app_size_check(self,path,app_img):
        try:
            f = open(path, encoding="utf-8",mode='r')
            partiton_data = json.load(f)
            APP_SIZE = int(partiton_data['macros']['CONFIG_APPIMG_FLASH_SIZE'].replace('0x',''),16)
            stats = os.stat(app_img)
            app_size = stats.st_size
            if(app_size > APP_SIZE):
                raise Exception("app img oversize")
            remain_size = APP_SIZE-app_size
            print("app total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,remain:0x{:04x}|{:.2f}KB".format(APP_SIZE,APP_SIZE/1024,app_size,app_size/1024,remain_size,remain_size/1024))
            f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            f.close()
            return 1  
    def spl_size_check(self,path,spl_img,spl_sign_img):
        try:
            #f = open(path, encoding="utf-8",mode='r')
            #partiton_data = json.load(f)
            #SPL_SIZE = int(partiton_data['macros']['CONFIG_SPL_FLASH_SIZE'].replace('0x',''),16)
            SPL_SIZE = int(0x12C00) #固定75KB(预留5KB)
            if(os.path.isfile(spl_sign_img)):
                stats = os.stat(spl_sign_img)
            else:
                stats = os.stat(spl_img)
 
            spl_size = stats.st_size
            if(spl_size > SPL_SIZE):
                over_size = SPL_SIZE-spl_size
                print("spl total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,overflow:0x{:04x}|{:.2f}KB".format(SPL_SIZE,SPL_SIZE/1024,spl_size,spl_size/1024,over_size,over_size/1024))
                raise Exception("spl img oversize")
            remain_size = SPL_SIZE-spl_size
            print("spl total size:0x{:04x}|{:.2f}KB,used:0x{:04x}|{:.2f}KB,remain:0x{:04x}|{:.2f}KB".format(SPL_SIZE,SPL_SIZE/1024,spl_size,spl_size/1024,remain_size,remain_size/1024))
            #f.close()
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            #f.close()
            return 1 
    def file_size_check(self,path,size):
        try:
            stats = os.stat(path)
            file_size = stats.st_size
            if(file_size < size):
                raise Exception("file size error")
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            return 1
    def file_line_ending_check(self,path):
        try:
            with open(path, 'rb') as f:
                data = f.read()
                if b'\r\n' in data:
                    raise Exception("file ending with CRLF!")
            return 0
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            return 1
            
def main(argv):
    try:
        parser = argparse.ArgumentParser(description='manual to this script' , formatter_class=argparse.RawTextHelpFormatter)
        
        parser.add_argument("--coreimg", type=str, default="none")
        parser.add_argument("--appimg", type=str, default="none")
        parser.add_argument("--splimg", type=str, default="none")
        parser.add_argument("--splsignimg", type=str, default="none")
        parser.add_argument("--partinfo", type=str, default="none")
        
        parser.add_argument("--file", type=str, default="none")
        parser.add_argument("--size", type=int, default=0)
        parser.add_argument("--line_ending", 
                            type=str, 
                            default="quectel_build/tool/build_all.sh",
                            help="check if the linux build script ends with LF")
     
        opt = parser.parse_args()
        
        file_path = opt.file
        file_size = opt.size
        
        partiton_path = opt.partinfo
        core_img_path = opt.coreimg
        app_img_path = opt.appimg
        spl_img_path = opt.splimg
        spl_sign_img_path = opt.splsignimg

        linux_build_file_path = opt.line_ending 
        
        build_check = BuildCheck()
        
        if(partiton_path != "none"):
        
            if(core_img_path == "none" and app_img_path == "none" and spl_img_path == "none" and spl_sign_img_path == "none"):
                return build_check.partition_check(partiton_path)  
            
            elif (core_img_path != "none" and app_img_path == "none" and spl_img_path == "none" and spl_sign_img_path == "none"):
                return build_check.kernel_size_check(partiton_path,core_img_path)
        
            elif (core_img_path == "none" and app_img_path != "none" and spl_img_path == "none"and spl_sign_img_path == "none"):
                return build_check.app_size_check(partiton_path,app_img_path)
                
            elif (core_img_path == "none" and app_img_path == "none" and spl_img_path != "none" and spl_sign_img_path != "none"):
                return build_check.spl_size_check(partiton_path,spl_img_path,spl_sign_img_path)   
                
        elif (file_path != "none" and file_size!=0):
            return build_check.file_size_check(file_path,file_size)
        
        elif (linux_build_file_path != "none"):
            return build_check.file_line_ending_check(linux_build_file_path)
            
        else:
            raise Exception("invalid parameters") 
    
    except Exception as e:
        import traceback
        print(traceback.format_exc())
        return 1
        
if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
