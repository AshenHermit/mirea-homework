import os
import traceback
from lib.docx_renderer import DocxReportRenderer
import argparse
from docx_pdf_converter import convert_docx_to_pdf

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("html_document", default="document.html", help="path to .html document")
    parser.add_argument("output", default="generated_report.docx", help="path to the .docx file to be generated")
    parser.add_argument("-p", "--pdf-output", default="", help="path to the .pdf file to be generated")
    parser.add_argument("-v", "--vars", default="", help="path to optional .html file where variables are defined")
    parser.add_argument("-m", "--magick-convert-cmd", default="convert", help='image magick convert command. default: "convert"')
    args = parser.parse_args()
    
    try:
        renderer = DocxReportRenderer(args.vars, args.magick_convert_cmd)
        renderer.open_html_file(args.html_document)
        renderer.convert_to_docx(args.output)

        if args.pdf_output:
            convert_docx_to_pdf(args.output, args.pdf_output)

    except Exception as e:
        traceback.print_exc()
        os.system('pause')

if __name__ == '__main__':
    main()