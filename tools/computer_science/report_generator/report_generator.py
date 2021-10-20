from lib.docx_renderer import DocxReportRenderer
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("html_document", default="document.html", help="path to .html document")
    parser.add_argument("output", default="generated_report.docx", help="path to the .docx file to be generated")
    parser.add_argument("--title-page", default="res/title_page.docx", help="path to title page .docx file")
    args = parser.parse_args()
    
    converter = DocxReportRenderer(args.title_page)
    converter.open_html_file(args.html_document)
    converter.convert_to_docx(args.output)

if __name__ == '__main__':
    main()