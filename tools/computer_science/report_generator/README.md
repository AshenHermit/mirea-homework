# Docx Report Generator

## Что делает
Копирует документ с титульным листом `res/title_page.docx` и добавляет в него отформатированное содержимое упрощенного html файла (подобного этому [report/document.html](report/document.html)). Сгенерированный docx документ содержит то форматирование, которое требуется соблюдать в отчете.
Упрощен ввод [формул](#формулы), формулы можно записывать в формате [latex](https://ru.overleaf.com/learn/latex/Mathematical_expressions), который тоже немного упрощен.

(скрины старые)
document.html                  |  generated.docx
:-----------------------------:|:-------------------------:
![](res/readme/screenshot_1.jpg) | ![](res/readme/screenshot_2.jpg)

## Возможности
* Автоматическое заполнение полей титульного листа.
* Автоматическое создание блока содержания с перекрестными ссылками на страницы разделов (открыв сгенерированный документ в ворде их можно обновить так: `Ctrl+A > ПКМ > обновить поле`)
* Простая запись [формул](#формулы).
* Запись простейших [таблиц](#таблицы).
* Добавление [изображений](#изображения) с диска. поддерживаются форматы: `.png` `.jpg` `.psd`
* Использование [переменных](#переменные).
* Быстрое создание pdf.
* Есть небольшой менеджер проектов.

## Использование
### Требования
* `python 3.8+`.
* (опционально) [ImageMagick](https://imagemagick.org/index.php), для конвертации psd в png и jpg.
### Установка необходимых модулей
```
> pip install -r requirements.txt
```
### CLI
**report_generator.py**
```
> python report_generator.py --help
usage: report_generator.py [-h] [-p PDF_OUTPUT] [-v VARS] [-m MAGICK_CONVERT_CMD] html_document output

positional arguments:
  html_document         path to .html document
  output                path to the .docx file to be generated

optional arguments:
  -h, --help            show this help message and exit
  -p PDF_OUTPUT, --pdf-output PDF_OUTPUT
                        path to the .pdf file to be generated
  -v VARS, --vars VARS  path to optional .html file where variables are defined
  -m MAGICK_CONVERT_CMD, --magick-convert-cmd MAGICK_CONVERT_CMD
                        image magick convert command. default: "convert"
```
Пример
```
> python report_generator.py "report/document.html" "report/generated.docx"
```

**docx_pdf_converter.py**
```
> python docx_pdf_converter.py --help
usage: docx_pdf_converter.py [-h] [-w WORD_EXECUTABLE] docx_filepath pdf_filepath

positional arguments:
  docx_filepath
  pdf_filepath

optional arguments:
  -h, --help            show this help message and exit
  -w WORD_EXECUTABLE, --word-executable WORD_EXECUTABLE
```

**project_manager.py**
```
> python project_manager.py --help
usage: project_manager.py [-h] [-d PROJECTS_DIRECTORY] [-df DOCX_PATH_FORMAT] [-pf PDF_PATH_FORMAT] [-v VARS_FILEPATH]

optional arguments:
  -h, --help            show this help message and exit
  -d PROJECTS_DIRECTORY, --projects-directory PROJECTS_DIRECTORY
                        path to directory containing folders with html files
  -df DOCX_PATH_FORMAT, --docx-path-format DOCX_PATH_FORMAT
                        filepath format-string of docx file
  -pf PDF_PATH_FORMAT, --pdf-path-format PDF_PATH_FORMAT
                        filepath format-string of pdf file
  -v VARS_FILEPATH, --vars-filepath VARS_FILEPATH
                        path to html file with external variables
```

## Элементы html документа
### Структура
* `<report title_page="../res/title_page.docx" author="студент" group_name="инбо..." title="заголовок" recipient_status="че за препод" recipient="препод">` - корень документа.  
`title_page` - путь к титульному листу, относительно html документа.  
Технически, любые атрибуты в этом теге используются для замены полей в титульном листе, соответсвующих такому формату: "attr_<имя_атрибута>".
* `<chapter title="Раздел первый">` - раздел.
* `<p>` - параграф. `<p align="center">` - расположен в центре.
* `<pbr/>` - разрыв страницы.
### Текст
* `<a href="https://"></a>` - ссылка.
* `<b>` - жирный текст.
* `<i>` - курсив.
### Формулы
* `<f flags="cs" label="1">` - формула в формате [latex](https://ru.overleaf.com/learn/latex/Mathematical_expressions).  
`label` - номер формулы, или текст который должен быть в круглых скобках справа от формулы.  
Флаг `cs` (*`computer science`*) отвечает за некоторые дополнительные функции:  
`!a` или `!(a+b...)` - отрицание, превращается в выражение с чертой сверху.  
`v` - символ дизъюнкции. `^` - символ конъюнкции.  
(Для разрыва формул лучше использовать несколько параграфов, как [тут](report/document.html))
### Изображения
* `<img src="image1.jpg" caption="Рисунок">` - изображение.  
`src` принимает путь к локальному файлу, относительно html документа. Поддерживаются форматы: `.png` `.jpg` `.psd`  
`caption` - подпись.
`width` - ширина в сантиметрах.
`height` - высота в сантиметрах.
### Таблицы
* `<table caption="Таблица 1" parse="simple" cell-width="32" cell-height="2" width="4">` - таблица.  
`caption` - подпись.  
`parse="simple"` - режим чтения упрощенной записи таблицы.  
`width` - количество столбцов, можно не указывать, но тогда будут плохо работать сложные элементы внутри ячеек.  
`cell-width` - ширина ячейки в пикселях. (необязательный атрибут)  
`cell-height` - высота ячейки в пикселях.  (необязательный атрибут)  
Элементы таблицы разделяются пробелами, табуляцией, разрывами строк. Если нужно использовать пробелы в ячейке, можно записать текст в тег `<t>`.  
Пример таблицы можно посмотреть [здесь](report/document.html)
### Переменные
* `<var name="имя_переменной">value</var>` - переменная.  
`name` - имя переменной.  
*value* - значение. Может быть текстом, числом.  
Ссылки на переменные, в любом месте документа, будут заменены на их значения, они записываются так: `$имя_переменной`  
Переменные также могут быть записаны в отдельном .html документе ([пример](hermit_works/vars.html)), который можно указать при запуске программы.
Пример: `<img src="karno_map_2_1.jpg" caption="..." width="$karno_map_image_width"/>`