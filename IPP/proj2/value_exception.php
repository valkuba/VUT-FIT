<?php
// value_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class value_exception extends IPPException
{
    // 56
    public function __construct(string $message = "ERROR: VALUE BIG BAD", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::VALUE_ERROR, $previous, false);
    }
}