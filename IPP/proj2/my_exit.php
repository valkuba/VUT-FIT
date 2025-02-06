<?php
// my_exit.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída pro ukončení programu a vypsání exit-code
class my_exit
{
    /**
     * @param array<mixed> $arg
     * @throws frame_exception
     * @throws op_val_exception
     * @throws op_type_exception
     */
    public static function execute(array $arg, frames $frame): void
    {
        $variable = defvar::find_val($arg[0], $frame);
        if ($variable) {
            if (!($variable->type == "int"))
                throw new op_type_exception();
            if(($variable->val >= 0 && $variable->val < 10))
                exit($variable->val);
            throw new op_val_exception();
        } else {
            $val = intval($arg[0]);
            if (!($arg[1] == "int"))
                throw new op_type_exception();
            if($val >= 0 && $val < 10)
                exit($val);
            throw new op_val_exception();
        }
    }
}