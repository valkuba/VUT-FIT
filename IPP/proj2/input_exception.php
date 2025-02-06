<?php
// input_exception.php
// Autor: Jakub Valeš, FIT
namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;
class input_exception extends IPPException
{
    public function __construct(string $message = "ERROR: INPUT ERROR", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::INPUT_FILE_ERROR, $previous, false);
    }
}