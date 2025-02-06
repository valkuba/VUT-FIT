<?php
// int2char.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída pro instrukci int2char
class int2char
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     * @throws frame_exception
     * @throws string_exception
     */
    public static function execute(array $arg1, array $arg2, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val = defvar::get_val_from_symb($arg2, $frame);
        // Kontola jestli je první argument instrukce číslo
        if (!(is_numeric($val[0]) && $val[1] == "int"))
            throw new op_type_exception();
        $val[0] = intval($val[0]);              // převedení na int není vždy potřeba, protože hodnota již může být int
        if ($val[0] < 0 || $val[0] > 255)       // kontrola indexace v rámci pole
            throw new string_exception();
        $var->set_val(chr($val[0]), "string");
    }
}