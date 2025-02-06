<?php
// setchar.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída instrukce setchar
class setchar
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @param array<mixed> $arg3
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws value_exception
     * @throws string_exception
     */
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        if ($val2[1] != "string")
            throw new op_type_exception();
        $idx = variable::check_idx_string([$var->val, $var->type], $val1);
        if (!(strlen($val2[0])))
            throw new string_exception();
        $var->set_val(substr_replace($var->val, $val2[0][0], $idx, 1), "string");
    }
}