# old solution. not in use now.

from argparse import ArgumentParser
from ahk import AHK
import time
import subprocess

import os

from ahk.utils import make_logger
logger = make_logger(__name__)

def wait_for_normal_cursor(ahk:AHK):
    ahk.run_script("""Loop
{
  Sleep, 1000
  If (A_Cursor <> Wait)
    Break
}""")

def arg_decode(text:str):
    return text.encode('cp866').decode('utf-8')

def ahk_decode(text:str):
    return text.encode("utf-8").decode("cp1251")

def run_ahk_script(ahk_executable, script_text, encoding="cp1251", **kwargs):
    runargs = [ahk_executable, '/ErrorStdOut', '*']
    script_bytes = script_text.encode(encoding)

    result = subprocess.run(runargs, input=script_bytes,
                            stderr=subprocess.PIPE, stdout=subprocess.PIPE, **kwargs)
    logger.debug('Stdout: %s', repr(result.stdout))
    logger.debug('Stderr: %s', repr(result.stderr))
    return result.stdout.decode()

def make_pdf(ahk_executable, word_executable, docx_filepath, pdf_filepath):
    print(pdf_filepath)
    ahk = AHK(executable_path=ahk_executable)
    word_executable_name = word_executable[word_executable.rfind("/")+1:word_executable.rfind(".")]
    ahk.run_script(f'Run "{word_executable}" "{docx_filepath}"')
    ahk.run_script(f'WinActivate {word_executable_name}')
    ahk.run_script(f'WinWait ahk_class OpusApp')
    win = ahk.active_window

    wait_time = 0.2

    ahk.click(x=1319, y=355)
    time.sleep(wait_time)
    ahk.send("^a")
    time.sleep(wait_time)

    ahk.click(x=900, y=47)
    time.sleep(wait_time)
    ahk.click(x=303, y=86)
    wait_for_normal_cursor(ahk)
    time.sleep(0.5)

    ##### update fields
    # ahk.mouse_move(x=1912, y=206)
    # time.sleep(wait_time)
    # ahk.mouse_drag(x=1912, y=985, speed=1)
    # time.sleep(wait_time)
    # ahk.mouse_drag(x=1912, y=206, speed=1)
    # time.sleep(wait_time)

    # ahk.send("^a")
    # time.sleep(wait_time)

    # ahk.right_click(x=1703, y=404)
    # time.sleep(wait_time)
    # ahk.click(x=ahk.mouse_position[0]+88, y=ahk.mouse_position[1]+118)
    # time.sleep(wait_time)
    #####

    ahk.click(27, 50)
    time.sleep(wait_time)
    ahk.click(66, 479)
    time.sleep(wait_time)
    ahk.click(571, 256)
    time.sleep(wait_time)
    wait_for_normal_cursor(ahk)
    time.sleep(wait_time)

    run_ahk_script(ahk_executable, f"SendInput {os.path.realpath(pdf_filepath)}")

    time.sleep(wait_time)
    ahk.key_press("Enter")
    time.sleep(wait_time)


# C:/Program Files (x86)/Microsoft Office/root/Office16/WINWORD.EXE

def main():
    parser = ArgumentParser()
    parser.add_argument("docx_filepath", type=str)
    parser.add_argument("pdf_filepath", type=str)
    parser.add_argument("-w", "--word-executable", type=str, default="C:/Program Files (x86)/Microsoft Office/root/Office16/WINWORD.EXE")
    parser.add_argument("-a", "--ahk-executable", type=str, default="C:/Program Files/AutoHotkey/AutoHotkeyU64.exe")
    args = parser.parse_args()

    make_pdf(args.ahk_executable, args.word_executable, args.docx_filepath, args.pdf_filepath)
    print("done.")

if __name__ == '__main__':
    main()