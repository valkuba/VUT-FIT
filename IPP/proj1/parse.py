# Skript parse.py
# Implementace 1.úlohy do IPP 2023/24
# Autor: Jakub Valeš
# Datum: 12.3.2024

import sys
import re


class Stats:
    """Dataclass for variables calculating stats of program in IPPcode24"""
    comments: int = 0
    loc: int = 0
    jumps: int = 0
    back_jmp: int = 0
    labels = []
    un_jmp = []                 # list for jump labels that havent been defined in the moment of jump
    freq = {}


def my_argparse():
    """Function for parsing arguments of program
       Returns list of parsed arguments when the STATP extension argument are used
       otherwise an empty list is returned"""
    help = ("A filter type script (parse.py in Python3.10) reads the source code in IPPcode24 from standard input,\n"
            "checks the lexical and syntactic correctness of the code and writes to the standard XML output representation\n"
            "of the program according to the specification.\n"
            "Script supports the STATP extensions with its following arguments:\n"
            "'--comments', '--labels', '--jumps', '--fwjumps', '--backjumps', '--badjumps', '--frequent', '--loc', '--eol'\n"
            "'--stats=file', '--print=string'\n"
            "-----------------------------------------------------------------------\n"
            "  21 - Missing or incorrectly typed header of IPPcode24\n"
            "  22 - Unknown or incorrectly entered instruction code of IPPcode24\n"
            "  23 - Other lexical or syntactic error in the IPPcode24 program.\n")
    valid_args = ['--comments', '--labels', '--jumps', '--fwjumps', '--backjumps', '--badjumps', '--frequent', '--loc', '--eol']
    parsed = []
    files = []  # varialbe to check if entered file was entered more times

    if len(sys.argv) > 1:
        argumentList = sys.argv[1:]
        if '--help' in argumentList:                # Help argument
            if len(argumentList) == 1:
                print(help)
                sys.exit(0)
            else:
                print('Wrong type of argument, try: --help', file=sys.stderr)
                sys.exit(10)

        # Arguments from STATP are entered, the first one must be --stats=file
        if not (file := re.findall(r'^(?:--stats=(?:\")?([^"]*)(?:\")?)', argumentList[0])):    # file neni prvni
            print('Wrong type of argument, try: --help', file=sys.stderr)
            sys.exit(10)
        else:
            files.append(file[0])
            parsed.append(argumentList[0])
            cur_pos = 1                                               # variable to check which arguments are to which file
            for idx, arg in enumerate(argumentList[1:]):
                if (nxt_file := re.findall(r'^(?:--stats=(?:\")?([^"]*)(?:\")?)', arg)):
                    cur_pos = idx + 2
                    if nxt_file[0] in files:
                        print('2x entered same file for stats', file=sys.stderr)
                        sys.exit(12)
                    else:
                        files.append(nxt_file[0])
                        parsed.append(arg)
                else:
                    if re.findall(r'^(?:--print=(.*))', arg):               # print argument
                        parsed.append(arg)
                    else:
                        if arg in valid_args:                          # check if the given argument is one the supported ones
                            if arg not in parsed[cur_pos:] or arg == '--eol':
                                parsed.append(arg)
                            else:
                                print('Wrong type of argument, try: --help', file=sys.stderr)
                                sys.exit(10)
                        else:
                            print('Wrong type of argument, try: --help', file=sys.stderr)
                            sys.exit(10)
    return parsed


def jump_check(stats, jmp_lbl):
    """Function for extenstion STATP to check if the Label that JUMP instruction is using
       is jumping backwards or not
       Returns 1 if JUMP is jumping backwards"""
    if jmp_lbl in stats.labels:
        return 1
    else:
        stats.un_jmp.append(jmp_lbl)
    return 0


def edit_for_xml(line):
    line = line.replace('&', '&amp;')
    line = line.replace('<', '&lt;')
    line = line.replace('>', '&gt;')
    return line


"""For the following functions:
   instr = current instruction, arg_str = type of argument, argnum = number of argument for given instruction,
   written = flag which arranges that <instr... > is printed only once for instruction, counter = instruction counter"""


def check_variable(instr, arg_str, written, counter, argnum):
    """Function checks if the given argument of instruction is valid variable
       and prints XML-format of the argument"""
    if re.fullmatch(r'^(LF|GF|TF)@[a-zA-Z_\-$&%\*!?][a-zA-Z0-9_\-$&%\*!?]*', arg_str):
        if not written:
            print(f'\t<instruction order=\"{counter}\" opcode=\"{instr}\">')
            written = True
        arg_str = edit_for_xml(arg_str)
        print(f'\t\t<arg{argnum} type=\"var\">{arg_str}</arg{argnum}>')
        argnum += 1
    else:
        print('Wrong format of variable/symbol/label/type', file=sys.stderr)
        sys.exit(23)
    return written, argnum


def check_label(instr, arg_str, written, counter, argnum):
    """Function checks if the given argument of instruction is valid label
       and prints XML-format of the argument"""
    if re.fullmatch(r'^[a-zA-Z_\-$&%\*!?][a-zA-Z0-9_\-$&%\*!?]*', arg_str):
        if not written:
            print(f'\t<instruction order=\"{counter}\" opcode=\"{instr}\">')
            written = True
        arg_str = edit_for_xml(arg_str)
        print(f'\t\t<arg{argnum} type=\"label\">{arg_str}</arg{argnum}>')
        argnum += 1
    else:
        print('Wrong format of variable/symbol/label/type', file=sys.stderr)
        sys.exit(23)
    return written, argnum


def check_type(instr, arg_str, written, counter, argnum):
    """Function checks if the given argument of instruction is valid type
       and prints XML-format of the argument"""
    if re.fullmatch(r'(string|bool|int)', arg_str):
        if not written:
            print(f'\t<instruction order=\"{counter}\" opcode=\"{instr}\">')
            written = True
        print(f'\t\t<arg{argnum} type=\"type\">{arg_str}</arg{argnum}>')
        argnum += 1
    else:
        print('Wrong format of variable/symbol/label/type', file=sys.stderr)
        sys.exit(23)
    return written, argnum


def check_symbol(instr, arg_str, written, counter, argnum):
    """Function checks if the given argument of instruction is valid symbol (or variable)
       and prints XML-format of the argument"""
    symbol = arg_str.partition('@')         # Cuts the string into part before @ and after @ which is including it
    sym_type = symbol[0]
    sym_value = symbol[1] + symbol[2]

    match sym_type:
        # Decimal, Octal or Hexa number
        case 'int':
            if not re.fullmatch(r'^@[-+]?([0-9][0-9]*(_[0-9]+)*|0[xX][0-9a-fA-F]+(_[0-9a-fA-F]+)*|0[Oo][0-7]+(_[0-7]+)*)', sym_value):
                print('Wrong format of variable/symbol/label/type', file=sys.stderr)
                sys.exit(23)
        # String which can include some escape sequences
        case 'string':
            if re.match(r'^@', sym_value):
                a = re.findall(r'\\(\d{0,3})', sym_value)
                if a:                                       # string includes escape sequence
                    for escape in a:
                        if not re.fullmatch(r'0([0-2][0-9]|3[0-2]|35|92)', escape):         # valid esc seq
                            print('Wrong format of variable/symbol/label/type', file=sys.stderr)
                            sys.exit(23)
            else:
                print('Wrong format of variable/symbol/label/type', file=sys.stderr)
                sys.exit(23)
        case 'bool':
            if not (sym_value == '@true' or sym_value == '@false'):
                print('Wrong format of variable/symbol/label/type', file=sys.stderr)
                sys.exit(23)
        case 'nil':
            if not sym_value == '@nil':
                print('Wrong format of variable/symbol/label/type', file=sys.stderr)
                sys.exit(23)
        # If all of the above fails, given argument can still be a variable
        case _:
            written, argnum = check_variable(instr, arg_str, written, counter, argnum)
            return written, argnum

    if not written:
        print(f'\t<instruction order=\"{counter}\" opcode=\"{instr}\">')
        written = True

    sym_value = sym_value.replace('@', '', 1)
    sym_value = edit_for_xml(sym_value)
    print(f'\t\t<arg{argnum} type=\"{sym_type}\">{sym_value}</arg{argnum}>')
    argnum += 1

    return written, argnum


def stats_print(args, stats):
    """Functions which prints statistics of the IPPcode24 program if arguments are given
       otherwise function does nothing
       args = list of parsed arguments, stats = dataclass of stats variables"""
    # check if STATP arguments are given
    if args:
        file = args[0]
        fw_jmp = 0                  # number of forward jumps
        # Calculate number of bad and backwards jump
        for lbl in set(stats.labels):
            fw_jmp += stats.un_jmp.count(lbl)
            # if jump label was found remove all of that labels from un_jmp list
            stats.un_jmp = [i for i in stats.un_jmp if i != lbl]
            # now un_jmp list contains jump labels that havent been defined and var fw_jmp containst number of forward jumps

        f = None  # Current file
        for arg in args:
            # Opens file and checks if its opened correctly
            if (file := re.findall(r'^(?:--stats=(?:\")?([^"]*)(?:\")?)', arg)):
                if f:
                    f.close()
                try:
                    f = open(file[0], 'w')
                except OSError:
                    print('Error while opening the file', file=sys.stderr)
                    sys.exit(10)
            # Prints stats according to STATP arguments
            else:
                if f:
                    match arg:
                        case '--comments':
                            f.write(str(stats.comments)+'\n')
                        case '--labels':
                            f.write(str(len(set(stats.labels)))+'\n')
                        case '--jumps':
                            f.write(str(stats.jumps)+'\n')
                        case '--fwjumps':
                            f.write(str(fw_jmp)+'\n')
                        case '--backjumps':
                            f.write(str(stats.back_jmp)+'\n')
                        case '--badjumps':
                            f.write(str(len(stats.un_jmp))+'\n')
                        case '--frequent':
                            max_val = max(stats.freq.values())
                            items = [key for key, value in stats.freq.items() if value == max_val]
                            for item in items:
                                f.write(item.upper())
                                if item != items[len(items)-1]:
                                    f.write(',')
                            f.write('\n')
                        case '--loc':
                            f.write(str(stats.loc)+'\n')
                        case '--eol':
                            f.write('\n')
                        case _:
                            # Parse string from the --print=string argument and prints it to the file
                            if (my_str := re.findall(r'^(?:--print=(.*))', arg)):
                                f.write(str(my_str[0])+'\n')
            # if the last argument is processed close the opened file
            if arg == args[len(args)-1]:
                f.close()
    return


def read_line():
    header = False
    counter = 1
    stats = Stats()
    # Going through all lines
    for line in sys.stdin:
        # Deleting (and calculating) comments
        if re.search("#", line):
            line = line.split("#")[0]
            stats.comments += 1

        # Editing lines from input
        if not line.strip():                    # Skip empty lines
            continue
        line = " ".join(line.split())           # Replace multiple whitespaces with one whitespace

        list = re.split(r'\s', line)             # Spliting edited lines by whitespaces
        # Check of the header
        if not header:
            if list[0] == '.IPPcode24':
                print('<program language=\"IPPcode24\">')
                header = True
                stats.loc += 1
                continue
            else:
                print('Missing or incorrectly typed header of IPPcode24', file=sys.stderr)
                sys.exit(21)

        written, argnum = False, 1
        stats = parse(list, counter, written, argnum, stats)
        counter += 1

    if not header:
        print('Missing or incorrectly typed header of IPPcode24', file=sys.stderr)
        sys.exit(21)
    else:
        print('</program>', end="")
    return stats


def parse(list, counter, written, argnum, stats):
    """Fucntion for parsing the OPcodes of given program, calls other functions for argument checking
       also calculates some statistics for STATP extension"""
    instruction = list[0].upper()

    # Checking the format of OPcode
    if re.fullmatch(r'^\..*', instruction):
        print('Too many headers', file=sys.stderr)
        sys.exit(23)

    # Switch for matching the instruction OPcode, sorted by number of arguments
    match instruction:
        # 0 ARGUMENT
        case 'CREATEFRAME' | 'PUSHFRAME' | 'POPFRAME':
            if len(list) != 1:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            print(f'\t<instruction order=\"{counter}\" opcode=\"{instruction}\">')

        case 'RETURN' | 'BREAK':
            if len(list) != 1:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            print(f'\t<instruction order=\"{counter}\" opcode=\"{instruction}\">')
            stats.jumps += 1

        # 1 ARGUMENT
        case 'DEFVAR' | 'POPS':
            if len(list) != 2:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_variable(instruction, list[1], written, counter, argnum)

        case 'PUSHS' | 'WRITE' | 'DPRINT':
            if len(list) != 2:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_symbol(instruction, list[1], written, counter, argnum)

        case 'EXIT':
            if len(list) != 2:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_symbol(instruction, list[1], written, counter, argnum)
            stats.jumps += 1

        case 'LABEL':
            if len(list) != 2:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_label(instruction, list[1], written, counter, argnum)
            stats.labels.append(list[1])

        case 'CALL' | 'JUMP':
            if len(list) != 2:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_label(instruction, list[1], written, counter, argnum)
            stats.jumps += 1
            stats.back_jmp += jump_check(stats, list[1])

        # 2 ARGUMENT
        case 'MOVE' | 'STRLEN' | 'INT2CHAR' | 'TYPE' | 'NOT':
            if len(list) != 3:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_variable(instruction, list[1], written, counter, argnum)
            written, argnum = check_symbol(instruction, list[2], written, counter, argnum)

        case 'READ':
            if len(list) != 3:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_variable(instruction, list[1], written, counter, argnum)
            written, argnum = check_type(instruction, list[2], written, counter, argnum)

        # 3 ARGUMENT
        case 'ADD' | 'SUB' | 'MUL' | 'IDIV' | 'LT' | 'GT' | 'EQ' | 'AND' | 'OR' | 'STRI2INT' | 'CONCAT' | 'GETCHAR' | 'SETCHAR':
            if len(list) != 4:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_variable(instruction, list[1], written, counter, argnum)
            written, argnum = check_symbol(instruction, list[2], written, counter, argnum)
            written, argnum = check_symbol(instruction, list[3], written, counter, argnum)

        case 'JUMPIFEQ' | 'JUMPIFNEQ':
            if len(list) != 4:
                print('Wrong number of arguments in instruction', file=sys.stderr)
                sys.exit(23)
            written, argnum = check_label(instruction, list[1], written, counter, argnum)
            written, argnum = check_symbol(instruction, list[2], written, counter, argnum)
            written, argnum = check_symbol(instruction, list[3], written, counter, argnum)
            stats.jumps += 1
            stats.back_jmp += jump_check(stats, list[1])

        case _:
            print('Unknown or incorrectly entered instruction code of IPPcode24', file=sys.stderr)
            sys.exit(22)

    print('\t</instruction>')  # footer of instruction

    # Calculate other stats
    stats.loc += 1
    # Asociative array for calculating the freq of each instruction
    try:
        stats.freq[instruction] += 1
    except KeyError:
        stats.freq[instruction] = 1
    return stats


if __name__ == '__main__':
    args = my_argparse()
    print('<?xml version=\"1.0\" encoding=\"UTF-8\"?>')
    stats = read_line()
    stats_print(args, stats)
    sys.exit(0)
