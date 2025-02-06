<?php
// semantic_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class semantic_exception extends IPPException
{
    public function __construct(string $message = "ERROR: SEMANTIC ERROR", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::SEMANTIC_ERROR, $previous, false);
    }
}