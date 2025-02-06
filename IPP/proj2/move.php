<?php
// move.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída pro instrukci MOVE vkládající hodnoty (a typu) do proměnné na paměťovém rámci
class move
{
    /**
     * @param array<mixed> $arg1
     * @param array<mixed> $arg2
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     * @throws frame_exception
     */
    public static function execute(array $arg1, array $arg2, frames &$frame): void
    {
        $var = defvar::get_variable($arg1, $frame);
        $val = defvar::get_val_from_symb($arg2, $frame);
        $var->set_val($val[0], $val[1]);
    }
}