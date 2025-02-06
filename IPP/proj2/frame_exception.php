<?php
// frame_exception.php
// Autor: Jakub Valeš, FIT
namespace IPP\Student;

use IPP\Core\Exception\IPPException;
use IPP\Core\ReturnCode;

// Třída dědící z IPPException, sloužící k vyhození výjimky
class frame_exception extends IPPException
{
    public function __construct(string $message = "ERROR: FRAME ACCESS", ?\Throwable $previous = null)
    {
        parent::__construct($message, ReturnCode::FRAME_ACCESS_ERROR, $previous, false);
    }
}