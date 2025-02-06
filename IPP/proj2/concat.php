<?php
// concat.php
// Autor: Jakub Valeš, FIT
namespace IPP\Student;

// Metoda pro řetězcovou instrukci concat
class concat
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @param array<mixed> $arg3
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     */
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        if (!($val1[1] == $val2[1] && $val1[1] == "string"))
            throw new op_type_exception();
        $val1[0] = $val1[0] . $val2[0];
        $var->set_val($val1[0], "string");
    }
}