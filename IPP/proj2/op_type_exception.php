<?php
// op_type_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class op_type_exception extends IPPException
{
    public function __construct(string $message = "ERROR: WRONG OPERAND TYPE ERROR", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::OPERAND_TYPE_ERROR, $previous, false);
    }
}