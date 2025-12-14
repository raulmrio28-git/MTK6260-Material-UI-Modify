#!/usr/bin/python
# ===========================================================================
#
# File: 
#     copy_project.py
#
# Description: 
#     Copy project to new directory
# 
# History: 
#
# when          who             what, where, why
# ----------    ------------    --------------------------------
# 2025-05-24    konakona        Rename file when copy
# 2025-05-23    konakona        Created.
#
# ===========================================================================

import os
import sys
import shutil

if os.name == "nt":
    source_path = os.path.dirname(__file__)
else:
    source_path = os.path.dirname(__file__)

DEST_PROJECT = sys.argv[3]

file_path = [["custom/codegen", "BB"],
             ["custom/system", "BB"],
             ["custom/ps", "BB"],
             ["custom/meta", "BB"],
             ["custom/drv/misc_drv", "BB"],
             ["custom/drv/color", "LCM"],
             ["custom/drv/LCD", "LCM"],
             ["custom/drv/camera", "BB"],
             ["custom/drv/bluetooth", "BB"],
             ["custom/audio", "BB"],
             ["custom/app", "BB"],
             ["lcmmi/Customer/CustResource", "MMI"],
             ["plutommi/Customer/CustResource", "MMI"]]

def modify_file(file_path, string_to_replace, replacement):
    f = open(file_path, "r", encoding='utf-8')
    content = f.read()
    f.close()
    content = content.replace(string_to_replace, replacement)
    f = open(file_path, "w", encoding='utf-8')
    f.write(content)
    f.close()

def rename_items_in_folder(folder_path, original_name, new_name):
    items_to_rename = []
    
    for item_name in os.listdir(folder_path):
        if original_name in item_name:
            old_path = os.path.join(folder_path, item_name)
            new_item_name = item_name.replace(original_name, new_name)
            new_path = os.path.join(folder_path, new_item_name)
            items_to_rename.append((old_path, new_path))
            
    for old_path, new_path in items_to_rename:
        os.rename(old_path, new_path)
        if os.path.isdir(new_path):
            rename_items_in_folder(new_path, original_name, new_name)
    
    for item_name in os.listdir(folder_path):
        item_path = os.path.join(folder_path, item_name)
        if os.path.isdir(item_path) and original_name not in item_name:
            rename_items_in_folder(item_path, original_name, new_name)

def copy_project_file(original_name, new_name):
    for item in file_path:
        try:
            source_file = os.path.join(source_path, item[0]) + f"/{original_name}_{item[1]}"
            destination_file = os.path.join(source_path, item[0]) + f"/{new_name}_{item[1]}"
            if os.path.exists(destination_file):
                    shutil.rmtree(destination_file)
            shutil.copytree(source_file, destination_file)
            rename_items_in_folder(destination_file, original_name, new_name)
        except:
            pass

if __name__ == "__main__":
    src_project = sys.argv[1]
    dst_project = DEST_PROJECT
    type_project = sys.argv[2]

    copy_project_file(src_project, dst_project)
    old_mak = os.path.join(os.path.join(source_path, "make"), f"{src_project}_{type_project}.mak")
    new_mak = os.path.join(os.path.join(source_path, "make"), f"{dst_project}_{type_project}.mak")
    shutil.copy(old_mak, new_mak)
    modify_file(new_mak, src_project, dst_project)

    old_mak = os.path.join(os.path.join(source_path, "make"), f"Verno_{src_project}.bld")
    new_mak = os.path.join(os.path.join(source_path, "make"), f"Verno_{dst_project}.bld")
    shutil.copy(old_mak, new_mak)
    modify_file(new_mak, src_project, dst_project)