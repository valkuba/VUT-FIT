<?php
// jumpif_n_eq.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída obsluhující instrukce pro podmíněný skok
class jumpif_n_eq
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
    // podle hodnoty $equal se rozhodne, zdali se jedná o skok s rovno nebo nerovno
    // Porovnává hodnoty argumentů a skok na návěští je řešen v metodě start třídy program
    public static function execute(array $arg1, array $arg2, bool $equal, frames $frame): bool
    {
        $res = false;
        $val1 = defvar::get_val_from_symb($arg1, $frame);
        $val2 = defvar::get_val_from_symb($arg2, $frame);
        if ($val1[1] == $val2[1] || $val1[1] == "nil" || $val2[1] == "nil") {
            $res = $val1[0] == $val2[0];
        } else
            throw new op_type_exception();
        return $res == $equal;
    }
}