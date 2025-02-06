<?php
// var_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class var_exception extends IPPException
{
    public function __construct(string $message = "ERROR: VARIABLE ACCESS", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::VARIABLE_ACCESS_ERROR, $previous, false);
    }
}