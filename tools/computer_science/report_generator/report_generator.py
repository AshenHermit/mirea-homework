from lib.docx_renderer import DocxReportRenderer
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("html_document", default="document.html", help="path to .html document")
    parser.add_argument("output", default="generated_report.docx", help="path to the .docx file to be generated")
    parser.add_argument("--title-page", default="res/title_page.docx", help='path to title page .docx file. default: "res/title_page.docx"')
    parser.add_argument("--vars", default="", help="path to optional .html file where variables are defined")
    parser.add_argument("--magick-convert-cmd", default="convert", help='image magick convert command. default: "convert"')
    args = parser.parse_args()
    
    renderer = DocxReportRenderer(args.title_page, args.vars, args.magick_convert_cmd)
    renderer.open_html_file(args.html_document)
    renderer.convert_to_docx(args.output)

if __name__ == '__main__':
    main()