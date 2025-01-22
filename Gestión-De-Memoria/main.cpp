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
    int size;               
    int size_bloque;
    vector<unsigned char> bytes;
    vector<Proceso> procesos;   

public:
    MapaBits(int n, int s ) : size(n) , size_bloque(s) {
        bytes.resize((n/s)/8, 0); // Inicializar con el número necesario de bytes
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
        if ( tamaño == 0  || (tamaño*size_bloque) > size || buscarProceso(nombre) ) {
            return false; 
        }

        cout << " "<<tamaño<<" " << nombre << size <<endl;

        int cantidadDisponible = 0; 
        int inicioCeldaDisponible = 0;
        for ( int i = 0 ; i < (bytes.size()*8); i++ ) {
            if ( !consultarBit(i) ) {
                cout<<"Cantidad disponible en i: " << i<<endl;
                cantidadDisponible++; 
            } else {
                inicioCeldaDisponible = i+1; 
                cantidadDisponible=0; 
            }

            if ( cantidadDisponible == tamaño ) {
                cout << "Cantidad disponible igual a tamaño"<<endl;
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

        for (int i = 0; i < bytes.size()/8; i++)
            cout << "       " << (i + 1) * 8;

        cout << endl;

        for (int fila = 0; fila <= 7; fila++)
        {
            cout << "<" << celda << "> ";
            celda++;

            for (int i = 0; i < bytes.size(); i++)
            {
                if (((bytes[i] >> fila) & 1) == 1)
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
        return (bytes[pos/8] & (1 << pos%8)) != 0;
    }

    void fijarUno(int pos) {
        if (pos < 0 || pos >= size) {
            return;
        }
        bytes[pos / 8] |= (1 << (pos % 8));
    }

    void limpiarBit(int pos) {
        if (pos < 0 || pos >= size) {
            return;
        }
        bytes[pos / 8] &= ~(1 << (pos % 8));
    }

};

bool revisarEntrada ( ) {
    if (cin.fail()) {
        cout << "Entrada no válida\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true; 
    }
    return false;
}

int main() {

    int tM = 1024; 
    int tB = 4096;

    MapaBits* mapa = new MapaBits(tM*1024,tB);

    int opcion;
    do {
        cout << "\n\t--- Menú (Memoria: "<<tM*1024<<" Kb, bloque: "<<tB<<" bytes) ---\n";
        cout << "1. Insertar un proceso\n";
        cout << "2. Eliminar un proceso\n";
        cout << "3. Ver mapa\n";
        cout << "4. Configurar memoria\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if ( revisarEntrada() )  continue;

        switch (opcion) {
            case 1: {
                string nombre;
                int tamaño;
                cout << "Ingrese el nombre del proceso: ";
                cin >> nombre;
                cout << "Ingrese no. bloques del proceso: ";
                cin >> tamaño;

                if ( revisarEntrada() )  continue;

                if ( mapa->insertarProceso(tamaño, nombre) == false ) 
                    cout<<"Error al insertar proceso"<<endl;
                else {
                    mapa->imprimirMapa();
                }
                break;
            }
            case 2: {
                string nombre;
                cout << "Ingrese el nombre del proceso a eliminar: ";
                cin >> nombre;
                if ( mapa->eliminarProceso(nombre) == false ) 
                    cout<<"Error al eliminar el proceso, no se encontró"<<endl;
                else {
                    mapa->imprimirMapa();
                }
                break;
            }
            case 3: {
                mapa->imprimirMapa();
                break;
            }
            case 4: {
                int tMemoria;
                int tBytes;

                cout << "Ingrese tamaño de memoria(Kb): "; 
                cin >> tMemoria; 
                if ( revisarEntrada() )  continue;
                cout << "Ingrese tamaño de bloque de memoria(b): "; 
                cin >> tBytes; 
                if ( revisarEntrada() )  continue;

                int tMb = tMemoria*1024;

                if ( tMb < tBytes ) 
                    cout << "El tamaño de bloque es mayor a lo permitido\n";
                else {
                    double r = static_cast<double>(tMb)/tBytes; 
                    int r2 = tMb/tBytes; // division entera

                    if ( (r - r2) != 0 || ((r/8)-r2/8) > 0 ) 
                        cout << "El tamaño no es permitido\n";
                   else {
                        delete mapa; 
                        mapa = new MapaBits(tMemoria*1024,tBytes);
                        tM = tMemoria;
                        tB = tBytes;
                        cout <<"Mapa configurado\n";
                    }
                }
                break;
            }
            case 5: {
                cout << "Saliendo del programa." << endl;
                break;
            }
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 5);


    return 0;
}
