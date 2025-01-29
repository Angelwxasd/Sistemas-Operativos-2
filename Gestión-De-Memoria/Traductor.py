def mostrar_menu():
    print("\nMenú de opciones:")
    print("1. Introducir dirección virtual")
    print("2. Mostrar tabla de páginas")
    print("3. Salir")

def main():
    # Datos de entrada (puedes modificarlos o pedirlos al usuario)
    input_data = [
        4,  # Tamaño de página
        16, # Tamaño de la memoria física
        32, # Tamaño de la memoria virtual
        1,  # Entrada 1 de la tabla de páginas
        2,  # Entrada 2
        3,  # Entrada 3
        4,  # Entrada 4
        5,  # Entrada 5
        6,  # Entrada 6
        7,  # Entrada 7
        8,  # Entrada 8
    ]

    # Verificar que hay suficientes datos
    if len(input_data) < 3:
        print("Error: Faltan datos en el archivo de entrada.")
        return

    # Leer parámetros iniciales
    tamaño_pagina = input_data[0]
    tam_mem_fisica = input_data[1]
    tam_mem_virtual = input_data[2]
    tabla_paginas = input_data[3:3 + (tam_mem_virtual // tamaño_pagina)]

    # Calcular bits necesarios
    bits_desplazamiento = tamaño_pagina.bit_length() - 1
    num_marcos = tam_mem_fisica // tamaño_pagina
    bits_marco = num_marcos.bit_length() - 1 if num_marcos > 0 else 0

    # Función para convertir a binario con bits requeridos
    def to_bin(num, bits):
        return bin(num)[2:].zfill(bits) if bits > 0 else '0'

    while True:
        mostrar_menu()
        try:
            opcion = int(input("Seleccione una opción: "))
        except ValueError:
            print("Entrada inválida. Por favor, ingrese un número.")
            continue

        if opcion == 1:
            try:
                direccion_virtual = int(input("Introduce la dirección virtual: "))
            except ValueError:
                print("Entrada inválida.")
                continue

            # Calcular número de página y desplazamiento
            numero_pagina = direccion_virtual >> bits_desplazamiento
            desplazamiento = direccion_virtual & (tamaño_pagina - 1)

            if numero_pagina >= len(tabla_paginas):
                print(f"Error: Número de página {numero_pagina} excede la tabla de páginas.")
                continue

            entrada_tabla = tabla_paginas[numero_pagina]

            # Extraer bits de control y marco
            bits_control = (entrada_tabla >> bits_marco) & 0x1F  # 5 bits
            marco = entrada_tabla & ((1 << bits_marco) - 1)

            # Calcular dirección física
            direccion_fisica = (marco << bits_desplazamiento) | desplazamiento

            # Mostrar resultados
            print("\nDirección virtual:")
            print(f"{direccion_virtual} (10)\t{to_bin(direccion_virtual, 0)} (2)")
            print(f"Número de página: {numero_pagina} (10)\t{to_bin(numero_pagina, 0)} (2)")
            print(f"Desplazamiento: {desplazamiento} (10)\t{to_bin(desplazamiento, bits_desplazamiento)} (2)")
            print(f"\nDirección física:")
            print(f"{direccion_fisica} (10)\t{to_bin(direccion_fisica, 0)} (2)")
            print(f"Bits de control: {to_bin(bits_control, 5)} (2)\t{bits_control} (10)")

            # Interpretación de bits de control (ejemplo según práctica)
            # Asumiendo: bit 4: caché, bit 3: referida, bit 2: modificada, bit 1: permiso, bit 0: presente
            presente = bits_control & 0b1
            permiso = (bits_control >> 1) & 0b1
            modificada = (bits_control >> 2) & 0b1
            referida = (bits_control >> 3) & 0b1
            cache = (bits_control >> 4) & 0b1

            print("\nSignificado de los bits de control:")
            print(f"- Presente en memoria física: {'Sí' if presente else 'No'}")
            print(f"- Permiso de escritura: {'Sí' if permiso else 'No'}")
            print(f"- Modificada: {'Sí' if modificada else 'No'}")
            print(f"- Referida en último ciclo: {'Sí' if referida else 'No'}")
            print(f"- Almacenable en caché: {'Sí' if cache else 'No'}")
            print("----------------------------------------")

        elif opcion == 2:
            print("\nTabla de páginas:")
            for i, entrada in enumerate(tabla_paginas):
                print(f"Página {i}: {entrada}")

        elif opcion == 3:
            print("Saliendo...")
            break

        else:
            print("Opción no válida. Por favor, seleccione una opción del menú.")

if __name__ == "__main__":
    main()