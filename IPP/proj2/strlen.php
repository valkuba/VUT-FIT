<?php
// strlen.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Statická metoda třídy strlen pro stejno jmennou instrukci
class strlen
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws value_exception
     */
    public static function execute(array $arg1, array $arg2, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val = defvar::get_val_from_symb($arg2, $frame);
        if ($val[1] != "string")
            throw new op_type_exception();
        $var->set_val(strlen($val[0]), "int");
    }
}