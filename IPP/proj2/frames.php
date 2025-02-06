<?php
// frames.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

// Třída reprezentující rámce programu pro proměnné
// Atributy jsou jednotlivé rámce, přičemž lokální rámec reprezentuje zásobník rámců
class frames
{
    /** @var mixed[] $global_frame */
    public array $global_frame;
    /** @var mixed[] $temp_frame */
    public ?array $temp_frame;
    /** @var mixed[] $local_frame */
    public ?array $local_frame;

    function __construct()
    {
        $this->global_frame = array();
        $this->temp_frame = null;
        $this->local_frame = null;
    }

    /**
     * @throws frame_exception
     */
    // Metoda přidává aktuální dočasný rámec na zásobník rámců (lokální rámce)
    public function push_frame(): void
    {
        if ($this->temp_frame === null)
            throw new frame_exception();
        if ($this->local_frame === null)
            $this->local_frame = array(array());
        array_unshift($this->local_frame, $this->temp_frame);
        $this->temp_frame = null;
    }

    /**
     * @throws frame_exception
     */
    // Metoda načte vrchol ze zásobníku rámců a vloží jej do přeměnné pro dočasný rámec
    // Načtený vrchol ze zásobníku odstraní
    public function pop_frame(): void
    {
        if ($this->local_frame === null || count($this->local_frame) == 0)
            throw new frame_exception();
        $this->temp_frame = $this->local_frame[0];
        array_shift($this->local_frame);
    }

    // Metoda pro vytvoření nového dočasného rámce (pokud již existuje přepíše jej prázdným rámcem)
    public function create_frame(): void
    {
        if ($this->temp_frame)
            $this->temp_frame = null;
        $this->temp_frame = array();
    }
}