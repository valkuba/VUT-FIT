<?php
// defvar.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída defvar obsahuje metodu pro provedení instrukce DEFVAR
// a následující instrukce pro hledání proměnných v rámcích
class defvar
{
    /**
     * @throws semantic_exception
     * @throws frame_exception
     */
    // Metoda, která podle předpony proměnné rozhodne na který rámec bude proměnná uložena a následně ji uloží
    // Kontroluje jestli se nejedná o redefinici proměnné
    public static function execute(string $val, frames &$frame): void
    {
        if (preg_match('/^(GF@).*/', $val)) {
            $var_val = explode("@", $val);
            foreach ($frame->global_frame as $var_name) {
                if ($var_val[1] == $var_name->name)
                    throw new semantic_exception();
            }
            $variable = new variable($var_val[1]);
            array_push($frame->global_frame, $variable);

        } elseif (preg_match('/^(LF@).*/', $val)) {
            if ($frame->local_frame === null)
                throw new frame_exception();

            $var_val = explode("@", $val);
            foreach ($frame->local_frame[0] as $var_name) {
                if ($var_val[1] == $var_name->name)
                    throw new semantic_exception();
            }
            $variable = new variable($var_val[1]);
            array_push($frame->local_frame[0], $variable);

        } elseif (preg_match('/^(TF@).*/', $val)) {
            if ($frame->temp_frame === null)
                throw new frame_exception();

            $var_val = explode("@", $val);
            foreach ($frame->temp_frame as $var_name) {
                if ($var_val[1] == $var_name->name)
                    throw new semantic_exception();
            }
            $variable = new variable($var_val[1]);
            array_push($frame->temp_frame, $variable);
        }
        else
            throw new struct_exception();
    }

    /**
     * @throws frame_exception
     */
    // Statická metoda pro vyhledání proměnné v paměťových rámcích
    // Vrací objekt proměnné v případě úspšchu, jinak null
    public static function find_val(string $val, frames $frame): ?variable
    {
        if (preg_match('/^(GF@).*/', $val)) {
            $var_val = explode("@", $val);
            foreach ($frame->global_frame as $var_name) {
                if ($var_val[1] == $var_name->name) {
                    return $var_name;
                }
            }
        } elseif (preg_match('/^(LF@).*/', $val)) {
            if ($frame->local_frame === null)
                throw new frame_exception();
            $var_val = explode("@", $val);
            foreach ($frame->local_frame[0] as $var_name) {
                if ($var_val[1] == $var_name->name) {
                    return $var_name;
                }
            }
        } elseif (preg_match('/^(TF@).*/', $val)) {
            if ($frame->temp_frame === null)
                throw new frame_exception();
            $var_val = explode("@", $val);
            foreach ($frame->temp_frame as $var_name) {
                if ($var_val[1] == $var_name->name) {
                    return $var_name;
                }
            }
        }
        return null;
    }

    /**
     * @param array<mixed> $arg
     * @return array<mixed>
     * @throws var_exception
     * @throws semantic_exception
     * @throws value_exception
     * @throws frame_exception
     */
    // Statická metoda, která z argumentů instrukce zjistí, zdali se jedná o prměnnou nebo symbol
    // A navrátí pole o dvou prvcích obsahující hodnotu proměnné/symbolu a typ
    public static function get_val_from_symb(array $arg, frames $frame): array
    {
        if ($arg[1] == "var") {
            $var = defvar::find_val($arg[0], $frame);
            if ($var == null) {
                throw new var_exception();
            }
            if ($var->type == null) {
                throw new value_exception();
            }
            $val = $var->val;
            $type = $var->type;
        }
        else {
            $type = $arg[1];
            switch ($type) {
                case "int":
                    if (!(is_numeric($arg[0])))
                        throw new semantic_exception();
                    $val = intval($arg[0]);
                    break;
                case "string":
                    $val = $arg[0];
                    break;
                case "bool":
                    if ($arg[0] == "true")
                        $val = true;
                    else
                        $val = false;
                    break;
                case "nil":
                    $val = "nil";
                    break;
                default:
                    $val = $arg[0];
            }
        }
        return [$val, $type];
    }

    /**
     * @param array<mixed> $arg
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     */
    // Statická metoda, která vrací objekt proměnné
    public static function get_variable(array $arg, frames $frame): variable
    {
        if (!$arg[1] == "var")
            throw new semantic_exception();
        $var = defvar::find_val($arg[0], $frame);
        if ($var == null)
            throw new var_exception();
        return $var;
    }
}