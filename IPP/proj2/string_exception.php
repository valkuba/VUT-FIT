<?php
// string_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class string_exception extends IPPException
{
    public function __construct(string $message = "ERROR: STRING OPERATION", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::STRING_OPERATION_ERROR, $previous, false);
    }
}