<?php
// struct_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class struct_exception extends IPPException
{
    public function __construct(string $message = "ERROR: WRONG XML STRUCTURE", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::INVALID_SOURCE_STRUCTURE, $previous, false);
    }
}