import os
from lib.utils import dir_name_of_file, real_path_of_file, try_turn_string_to_number, text_to_simple_table

class ImageProvider():
    def __init__(self, working_directory:str, image_magick_convert_cmd="convert",
                    delete_processed_files_after_destroy=False, psd_export_extension="jpg") -> None:
        self.working_directory = working_directory
        self.image_magick_convert_cmd = image_magick_convert_cmd
        self.delete_processed_files_after_destroy = delete_processed_files_after_destroy

        self.psd_export_extension = psd_export_extension

        self.file_processors = self._get_file_processors()

    def set_working_directory(self, working_directory):
        self.working_directory = working_directory
        
    def _get_file_processors(self):
        file_processors = {ext: self._ready_file_processor for ext in ["png", "jpg", "jpeg"]}
        file_processors["psd"] = self._psd_image_processor
        return file_processors

    def prepare_image(self, relative_path):
        relative_path = self.working_directory + relative_path
        filepath:str = real_path_of_file(relative_path)
        processed_file_path = self.process_file(filepath)
        return processed_file_path

    def process_file(self, filepath:str)->str:
        if filepath.rfind(".")==-1: raise Exception("file has no extension")
        extension = filepath[filepath.rfind(".")+1:].lower()
        processed_file_path = self.file_processors[extension](filepath)
        return processed_file_path

    def _ready_file_processor(self, filepath):
        return filepath
    
    def _psd_image_processor(self, filepath):
        export_path = filepath[:filepath.rfind(".")+1] + self.psd_export_extension
        cmd = f'{self.image_magick_convert_cmd} "{filepath}[0]" "{export_path}"'
        
        name_of_orig = filepath[filepath.rfind("/")+1:]
        name_of_export = export_path[export_path.rfind("/")+1:]
        print(f'converting psd to {self.psd_export_extension}: \n\t"{name_of_orig}" to {name_of_export}')
        os.system(cmd)
        return export_path