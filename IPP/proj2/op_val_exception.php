<?php
// op_val_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class op_val_exception extends IPPException
{
    public function __construct(string $message = "ERROR: WRONG OPERAND VALUE", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::OPERAND_VALUE_ERROR, $previous, false);
    }
}