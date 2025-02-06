<?php
// instruction.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída pro jednotlivé instrukce programu (následně se v metodě volající tuto metodu vytvoří pole instrukcí)
// Obsahuje atributy pro název instrukce (opcode) a 2D pole pro agrumenty instrukce
class instruction
{
    /**
     * @var mixed[] $args
     */
    public array $args = array(array());
    public string $name;
    public int $arr_cnt;    // Čítač pro ukládání argumentů

    function __construct(string $name)
    {
        $this->arr_cnt = 0;
        $this->name = $name;
    }

    function set_args(string $type, string|null $value): void
    {
        $this->args[$this->arr_cnt++] = array($value, $type);
    }
}