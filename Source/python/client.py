import argparse
import logging
import os
from pathlib import Path
from zipfile import ZipFile
import requests


EPHEM_URL = "localhost"

output_dirpath = Path('D:\Persona')
###

class ApiError(Exception):
    pass


def call_api(input_filepath:Path, output_extension:str):

    url = f'http://{EPHEM_URL}:9092/process/obj_forward'

    # Open input file, and process with api 
    with open(input_filepath, 'rb') as file:
        r = requests.post(url, files={'file': file})

    if r.status_code == 200:
        logging.info(f"Persona returned results!")

        output_filepath = output_dirpath / 'response.zip'
        with open(output_filepath, 'wb') as output:
            output.write(r.content)

        return output_filepath

    else:
        raise ApiError(f"Persona failed! Status code: {r.status_code}")


def unzip_file(zip_filepath:Path):
 
    with ZipFile(zip_filepath, 'r') as zip_ref:
        zip_ref.extractall(output_dirpath)


    os.remove(zip_filepath)

    logging.info(f"result was unzipped!")


def persona(input_filepath:Path, output_dirpath:Path):

    logging.info(f"Processing file: {input_filepath} ...")
    
    try:
        main_api_call = call_api(input_filepath, "zip")
        main_api_results = unzip_file(main_api_call)
        logging.info("Done!")

    except requests.exceptions.RequestException as e:
        logging.error(f"Request failed because {e}")
    except ApiError as e:
        logging.error(f"Failed because {e}")

    output_dirpath = Path(output_dirpath)


if __name__ == '__main__':


    parser = argparse.ArgumentParser(description = 'Persona ML api client') # TODO store record.txt
    parser.add_argument(
        '-assets',
        help='path to assets folder'
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
    
    if(args.ip):
        EPHEM_URL = args.ip

    output_dirpath = Path(args.assets)
    persona(args.path, args.assets)