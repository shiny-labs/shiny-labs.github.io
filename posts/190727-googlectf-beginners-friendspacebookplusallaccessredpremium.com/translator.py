#!/usr/bin/env python

"""
Translate emojis to kinda assembly
Outputs a file <program>.translated
    Usage: ./translator.py <program_to_translate>

"""

import sys

EMOJIS =  {
      '🍡': "add",
      '🤡': "clone",
      '📐': "divide",
      '😲': "if_zero",
      '😄': "if_not_zero",
      '🏀': "jump_to",
      '🚛': "load",
      '📬': "modulo",
      '⭐': "multiply",
      '🍿': "pop",
      '📤': "pop_out",
      '🎤': "print_top",
      '📥': "push",
      '🔪': "sub",
      '🌓': "xor",
      '⛰': "jump_top",
      '⌛': "exit",
      '0️⃣': "0",
      '1️⃣': "1",
      '2️⃣': "2",
      '3️⃣': "3",
      '4️⃣': "4",
      '5️⃣': "5",
      '6️⃣': "6",
      '7️⃣': "7",
      '8️⃣': "8",
      '9️⃣': "9",
      '✋': "", # end of number
      '🥇': "acc1",
      '🥈': "acc2",
      '😐': "endif",
      '🖋💠🔶🎌🚩🏁': "label A",
      '💰💠🔶🎌🚩🏁': "label A",
      '🖋💠🎌🏁🚩🔶': "label B",
      '💰💠🎌🏁🚩🔶': "label B",
      '🖋💠🏁🎌🔶🚩': "label C",
      '💰💠🏁🎌🔶🚩': "label C",
      '🖋🚩💠🎌🔶🏁': "label D",
      '💰🚩💠🎌🔶🏁': "label D",
      '🖋🚩🔶🏁🎌💠': "label E",
      '💰🚩🔶🏁🎌💠': "label E",
      '🖋🏁🚩🎌💠🔶': "label F",
      '💰🏁🚩🎌💠🔶': "label F",
      '🖋🏁💠🔶🚩🎌': "label G",
      '💰🏁💠🔶🚩🎌': "label G",
      '🖋🔶🎌🚩💠🏁': "label H",
      '💰🔶🎌🚩💠🏁': "label H",
      '🖋🔶🚩💠🏁🎌': "label I",
      '💰🔶🚩💠🏁🎌': "label I",
      '🖋🎌🏁💠🔶🚩': "label J",
      '💰🎌🏁💠🔶🚩': "label J",
      '🖋🎌🏁🚩🔶💠': "label K",
      '💰🎌🏁🚩🔶💠': "label K",
      '🖋🎌🚩💠🔶🏁': "label L",
      '💰🎌🚩💠🔶🏁': "label L",
}

def usage():
    print("[!] missing argument")
    print("Usage: {} <program_to_translate>".format(sys.argv[0]))


if __name__ == '__main__':
    # check that a program was given as argument
    if len(sys.argv) != 2:
        usage()
        raise SystemExit()

    # read the content of program
    content = []
    with open(sys.argv[1], 'r') as f:
        content = f.readlines()

    # start the translation!
    with open("{}.translated".format(sys.argv[1]), "w") as f:
        for line in content:
            line = line.split()
            line_translation = []

            for ins in line:
                if ins is not '':
                    try:
                        line_translation.append(EMOJIS[ins])
                    except KeyError:
                        line_translation.append("{}".format(ins))

            f.write("{}\n".format(" ".join(line_translation)))
