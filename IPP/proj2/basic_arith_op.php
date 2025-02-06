<?php
// basic_arith_op.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída obsluhující aritmetické instukce programu (ADD, SUB, MUL, IDIV)
class basic_arith_op
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @param array<mixed> $arg3
     * @throws op_val_exception
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     * @throws frame_exception
     */
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame, string $op): void
    {
        $res = 0;
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        if (!($val1[1] == $val2[1] && $val1[1] == "int"))
            throw new op_type_exception();
        switch ($op) {
            case "ADD":
                $res = $val1[0] + $val2[0];
                break;
            case "SUB":
                $res = $val1[0] - $val2[0];
                break;
            case "MUL":
                $res = $val1[0] * $val2[0];
                break;
            case "IDIV":
                if ($val2[0] == 0)
                    throw new op_val_exception();
                $res = $val1[0] / $val2[0];
                break;
        }
        $var->set_val($res, "int");
    }
}