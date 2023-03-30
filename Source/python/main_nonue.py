import argparse
import pathlib
import os
import shutil
import subprocess
import sys
import importlib
from datetime import datetime



def requests_preparation():
    package_name = "requests"
    try:
        importlib.import_module(package_name)
        print(f"The {package_name} package is already installed.")
    except ImportError:
        print(f"The {package_name} package is not installed. Installing it now...")
        subprocess.check_call([f"{pythonExePath}","-m","pip", "install", package_name])
        print(f"The {package_name} package has been installed.")

def not_unreal_actions():    
    clientpath = pathlib.Path(args.content + '/Source/python/client.py')
    os.system(f"start /wait cmd /c \"\"{pythonExePath}\" \"{clientpath}\" -assets \"{folderPath}\" -ip \"{args.ip}\" -path \"{os.path.join(folderPath, namewithpng)}\" -name \"{name}\"\"")
def create_person_folder():
    if not os.path.exists(folderPath):
        os.makedirs(folderPath)
    shutil.copyfile(args.path, os.path.join(folderPath, namewithpng))
    

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description = 'persona')
    parser.add_argument(
        '-content',
        help='path to plugin folder'
    )
    parser.add_argument(
        '-ip',
        help='ip of server'
    )
    parser.add_argument(
        '-path',
        help='path to concept'
    )
    parser.add_argument(
        '-name',
        help='name of character'
    )
    parser.add_argument(
        '-ppath',
        help='unreal python path'
    )


    args = parser.parse_args()
    
    if(args.content):
        OPTION = args.content


    name = args.name
    ##

    namewithpng =  name + '.png'
    folderPath = pathlib.Path(args.content + '/Assets/' + name)
    #folderPath = os.path.abspath(os.path.join(unreal.Paths.project_dir()))
    pathwithobj = pathlib.Path(str(folderPath) +'/' + name + '.obj')
    tpathwithpng = pathlib.Path(str(folderPath) +'/t_' + name + '.png')
    editorPath = sys.executable
    #pythonExePath = pathlib.Path(editorPath[:-23]  + '/ThirdParty/Python3/Win64/python.exe')
    pythonExePath = args.ppath

    requests_preparation()
    create_person_folder()
    not_unreal_actions()