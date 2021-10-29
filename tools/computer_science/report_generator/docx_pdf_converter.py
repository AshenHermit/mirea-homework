from argparse import ArgumentParser
import os

this_file_dir = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")

def convert_docx_to_pdf(docx_filepath, pdf_filepath):
    convert_vbs = this_file_dir+"/res/docx_to_pdf.vbs"
    cmd = f'cscript "{convert_vbs}" "{docx_filepath}" "{pdf_filepath}"'
    print(cmd)
    os.system(cmd)

def main():
    parser = ArgumentParser()
    parser.add_argument("docx_filepath", type=str)
    parser.add_argument("pdf_filepath", type=str)
    parser.add_argument("-w", "--word-executable", type=str, default="C:/Program Files (x86)/Microsoft Office/root/Office16/WINWORD.EXE")
    args = parser.parse_args()

    convert_docx_to_pdf(args.docx_filepath, args.pdf_filepath)
    print("done.")

if __name__ == '__main__':
    main()