import argparse
import pathlib
import unreal
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

def import_texture():
    import_task = unreal.AssetImportTask()
    import_task.automated = True
    import_task.destination_name =''
    import_task.destination_path = '/Game/Persona/'+ name + '/'
    import_task.filename = str(tpathwithpng)
    import_task.replace_existing = True
    import_task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([import_task])

def import_mesh():
    import_task = unreal.AssetImportTask()   
    import_task.destination_name = ''   
    import_task.destination_path = '/Game/Persona/'+ name + '/'
    import_task.filename = str(pathwithobj)
    import_task.replace_existing = True   
    import_task.automated = True
    import_task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([import_task])

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


    args = parser.parse_args()
    
    if(args.content):
        OPTION = args.content

    ##
    now = datetime.now()
    dt = now.strftime("%d%m%H%M%S")
    name = args.name +'_'+ dt
    ##

    namewithpng =  name + '.png'
    folderPath = pathlib.Path(args.content + '/Assets/' + name)
    #folderPath = os.path.abspath(os.path.join(unreal.Paths.project_dir()))
    pathwithobj = pathlib.Path(str(folderPath) +'/' + name + '.obj')
    tpathwithpng = pathlib.Path(str(folderPath) +'/t_' + name + '.png')
    editorPath = sys.executable
    pythonExePath = pathlib.Path(editorPath[:-23]  + '/ThirdParty/Python3/Win64/python.exe')
    
    print(folderPath)

    requests_preparation()
    create_person_folder()
    not_unreal_actions()
    import_mesh()
    import_texture()
    
   

