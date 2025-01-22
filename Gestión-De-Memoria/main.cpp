#include <iostream>
#include <string>
#include <vector>
#include<algorithm>
#include <limits>
using namespace std; 

class Proceso {
public: 
    string nombre;
    int bitInicio;
    int bitFinal;

public:
    Proceso(string n, int bI, int bF) : nombre(n), bitInicio(bI), bitFinal(bF) {}

    void imprimir() const {
        cout << "Proceso: " << nombre << " | Inicio: " << bitInicio+1<< " | Final: " << bitFinal+1<< endl;
    }


};

class MapaBits {
private:
    size_t size;                // Número total de bits
    vector<unsigned char> bits; // Vector para almacenar los bits
    vector<Proceso> procesos;   // Vector para almacenar los procesos

public:
    MapaBits(size_t n) : size(n) {
        bits.resize((n + 7) / 8, 0); // Inicializar con el número necesario de bytes
    }

    Proceso* buscarProceso( string& nombre ) {
        for (auto& proceso : procesos) {
            if (proceso.nombre == nombre) {
                return &proceso; 
            }
        }
        return nullptr;
    }

    bool eliminarProceso ( string nombre ) {
        Proceso* p = buscarProceso(nombre);
        if ( p == nullptr ) {
            return false; 
        }
        for ( int i = p->bitInicio; i < p->bitFinal+1; i++ ){
            limpiarBit(i);
        }

        auto it = find_if(procesos.begin(), procesos.end(), [&nombre](const Proceso& proceso) {
            return proceso.nombre == nombre;
        });

        procesos.erase(it);

        return true; 
    }

    bool insertarProceso ( int tamaño, string nombre ) {
        if ( tamaño == 0  || tamaño > size || buscarProceso(nombre) ) {
            return false; 
        }
        int cantidadDisponible = 0; 
        int inicioCeldaDisponible = 0;
        for ( int i = 0 ; i < 1024; i++ ) {
            if ( !consultarBit(i) ) {
                cantidadDisponible++; 
            } else {
                inicioCeldaDisponible = i+1; 
                cantidadDisponible=0; 
            }

            if ( cantidadDisponible == tamaño ) {

                Proceso p(nombre,inicioCeldaDisponible,inicioCeldaDisponible+tamaño-1);
                procesos.push_back(p);

                for ( int j = inicioCeldaDisponible; j < inicioCeldaDisponible+tamaño; j++ ) {
                    fijarUno(j);
                }
                return true; 
            }

        }

        return false; 
    }

    void imprimirMapa() const
    {
        int celda = 1;

        for (int i = 0; i < bits.size() / 8; i++)
            cout << "       " << (i + 1) * 8;

        cout << endl;

        for (int fila = 0; fila <= 7; fila++)
        {
            cout << "<" << celda << "> ";
            celda++;

            for (int i = 0; i < bits.size(); i++)
            {
                if (((bits[i] >> fila) & 1) == 1)
                    cout << "■";
                else
                    cout << "□";
                if ((i + 1) % 8 == 0)
                    cout << " ";
            }
            cout << endl;
        }

        cout << "Procesos:" << endl;
        for (const auto& proceso : procesos) {
            proceso.imprimir();
        }

    }

    bool consultarBit(int pos) const {
        if (pos < 0 || pos >= size) {
            return false;
        }
        return (bits[pos/8] & (1 << pos%8)) != 0;
    }

    void fijarUno(int pos) {
        if (pos < 0 || pos >= size) {
            return;
        }
        bits[pos / 8] |= (1 << (pos % 8));
    }

    void limpiarBit(int pos) {
        if (pos < 0 || pos >= size) {
            return;
        }
        bits[pos / 8] &= ~(1 << (pos % 8));
    }

};

int main() {
    int n = 1024; // Número de bits
    MapaBits mapa(n);

    int opcion;
    do {
        cout << "\n--- Menú ---\n";
        cout << "1. Insertar un proceso\n";
        cout << "2. Eliminar un proceso\n";
        cout << "3. Ver mapa\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (cin.fail()) {
            cout << "Entrada no válida. Por favor, ingresa un número.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        } 

        switch (opcion) {
            case 1: {
                string nombre;
                int tamaño;
                cout << "Ingrese el nombre del proceso: ";
                cin >> nombre;
                cout << "Ingrese el tamaño del proceso: ";
                cin >> tamaño;
                if ( mapa.insertarProceso(tamaño, nombre) == false ) 
                    cout<<"Error al insertar proceso, ya existe ese nombre o es demasiado grande"<<endl;
                else {
                    mapa.imprimirMapa();
                }
                break;
            }
            case 2: {
                string nombre;
                cout << "Ingrese el nombre del proceso a eliminar: ";
                cin >> nombre;
                if ( mapa.eliminarProceso(nombre) == false ) 
                    cout<<"Error al eliminar el proceso, no se encontró"<<endl;
                else {
                    mapa.imprimirMapa();
                }
                break;
            }
            case 3: 
                mapa.imprimirMapa();
                break;
            case 4:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 4);


    return 0;
}
