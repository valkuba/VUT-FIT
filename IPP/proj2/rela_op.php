<?php
// rela_op.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída interpretující instrukce relačních operátorů GT, LT, EQ
class rela_op
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
     */
    public static function execute(array $arg1, array $arg2, array $arg3, frames &$frame, string $op): void
    {
        $res = false;
        $var = defvar::get_variable($arg1, $frame);
        $val1 = defvar::get_val_from_symb($arg2, $frame);
        $val2 = defvar::get_val_from_symb($arg3, $frame);
        // Symboly typu nil, mohou být pouze porovnány přes operator EQ
        if ($val1[1] == "nil" || $val2[1] == "nil") {
            if ($op != "EQ")
                throw new op_type_exception();
            if ($val1[1] == $val2[1])
                $res = true;
            elseif ($val1[0] == "" || $val1[0] == "")
                $res = true;
            else
                $res = false;
        } elseif (!($val1[1] == $val2[1]))
            throw new op_type_exception();
        else {
            switch ($op) {
                case "LT":
                    $res = $val1[0] < $val2[0];
                    break;
                case "GT":
                    $res = $val1[0] > $val2[0];
                    break;
                case "EQ":
                    $res = $val1[0] == $val2[0];
                    break;
            }
        }
        $var->set_val($res, "bool");
    }
}