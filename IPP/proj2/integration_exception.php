<?php
// integration_exception.php
// Autor: Jakub Valeš, FIT

namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

class integration_exception extends IPPException
{
    public function __construct(string $message = "ERROR: INTEGRATION", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::INTEGRATION_ERROR, $previous, false);
    }
}