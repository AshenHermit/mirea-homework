from pathlib import Path
from tkinter import *
from argparse import ArgumentParser
import os
import sys
import traceback
from functools import partial

from docx_pdf_converter import convert_docx_to_pdf
from lib.docx_renderer import DocxReportRenderer

import re

from lib.utils import dir_name_of_file

def non_digit_char(x):
    return re.search(r"(?!\d)", x).span()[0]
def get_value_at_start(x:str, default=0):
    ns = x[:non_digit_char(x)]
    if ns=="": return default
    return int(ns)

class Config:
    docx_path_format:str = ""
    pdf_path_format:str = ""

    vars_filepath:str = ""
    image_magic_convert_cmd:str = "convert"

class Project:
    def __init__(self, html_path, config:Config) -> None:
        self.html_path = html_path
        self.config = config

        self.html_directory = dir_name_of_file(self.html_path)
        self.html_filename = os.path.split(self.html_path)[-1]
        self.document_name = self.html_filename.split(".")[0]

        self.docx_filename = self.path_with_document_name(config.docx_path_format)
        self.pdf_filename = self.path_with_document_name(config.pdf_path_format)

    def path_with_document_name(self, format_string:str):
        if format_string=="": return ""
        path = self.html_directory+format_string.format(
            self.document_name, 
            str(get_value_at_start(self.document_name)))
        return path
    
    def generate(self):
        if self.docx_filename: 
            self.generate_docx()
            
            if self.pdf_filename:
                self.generate_pdf()
    
    def generate_docx(self):
        Path(dir_name_of_file(self.docx_filename)).mkdir(exist_ok=True)
        renderer = DocxReportRenderer(self.config.vars_filepath, self.config.image_magic_convert_cmd)
        renderer.open_html_file(self.html_path)
        renderer.convert_to_docx(self.docx_filename)

    def generate_pdf(self):
        Path(dir_name_of_file(self.pdf_filename)).mkdir(exist_ok=True)
        convert_docx_to_pdf(self.docx_filename, self.pdf_filename)

class ProjectManager:
    def __init__(self, projects_directory, config:Config) -> None:
        self.projects_directory = projects_directory
        self.config = config
        self.projects:list[Project] = self.gather_projects()

    def sort_files(self, projects):
        def sort_func(x):
            x = os.path.split(x)[-1]
            nx = x[:non_digit_char(x)]
            nx = get_value_at_start(x, sys.maxsize)
            sx = x[non_digit_char(x):]
            return (nx, sx)

        projects = sorted(projects, key=sort_func)
        return projects
        
    def gather_projects(self):
        project_files = []

        def check_folder_for_projects(folder_path):
            for path in os.listdir(folder_path):
                path = folder_path + path
                if os.path.isfile(path):
                    dot_pos = path.rfind(".")
                    if dot_pos == -1: continue
                    extension = path[dot_pos+1:].lower()
                    if extension == "html":
                        path = os.path.realpath(path)
                        project_files.append(path)

        for path in os.listdir(self.projects_directory):
            path = self.projects_directory + path
            if os.path.isdir(path):
                check_folder_for_projects(path+"/")

        project_files = self.sort_files(project_files)
        projects = [Project(proj_file, self.config) for proj_file in project_files]

        return projects            

class ManagerClientGUI:
    def __init__(self, master, projects_directory, config:Config):
        self.master:Tk = master
        self.config = config
        self.manager:ProjectManager = ProjectManager(projects_directory, self.config)
        master.title("Project Manager")

        self.generate_buttons:list[Button] = []
        self.status_labels:list[Label] = []

        self.render_controls()

    def render_controls(self):
        self.master.grid_rowconfigure(5, minsize=16)
        Label(self.master, text=f'projects directory:').grid(row=1, column=0, sticky="w")
        Label(self.master, text=f'"{self.manager.projects_directory}"').grid(row=1, column=1, sticky="w")

        Label(self.master, text=f'vars filepath:').grid(row=2, column=0, sticky="w")
        Label(self.master, text=f'"{self.manager.config.vars_filepath}"').grid(row=2, column=1, sticky="w")

        Label(self.master, text=f'docx path format:').grid(row=3, column=0, sticky="w")
        Label(self.master, text=f'"{self.manager.config.docx_path_format}"').grid(row=3, column=1, sticky="w")

        Label(self.master, text=f'pdf path format:').grid(row=4, column=0, sticky="w")
        Label(self.master, text=f'"{self.manager.config.pdf_path_format}"').grid(row=4, column=1, sticky="w")

        Label(self.master, text=f'generate').grid(row=6, column=0, sticky="w")
        Label(self.master, text=f'status').grid(row=6, column=1, sticky="w")
        row = 7
        for i, project in enumerate(self.manager.projects):
            command = partial(self.generate_project, project, i)
            button = Button(self.master, text=f"{project.document_name}", command=command)
            button.grid(row=row+i, column=0, sticky="w")
            self.generate_buttons.append(button)

            status_label = Label(self.master, text=f'...')
            status_label.grid(row=row+i, column=1, sticky="w")
            self.status_labels.append(status_label)

    def generate_project(self, project:Project, i=0):
        self.status_labels[i].config(text="generating...")
        self.master.update()
        try:
            self.manager.projects[i].generate()
            self.status_labels[i].config(text="ready")
        except Exception as e:
            traceback.print_exc()
            self.status_labels[i].config(text="failed")

    def generate_documents(self):
        print("Greetings!")

    def run(self):
        while True:
            try:
                self.master.update()
            except:
                traceback.print_exc()
                break

def main():
    parser = ArgumentParser()
    parser.add_argument("-d", "--projects-directory", default="./hermits_works/", type=str, help="path to directory containing folders with html files")
    parser.add_argument("-df", "--docx-path-format", default="generated.docx", type=str, help="filepath format-string of docx file")
    parser.add_argument("-pf", "--pdf-path-format", default="private/report {1}.pdf", type=str, help="filepath format-string of pdf file")
    # TODO: vars file also needs to be specified in project's html document
    parser.add_argument("-v", "--vars-filepath", default="", type=str, help="path to html file with external variables")
    args = parser.parse_args()

    config = Config()
    if args.docx_path_format: config.docx_path_format = args.docx_path_format
    if args.pdf_path_format: config.pdf_path_format = args.pdf_path_format
    if args.vars_filepath: config.vars_filepath = args.vars_filepath

    root = Tk()
    client_gui = ManagerClientGUI(root, args.projects_directory, config)
    client_gui.run()

if __name__ == '__main__':
    main()