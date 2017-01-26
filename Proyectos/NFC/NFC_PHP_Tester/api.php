<?php

//Identificador de tarjeta válida
$tarjetaValida = "5bb282cb"

$identificadorRecibido = $_REQUEST['data'];

if($identificadorRecibido == $tarjetaValida)
{
	/*
	*	El código Arduino proporcionado (httpClient) como ejemplo
	*	responde a esta palabra clave llamando a la función success.
	*/

	echo "ACCESS_GRANTED";
}
else
{
	/*
	*	Devuelve el dato que ha recibido, útil para comprobar integridad.
	*/
	
	echo "ACCESS DENIED FOR ID ".$identificadorRecibido;
}

?>
