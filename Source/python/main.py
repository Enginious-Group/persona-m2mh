from operator import truediv
import argparse
import pathlib
import unreal
import subprocess
import sys
import importlib
import time
import os
import json
from datetime import datetime

def import_texture():
    import_task = unreal.AssetImportTask()
    import_task.automated = True
    import_task.destination_name =''
    import_task.destination_path = '/Persona3d/Assets/'+ name + '/'
    import_task.filename = str(newPathPng)
    import_task.replace_existing = True
    import_task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([import_task])

def import_mesh():
    import_task = unreal.AssetImportTask()   
    import_task.destination_name = ''   
    import_task.destination_path = '/Persona3d/Assets/' + name + '/'
    import_task.filename = str(newPathObj)
    import_task.replace_existing = True   
    import_task.automated = True
    import_task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([import_task])

def not_unreal_actions():    
    #os.system(r'start /wait cmd /c py D:\\03_12\\Persona51\\Plugins\\Persona3d\\Source\\python\\client.py -ip "127.0.0.1" "D:\03_12\Persona51\Plugins\Persona3d\Assets"')
    clientpath = pathlib.Path(args.content + '/Source/python/client.py')
    os.system(f"start /wait cmd /c py {clientpath} -assets {folderPath} -ip {args.ip} -path {args.path} -name {name}")
    print(f"start /wait cmd /c {clientpath} -assets {folderPath} -ip {args.ip} -path {args.path} -name {name}")
    

def create_person_folder():
    if not os.path.exists(folderPath):
        os.makedirs(folderPath)

def rename_assets():
    os.rename(oldPathObj, newPathObj)
    os.rename(oldPathPng, newPathPng)

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

    now = datetime.now()
    dt = now.strftime("%d%m%Y%H%M")
    name = args.name +'_'+ dt
    namewithobj =  name + '.obj'
    namewithpng =  't_' + name + '.png'
    folderPath = pathlib.Path(args.content + '/Assets/' + name)
    oldPathObj = pathlib.PurePath(folderPath, 'ML_Head.obj')
    newPathObj = pathlib.PurePath(folderPath, namewithobj)
    oldPathPng = pathlib.PurePath(folderPath, 'ML_Head.png')
    newPathPng = pathlib.PurePath(folderPath, namewithpng)
    
    
    print(args.content)
    print(args.ip)
    print(args.path)
    print(name)
    create_person_folder()
    not_unreal_actions()
    rename_assets()
    import_mesh()
    import_texture()
    
   

