<?php

//Identificador de tarjeta válida
$tarjetaValida = "5bb282cb"

$identificadorRecibido = $_REQUEST['data'];

if($identificadorRecibido == $tarjetaValida)
{
	echo "ACCESS_GRANTED";
}
else
{
	echo "ACCESS DENIED FOR ID ".$identificadorRecibido;
}

?>
