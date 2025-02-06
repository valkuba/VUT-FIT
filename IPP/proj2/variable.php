<?php
// variable.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída reprezentující proměnné programu, obsahuje atributy pro jméno, typ a hodnotu proměnné
// dále metodu pro inicializaci proměnné a metodu pro zjištění délky řetězce (využívanou řetězcovými instrukcemi)
class variable
{
    /** @var mixed $val */
    public $val;
    public string $name;
    public ?string $type;

    function __construct(string $name)
    {
        $this->name = $name;
        $this->type = null;
        $this->val = null;
    }

    // Metoda, která inicializuje proměnnou - přiřadí ji typ a hodnotu
    /** @param mixed $val */
    function set_val($val, string $type): void
    {
        if ($type == "bool") {
            if ($val === "true" || $val === 1 || $val === true)
                $this->val = true;
            else
                $this->val = false;
            $this->type = $type;
        } elseif (is_numeric($val) && $type != "string") {
            $this->val = intval($val);
            $this->type = $type;
        } elseif ($type == "nil") {
            $this->val = "nil";
            $this->type = $type;
        } elseif ($type == "string") {
            $this->val = $val;
            $this->type = $type;
        }
        else
            throw new semantic_exception();

    }

    // Metoda pro kontrolu jestli daný index není mimo rozmezí řetězce, vrací hodnotu indexu získanou z argumetnů
    /**
     * @param array<mixed> $val1
     * @param array<mixed> $val2
     */
    public static function check_idx_string(array $val1, array $val2) : int {
        if (!($val1[1] == "string" && $val2[1] == "int")) {
            fwrite(STDERR, "ERROR: INVALID TYPES\n");
            exit(53);
        }
        $idx = intval($val2[0]);
        if (strlen($val1[0]) <= $idx || $idx < 0) {
            fwrite(STDERR, "ERROR: INDEX OUT OF BOUNDS\n");
            exit(58);
        }
        return $idx;
    }
}