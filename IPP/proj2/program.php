<?php
// program.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Interface\InputReader;
use IPP\Core\Interface\OutputWriter;

class program
{
    /**
     * @param array<mixed> $label_map
     * @param array<mixed> $instr
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws value_exception
     * @throws string_exception
     * @throws op_val_exception
     * @throws struct_exception
     */
    // Statická metoda, která postupně prochází instrukce programu a volá pro ně odpovídající metody
    public static function start(int $IP, array $label_map, array $instr, OutputWriter $output, OutputWriter $error, InputReader $input): void
    {
        $call_stack = array();          // Zásobník volání, využívaný instrukcemi CALL a RETURN
        $frame = new frames();          // Objekt reprezentující jednotlivé rámce programu (globální, lokální, dočasný)
        $stack = array(array());        // Zásobník pro instrukce POPS a PUSHS
        $next = true;                   // Přiznak jestli se má načíst následující instrukce (true) nebo byla provedena instukce skoku
        while ($IP <= array_key_last($instr)) {
            switch (strtoupper($instr[$IP]->name)) {
                case "CREATEFRAME":
                    $frame->create_frame();
                    break;
                case "PUSHFRAME":
                    $frame->push_frame();
                    break;
                case "POPFRAME":
                    $frame->pop_frame();
                    break;
                case "RETURN":
                        if (count($call_stack) < 1)
                        throw new value_exception();
                    $IP = end($call_stack);
                    array_shift($call_stack);
                    $next = false;
                    break;
                case "BREAK":
                    $error->WriteString("INTERNAL STATE");
                    break;

                case "PUSHS":
                    $stack_sym = defvar::get_val_from_symb($instr[$IP]->args[0], $frame);
                    array_unshift($stack, $stack_sym);
                    break;
                case "POPS":
                    if (count($stack) > 1)
                        move::execute($instr[$IP]->args[0], $stack[0], $frame);
                    else
                        throw new value_exception();
                    array_shift($stack);
                    break;
                case "LABEL":
                    if (count($instr[$IP]->args) != 1) {
                        fwrite(STDERR, "ERROR: LABEL ERROR\n");
                        exit(99);
                    } else
                        break;
                case "DEFVAR":
                    defvar::execute($instr[$IP]->args[0][0], $frame);
                    break;

                case "CALL":
                    if (!(array_key_exists($instr[$IP]->args[0][0], $label_map)))
                        throw new semantic_exception();
                    array_unshift($call_stack, $IP + 1);
                    $IP = $label_map[$instr[$IP]->args[0][0]];
                    $next = false;
                    break;
                case "JUMP":
                    if (!(array_key_exists($instr[$IP]->args[0][0], $label_map)))
                        throw new semantic_exception();
                    $IP = $label_map[$instr[$IP]->args[0][0]];
                    $next = false;
                    break;

                case "EXIT":
                    my_exit::execute($instr[$IP]->args[0], $frame);
                    break;
                case "DPRINT":
                    $error->WriteString("DPRINT");
                    break;
                case "WRITE":
                    write::execute($instr[$IP]->args[0], $frame, $output);
                    break;

                case "MOVE":
                    move::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame);
                    break;
                case "STRLEN":
                    strlen::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame);
                    break;
                case "INT2CHAR":
                    int2char::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame);
                    break;
                case "TYPE":
                    type::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame);
                    break;
                case "NOT":
                    not::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame);
                    break;
                case "READ":
                    read::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $frame, $input);
                    break;

                case "JUMPIFEQ":
                    if (jumpif_n_eq::execute($instr[$IP]->args[1], $instr[$IP]->args[2], true, $frame)) {
                        if (!(array_key_exists($instr[$IP]->args[0][0], $label_map)))
                            throw new semantic_exception();
                        $IP = $label_map[$instr[$IP]->args[0][0]];
                        $next = false;
                    }
                    break;
                case "JUMPIFNEQ":
                    if (jumpif_n_eq::execute($instr[$IP]->args[1], $instr[$IP]->args[2], false, $frame)) {
                        if (!(array_key_exists($instr[$IP]->args[0][0], $label_map)))
                            throw new semantic_exception();
                        $IP = $label_map[$instr[$IP]->args[0][0]];
                        $next = false;
                    }
                    break;
                case "CONCAT":
                    concat::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame);
                    break;

                case "GETCHAR":
                    getchar::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame);
                    break;
                case "STRI2INT":
                    stri2int::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame);
                    break;
                case "SETCHAR":
                    setchar::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame);
                    break;
                case "ADD":
                case "SUB":
                case "MUL":
                case "IDIV":
                    basic_arith_op::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame, $instr[$IP]->name);
                    break;
                case "LT":
                case "GT":
                case "EQ":
                    rela_op::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame, $instr[$IP]->name);
                    break;
                case "AND":
                case "OR":
                    boolean_op::execute($instr[$IP]->args[0], $instr[$IP]->args[1], $instr[$IP]->args[2], $frame, $instr[$IP]->name);
                    break;
                default:
                    throw new struct_exception();
            }
            // Logika načtení další instukce, pokud předchozí nebyla skok
            if ($next) {
                while (!array_key_exists(++$IP, $instr)) {
                    if ($IP > array_key_last($instr))
                        break;
                }
            } else
                $next = true;
        }
    }
}