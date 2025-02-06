<?php
// boolean_op.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída obsluhující metodu pro interpretaci instrukcí AND a OR
class boolean_op
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
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame, string $op): void
    {
        $res = 0;
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        if (!($val1[1] == $val2[1] && $val1[1] == "bool"))
            throw new op_type_exception();
        switch ($op) {
            case "AND":
                $res = $val1[0] && $val2[0];
                break;
            case "OR":
                $res = $val2[0] || $val1[0];
                break;
        }
        $var->set_val($res, "bool");
    }
}