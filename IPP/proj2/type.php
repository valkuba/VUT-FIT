<?php
// type.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída pro instrukci TYPE zjišťující typ proměnné/symbolu a zapisující ho do proměnné
class type
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws value_exception
     */
    public static function execute(array $arg1, array $arg2, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val = array();
        if ($arg2[1] == "var") {
            $var2 = defvar::find_val($arg2[0], $frame);
            if ($var2 == null) {
                throw new var_exception();
            }
            if ($var2->type == null) {
                $val[1] = "";
            } else
                $val[1] = $var2->type;
        } else
            $val = defvar::get_val_from_symb($arg2, $frame);
        $var->set_val($val[1], "string");
    }
}