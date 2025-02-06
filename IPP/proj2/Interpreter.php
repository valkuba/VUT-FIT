<?php
// Interpreter.php
// Řešení IPP-Skript-php,  16.4.2024
// Autor: Jakub Valeš, xvales04, FIT
// Interpretováno: php 8.3.2

namespace IPP\Student;

use IPP\Core\AbstractInterpreter;

class Interpreter extends AbstractInterpreter
{

    /**
     * @throws op_val_exception
     * @throws op_type_exception
     * @throws var_exception
     * @throws semantic_exception
     * @throws frame_exception
     * @throws struct_exception
     * @throws value_exception
     * @throws string_exception
     */
    // Metoda třídy Interpreter načte XML vstup ze souboru/STDIN a zpracuje jej na jednotlivé instrukce programu
    // Následně volá statickou metodu start, které předává pole instrukcí programu
    public function execute(): int
    {
        // Načtení XML
        $dom = $this->source->getDOMDocument();
        $dom_instr = $dom->getElementsByTagName("instruction");

        $instructions = array();    // Pole pro jednotlivé instrukce
        $label_map = array();       // hashovací tabulka pro návěští a číslo odpovídající instrukce
        $IP = 0;                    // ukazatel na číslo aktuálně prováděné instrukce
        foreach ($dom_instr as $single_instr) {
            $order = $single_instr->getAttribute("order");
            if (array_key_exists($order, $instructions))
                throw new struct_exception();
            // Vytvoření nové instance třídy instruction pro načtenou instrukci a přidání do pole instrukcí
            $instructions[$order] = new instruction($single_instr->getAttribute("opcode"));

            // Zpracování argumentů instrukce z XML
            $args = $single_instr->childNodes;
            foreach ($args as $arg) {
                if ($arg->nodeType === XML_ELEMENT_NODE && $arg instanceof \DOMElement) {
                    $instructions[$order]->set_args($arg->getAttribute("type"), $arg->nodeValue);
                }
            }
            // Mapování návěští na číslo instrukce a kontrola unikátnosti návěští
            if ($instructions[$order]->name == "LABEL") {
                if (array_key_exists($instructions[$order]->args[0][0], $label_map))
                    throw new semantic_exception();
                $label_map[$instructions[$order]->args[0][0]] = $order;
            }
        }
        // Seřazení instrukcí podle opcode v poli
        ksort($instructions);

        // Zjištění opcode první instrukce
        while (!array_key_exists(++$IP, $instructions)) {
            if ($IP > array_key_last($instructions))
                return 0;
        }
        program::start($IP, $label_map, $instructions, $this->stdout, $this->stderr, $this->input);

        return 0;
    }
}

