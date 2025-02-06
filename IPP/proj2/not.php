<?php
// not.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Logická funkce NOT
class not
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     * @throws frame_exception
     */
    public static function execute(array $arg1, array $arg2, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        if ($val1[1] != "bool")
            throw new op_type_exception();
        $var->set_val((!$val1[0]), "bool");
    }
}