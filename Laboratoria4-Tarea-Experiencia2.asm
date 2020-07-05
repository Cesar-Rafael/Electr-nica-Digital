.model small
.stack 100h
.data

array db 1,3,8,0,2,5,9,4,6,7	;arreglo desordenado de 10 elementos
espacio db " ", "$"	; espacio para la impresion de cada elemento
fin db 10, 13, "$"	; cambio de linea después de la impresion del ultimo elemento

.code 
	mov ax, @data ;se apunta a la data
	mov ds, ax
	
	mov cx, 9	; se inicializa el registro Cx con el valor de 9, ya que el BubbleSort itera hasta hasta n-1 elementos
				; En nuestro caso n = 10, por tanto n-1 = 9 veces
	
	;Se inicializa la iteración externa
	loop1:
		;Se apunta al arreglo que queremos ordenar
		mov si, offset array
		;En cada iteracion, nuestro registro b empezará en 0, es un contador que recorrerá el arreglo
		mov bx, 0
		
		;Se inicializa la iteración interna
		loop2:
			;Primero se compara si el contador Bx ya ha llegado hasta el valor de Cx
			cmp bx, cx
			;En caso sean iguales, significa que ya cumplió la iteración interna y sale de esta:
			jz iter	;jz hace un salto en caso bx y cx sean iguales
			;Se copian los elementos del arreglo que van a ser comparados, i y i+1 donde i es igual a bx
			mov dl, [si + bx]
			mov al, [si + bx + 1]
			;Se compara al y dl
			cmp al, dl
			jb swap	;jp hace un salto en caso dl sea menor que al
			;swap en una subrutina que cambiará los datos del arreglo
			inc bx	;Siempre si incrementa el valor de bx para que pueda salir de la iteración
			jmp loop2	;Se itera
		iter:	;Aquí llega el salto en caso bx y cx sean iguales
	loop loop1	;En esta linea de código, se itera y asimismo se decrementa en 1 el valor de cx y cuando sea 0 sale del bucle
	
	;Inicializamos cx y bx para la impresión del arreglo:
	mov cx, 10
	mov bx, 0
	
	
	impresion:	;Se imprime el arreglo
	;Se apunta al arreglo
	mov si, offset array
	;Se guarda el valor en la posición bx
	mov al, [si + bx]
	;Se incrementa el valor de bx para recorrer el arreglo
	inc bx
	
	;Se imprime como caracter
	add al, 48
	mov ah, 2
	mov dl, al
	int 21h

	;Se agrega un espacio entre cada numero
	mov dx, offset espacio
	mov ah, 9
	int 21h
	
	loop impresion ; Itera y decrementa el valor de cx

	;Impresion del cambio de linea final 
	mov dx, offset fin
	mov ah, 9
	int 21h
	
	;Fin del programa:
	dormamu:
	mov al, 0
	mov ah, 04ch
	int 21h
	jmp dormamu
	
	;Subturina que cambia los elementos del arreglo según la posición bx
	swap proc
		mov [si + bx], al
		mov [si + bx + 1], dl
		ret
	swap endp
end 
