from bs4.element import NavigableString, Tag
import docx
import docx.document
from docx.enum.text import WD_ALIGN_PARAGRAPH, WD_BREAK
from docx.enum.table import WD_CELL_VERTICAL_ALIGNMENT, WD_TABLE_ALIGNMENT, WD_ALIGN_VERTICAL
import docx.text
import docx.text.paragraph
import docx.text.run
from docx import Document
from docx.shared import Cm, Emu, Pt
from lxml import etree
from docx.enum.dml import MSO_THEME_COLOR_INDEX

from bs4 import BeautifulSoup
from docx.shared import Pt
import re
import markdown

from lib.utils import dir_name_of_file, real_path_of_file, text_to_simple_table
from lib.latex_utils import LatexExtender, latex_to_mathml

from pathlib import Path
import os
import traceback
dir_path = dir_name_of_file(__file__)
MML2OMMLXML_PATH = dir_path+"../res/mml2omml.xsl"

class DocumentConfig:
    font_name:str = 'Times New Roman'
    chapter_title_alignment:str = 'center'
    paragraph_alignment:str = 'justify'
    picture_alignment:str = 'center'
    table_alignment:str = 'center'
    font_size:int = 14
    caption_font_size:int = 12
    table_of_contents_title:str = "Содержание"
    table_of_contents_title_alignment:str = "center"
    line_spacing:float = 1.5
    paragraph_first_line_indent:Cm = Cm(1.25)
    paragraph_left_indent:Cm = Cm(0.0)
    chapter_title_format:str = "{0}  {1} "
    single_formula_alignment:str = "right"


class DocxReportRenderer:
    def __init__(self, title_page_filepath="title_page.docx") -> None:
        self.title_page_filepath = title_page_filepath
        self.html_file_directory = ""
        self.soup: BeautifulSoup = None
        self.document: docx.document.Document = self.create_new_document()
        self.document_config = DocumentConfig()

        self.xml_namespaces = self.get_xml_namespaces()

    def get_xml_namespaces(self):
        return 'xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" '\
            + 'xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math"'

    def create_new_document(self):
        if self.title_page_filepath == "":
            return docx.Document()
        else:
            doc:docx.document.Document = docx.Document(self.title_page_filepath)
            doc.add_page_break()
            return doc
        
    def open_html_file(self, filepath):
        file = Path(filepath)
        if file.exists():
            self.load_html(file.read_text(encoding='utf-8'))
            self.html_file_directory = dir_name_of_file(filepath)
        else:
            self.load_html("")
            raise FileNotFoundError()
            
    def load_html(self, html=""):
        self.html = html
        self.html = self.html.replace("\n", "")
        self.soup = BeautifulSoup(self.html, 'html.parser')

    def latex_to_word(self, latex_input, font_size=14, label=""):
        font_size = font_size*2

        mathml = latex_to_mathml(latex_input, label)
        tree = etree.fromstring(mathml)
        xslt = etree.parse(MML2OMMLXML_PATH)
        transform = etree.XSLT(xslt)
        new_dom = transform(tree)
        root = new_dom.getroot()

        prefix_map = {"m": "http://schemas.openxmlformats.org/officeDocument/2006/math"}

        for run in root.findall('.//m:sSub', prefix_map):
            params = etree.fromstring(f"""
                            <m:sSubPr {self.xml_namespaces}>
                                <m:ctrlPr>
                                    <w:rPr>
                                        <w:rFonts w:ascii="Cambria Math" w:hAnsi="Cambria Math"/>
                                        <w:sz w:val="{font_size}"/>
                                        <w:szCs w:val="{font_size}"/>
                                    </w:rPr>
                                </m:ctrlPr>
                            </m:sSubPr>""")
            run.insert(0, params)
        
        for run in root.findall('.//m:r', prefix_map):
            params = etree.fromstring(f"""
                            <w:rPr {self.xml_namespaces}>
								<w:rFonts w:ascii="Cambria Math" w:hAnsi="Cambria Math"/>
								<w:sz w:val="{font_size}"/>
								<w:szCs w:val="{font_size}"/>
							</w:rPr>""")
            run.insert(0, params)
        
        return root
    
    def write_title_page(self):
        title_page = etree.parse(self.title_page_filepath)
        for el in title_page.findall("./"):
            self.document._body._element.append(el)
        self.document.add_page_break()

    def make_paragraph(self, document:docx.document.Document, align="left"):
        pg = document.add_paragraph()
        pg.paragraph_format.left_indent = self.document_config.paragraph_left_indent
        pg.paragraph_format.first_line_indent = self.document_config.paragraph_first_line_indent
        pg.paragraph_format.line_spacing = self.document_config.line_spacing
        pg.alignment = self.get_paragraph_alignment(align)
        if align=="center":
            pg.paragraph_format.first_line_indent = pg.paragraph_format.left_indent
            pg.paragraph_format.alignment = self.get_paragraph_alignment("center")
        return pg

    def make_paragraph_text_run(self, paragraph:docx.text.paragraph.Paragraph, text:str, font_size=14):
        run = paragraph.add_run(text)
        run.font.size = Pt(font_size)
        run.font.name = self.document_config.font_name
        return run

    def apply_tag_style_to_run(self, run:docx.text.run.Run, tag:Tag):
        if tag.name == "b":
            run.bold = True
        if tag.name == "i":
            run.italic = True

    def add_formula_tag_to_paragraph(self, paragraph:docx.text.paragraph.Paragraph, f_tag:Tag, font_size=14):
        latex_text = LatexExtender(f_tag.get("flags", "")).extend_latex(f_tag.text)
        label = f_tag.get("label", "")
        word_math = self.latex_to_word(latex_text, font_size, label)
        paragraph._element.append(word_math)

    def process_paragraph_child(self, paragraph:docx.text.paragraph.Paragraph, tag:Tag, font_size=14, index=1):
        if type(tag) is NavigableString or type(tag) is str:
            # just a text
            if type(tag) is NavigableString: 
                if index==0: text = tag.lstrip()
                else: text = str(tag)
            if type(tag) is str: text = tag
            run = self.make_paragraph_text_run(paragraph, text, font_size)
        elif type(tag) is Tag:
            # <f> tags
            if tag.name=="f":
                self.add_formula_tag_to_paragraph(paragraph, tag, font_size)
            # <f> tags
            elif tag.name=="a":
                self.add_hyperlink(paragraph, tag.text, tag.get("href", ""))
            # <b>, <i>... tags
            else:
                text = tag.text
                run = self.make_paragraph_text_run(paragraph, text, font_size)
                self.apply_tag_style_to_run(run, tag)

    def render_tag_to_paragraph(self, paragraph:docx.text.paragraph.Paragraph, tag:Tag):
        font_size = int(tag.get("font-size", str(self.document_config.font_size)))

        for i,child in enumerate(tag.children):
            self.process_paragraph_child(paragraph, child, index=i)

    def get_paragraph_alignment(self, value=None):
        enum = {"left": WD_ALIGN_PARAGRAPH.LEFT, "center": WD_ALIGN_PARAGRAPH.CENTER, "right": WD_ALIGN_PARAGRAPH.RIGHT,
                "justify": WD_ALIGN_PARAGRAPH.JUSTIFY, "distribute": WD_ALIGN_PARAGRAPH.DISTRIBUTE}
        if value in enum:
            return enum[value]
        return enum[self.document_config.paragraph_alignment]

    def add_paragraph_tag(self, pg_tag:Tag):
        align = pg_tag.get("align", self.document_config.paragraph_alignment)
        children = list(pg_tag.children)
        if len(children)==1:
            if children[0].name == "f":
                align = self.document_config.single_formula_alignment
        
        pg = self.make_paragraph(self.document, align)
        self.render_tag_to_paragraph(pg, pg_tag)
        return pg

    def add_picture(self, img_tag:Tag):
        pg = self.make_paragraph(self.document, img_tag.get("align", self.document_config.picture_alignment))

        run = pg.add_run()

        width = img_tag.get("width", None)
        if not width is None: width = Cm(int(width))
        height = img_tag.get("height", None)
        if not height is None: height = Cm(int(height))

        run.add_picture(self.html_file_directory + img_tag.get("src", ""), width=width, height=height)
        caption = img_tag.get("caption", "")
        if caption!="":
            run.add_break(WD_BREAK.LINE)
            run.add_text(caption)
            run.font.size = Pt(self.document_config.caption_font_size)
        
        self.make_paragraph(self.document)
        return pg

    def delete_paragraph(self, paragraph:docx.text.paragraph.Paragraph):
        pg = paragraph._element
        pg.getparent().remove(pg)
        pg._p = pg._element = None

    def add_table(self, table_tag:Tag):
        # caption
        self.make_paragraph(self.document)
        caption = table_tag.get("caption", "")
        if caption!="":
            pg = self.make_paragraph(self.document, "center")
            run = pg.add_run(caption)
            run.font.size = Pt(self.document_config.caption_font_size)

        table_data = text_to_simple_table(table_tag.renderContents().decode("utf-8"))
        cell_width = int(table_tag.get("cell-width", "10"))
        cell_height = int(table_tag.get("cell-height", "10"))

        #TODO: add size checking. actually better to do a class for table
        table = self.document.add_table(len(table_data), len(table_data[0]))
        table.alignment = WD_TABLE_ALIGNMENT.CENTER
        table.style = 'Table Grid'
        for r,row in enumerate(table.rows):
            row.height = Pt(cell_height)
            for c,cell in enumerate(row.cells):
                cell.width = Pt(cell_width)
                cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.CENTER
                self.delete_paragraph(cell.paragraphs[0])

                pg = self.make_paragraph(cell, "center")
                tag = BeautifulSoup(table_data[r][c], "html.parser")
                children = list(tag.children)
                if len(children)>0:
                    tag = children[0]
                    self.process_paragraph_child(pg, tag, self.document_config.font_size)

    def add_hyperlink(self, paragraph, text, url):
        # took from here https://github.com/python-openxml/python-docx/issues/384#issuecomment-294853130
        # This gets access to the document.xml.rels file and gets a new relation id value
        part = paragraph.part
        r_id = part.relate_to(url, docx.opc.constants.RELATIONSHIP_TYPE.HYPERLINK, is_external=True)

        # Create the w:hyperlink tag and add needed values
        hyperlink = docx.oxml.shared.OxmlElement('w:hyperlink')
        hyperlink.set(docx.oxml.shared.qn('r:id'), r_id, )

        # Create a w:r element and a new w:rPr element
        new_run = docx.oxml.shared.OxmlElement('w:r')
        rPr = docx.oxml.shared.OxmlElement('w:rPr')

        # Join all the xml elements together add add the required text to the w:r element
        new_run.append(rPr)
        new_run.text = text
        hyperlink.append(new_run)

        # Create a new Run object and add the hyperlink into it
        r = paragraph.add_run ()
        r._r.append (hyperlink)

        # A workaround for the lack of a hyperlink style (doesn't go purple after using the link)
        # Delete this if using a template that has the hyperlink style in it
        r.font.color.theme_color = MSO_THEME_COLOR_INDEX.HYPERLINK
        r.font.underline = True

        return hyperlink
        
    def add_chapter(self, ch_el:Tag, index=0):
        # title
        pg = self.make_paragraph(self.document, self.document_config.chapter_title_alignment)
        title_text = self.document_config.chapter_title_format.format(index+1, ch_el["title"].upper())
        title = self.make_paragraph_text_run(pg, title_text)
        title.bold = True
        
        paragraphs = ch_el.findChildren()
        for tag in paragraphs:
            # <p> tags
            if tag.name == "p":
                self.add_paragraph_tag(tag)
            # <img> tags
            elif tag.name == "img":
                self.add_picture(tag)
            # <table> tags
            elif tag.name == "table":
                self.add_table(tag)
        self.make_paragraph(self.document)

    def add_ptab(self, paragraph:docx.text.paragraph.Paragraph):
        run = paragraph.add_run()
        ptab_el = etree.fromstring(
            f'<w:ptab {self.xml_namespaces} w:alignment="right" w:leader="dot" w:relativeTo="margin"/>')
        run._element.append(ptab_el)
        return run

    def add_table_of_contents(self, chapter_names):
        # title
        pg = self.make_paragraph(self.document, self.document_config.table_of_contents_title_alignment)
        title = self.make_paragraph_text_run(pg, self.document_config.table_of_contents_title.upper())
        title.bold = True

        # list
        pg = self.make_paragraph(self.document)
        pg.paragraph_format.first_line_indent = self.document_config.paragraph_left_indent

        for i,name in enumerate(chapter_names):
            line = self.document_config.chapter_title_format.format(i+1, name.upper())
            pg.add_run(line)
            self.add_ptab(pg)
            pg.add_run().add_break(WD_BREAK.LINE)
        
        self.document.add_page_break()

    def docx_replace_regex(self, doc, regex, replace):
        for p in doc.paragraphs:
            if regex.search(p.text):
                inline = p.runs
                # Loop added to work with runs (strings with same style)
                for i in range(len(inline)):
                    if regex.search(inline[i].text):
                        text = regex.sub(replace, inline[i].text)
                        inline[i].text = text

        for table in doc.tables:
            for row in table.rows:
                for cell in row.cells:
                    self.docx_replace_regex(cell, regex, replace)

    def replace_patterns_in_document(self, patterns:dict):
        for key in list(patterns.keys()):
            if not type(key) is str: continue
            
            regex = re.compile(r"attr_" + key)
            replace = patterns[key]
            self.docx_replace_regex(self.document, regex, replace)
        
    def create_docx_from_html(self):
        # <report> tag
        report_tag = self.soup.find("report")
        self.replace_patterns_in_document(report_tag.attrs)
        
        chapters:list[Tag] = report_tag.findChildren("chapter")
        self.chapter_names = [ch_el.get("title", "...") for ch_el in chapters]
        self.add_table_of_contents(self.chapter_names)

        children:list[Tag] = report_tag.findChildren()

        for ch_el in children:
            # <chapter> tags
            if ch_el.name == "chapter":
                self.add_chapter(ch_el, chapters.index(ch_el))
            # <pbr> tags
            if ch_el.name == "pbr":
                self.document.add_page_break()
        
        return self.document

    def convert_to_docx(self, output_filepath=""):
        try:
            if output_filepath=="": raise Exception("filepath cant be empty")
            document = self.create_docx_from_html()
            document.save(output_filepath)
            print()
            print("document successfully generated, see file")
            print(real_path_of_file(output_filepath))

        except Exception as e:
            traceback.print_exc()
            print("something went wrong")
        
