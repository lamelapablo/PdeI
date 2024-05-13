import socket
import sys

def recibirDatos(sock):
    datos = b""
    buff_size = 4096
    # La primera vez nos forzamos a esperar la llegada de datos
    sock.setblocking(True)
    try:
        while True:
            datos += sock.recv(buff_size)
            # A partir de ahora si no hay datos que leer finalizamos el bucle
            sock.setblocking(False)
    except socket.error: #excepcion que se va a lanzar cuando no se lea nada del socket en modo no bloqueante
        pass
    
    sock.setblocking(True)

    return datos

def main():
    # Creando un socket TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Conecta el socket en el puerto cuando el servidor esté escuchando
    server_dir = ('localhost', 3535)
    print(f'Conectando a {server_dir[0]}:{server_dir[1]} ...')
    sock.connect(server_dir)

    print(f"Hora de conexion recibida del server: {recibirDatos(sock).decode()}")

    print('Cuando quiera cerrar la conexion escriba "quit": ');
    while((salida := input()) != "quit"):
        pass

    sock.sendall(salida.encode())

    print(f"La conexion ha durado {recibirDatos(sock).decode()} segundos")

    print('Cerrando socket ...')
    sock.close()

if __name__ == "__main__":
    main()
