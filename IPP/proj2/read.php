<?php
// read.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Interface\InputReader;

// Třída obsahující metodu pro čtení ze souboru/STDIN podle agrumentu programu --input
class read
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     */
    // Objekt InputReader je převzit z třídy FileInputReader a vypisuje na STDOUT
    public static function execute(array $arg1, array $arg2, frames &$frame, InputReader &$input): void
    {
        $val = 0;
        $var = defvar::get_variable($arg1, $frame);
        switch ($arg2[0]) {
            case "int":
                $val = $input->readInt();
                break;
            case "string":
                $val = $input->readString();
                break;
            case "bool":
                $val = $input->readbool();
                break;
            case "nil":
                $val = "";
                break;
            default:
                throw new struct_exception();
        }
        // Proměnná nebo symbol typu nil vypíše prázdný řetězec
        if ($val === null) {
            $val = "";
            $arg2[0] = "nil";
        }
        $var->set_val($val, $arg2[0]);
    }
}