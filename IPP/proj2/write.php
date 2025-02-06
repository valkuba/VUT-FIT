<?php
// write.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Interface\OutputWriter;

// Třída write obsahuje statickou metodu pro výpis na STDOUT
class write
{
    /**
     * @param array<mixed> $arg
     * @throws frame_exception
     */
    public static function execute(array $arg, frames $frame, OutputWriter $streamWriter): void
    {
        $type = $arg[1];
        $val = $arg[0];
        // V případě proměnné je zapotřebí získat typ a hodnotu z paměťového rámce
        if ($type == "var") {
            $variable = defvar::find_val($arg[0], $frame);
            if ($variable) {
                $type = $variable->type;
                $val = $variable->val;
            }
        }
        // Následný výpis, složítější je u řetězců kvůli interpretaci escape sekvencí
        if ($type == "string") {
            $ptrn = '/([0-9]{3})/';
            // Využití callback funkce z knihovny regex funkcí pro interpretaci escape sekvence
            // ordinální hodnota z escape sekvence je pomocí funkce intval převedena z řetězce na integer
            $val = preg_replace_callback($ptrn, function ($matched) { return mb_chr(intval($matched[1]), "UTF-8"); }, $val);
            // Funkce mb_char následně vrátí unicode reprezentaci ordinální hodnoty získané z escape sekvence
            $streamWriter->writeString(stripcslashes($val));
        } elseif ($type == "int")
            $streamWriter->writeInt(intval($val));
        elseif ($type == "bool")
            $streamWriter->writeBool(boolval($val));
        elseif ($type == "nil")
            // Výpis hodnoty typu nil je prázdný řetězec
            $streamWriter->writeString("");
        else
            throw new value_exception();
    }
}