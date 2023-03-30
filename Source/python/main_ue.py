import argparse
import pathlib
import unreal
import os
import shutil
import subprocess
import sys
import importlib
from datetime import datetime


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

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description = 'persona')
    parser.add_argument(
        '-content',
        help='path to plugin folder'
    )
    parser.add_argument(
        '-name',
        help='name of character'
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

    import_mesh()
    import_texture()