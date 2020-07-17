;En este programa se realiza la experiencia 1 y experiencia 2:
.model small
.stack 100h
.data 

;Mensajes que serán usados para la solicitud e impresión de datos:
msj1 db "Ingrese una secuencia y finalice con enter: ", 13, 10, "$"
msj2 db 13, 10,"La cantidad de digitos es: $" 
msj3 db 13, 10,"La secuencia invertida es: $" 
   
.code
	;Se apunta los datos al segmento de datos
	mov ax, @data
	mov ds, ax 
	;Se genera una interrupción para imprimir el mensaje 1
	mov ah, 9
	mov dx, offset msj1
	int 21h
    
	;Se inicializa el valor de cx que contará el numero de letras
	mov cx, 0
    
	;Inicializar el bucle de lectura de una secuencia
	bucle: 
		;Interrupción para leer una letra
		mov ah, 1
		int 21h   
		cmp al, 13
		;Si la letra es "enter" o "cambio de linea" se sale del buble
		jz fin
		;En caso contrario se pushea al stack el valor de la letra y
		;se aumenta el valor de cx en uno
		push ax
		inc cx
		jmp bucle 
       
	;Una vez el el usuario termina de escribir su secuencia entra aquí
	fin: 
		;Se genera una interrupción para escribir el mensaje 2
		mov ah, 9
		mov dx, offset msj2
		int 21h
        
		;Se copia el valor de cx a al que tiene la cantidad de letras
		mov al, cl
		;aam divide el valor de al en 2
		aam
		mov bl, al
		mov dl, ah  
		;Se imprime el primer digito
		add dl, 48 
		mov ah, 2
		int 21h
		;Se imprime el segundo digito
		mov dl, bl
		add dl, 48
		mov ah, 2
		int 21h 
       
		;Finalmente se imprime el mensaje 3
		mov ah, 9
		mov dx, offset msj3
		int 21h    
     
	;Se imprime la secuencia que ingresó el usuario al revés usando el stack
	loop1: 
		pop dx
		mov ah, 2
		int 21h
		loop loop1 
;.exit es equivalente a:
;mov ah, 4ch
;int 21h
.exit
end