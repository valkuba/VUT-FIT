<?php
// stri2int.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třida instrukce stri2int
class stri2int
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @param array<mixed> $arg3
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws value_exception
     */
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        $idx = variable::check_idx_string($val1, $val2);
        $var->set_val(strval(ord(substr($val1[0], $idx, 1))), "int");
    }
}