// ===================================================================
//  Proyecto final: Distancia de Hamming en secuencias de ADN
//  Código base
// ===================================================================

#include <iostream>   // cout y cin: escribir en pantalla y leer del teclado
#include <fstream>    // ifstream: leer archivos de texto
#include <string>     // string: cadenas de caracteres
#include <cctype>     // toupper: convertir una letra a mayúscula
#include <iomanip>    // setprecision: controlar los decimales al imprimir
using namespace std;

// -------------------------------------------------------------------
// Incluir aqui el código que ejecute lo solicitado en el enunciado
// (funciones de las Partes A y B).
// -------------------------------------------------------------------

// Osea empezamos metiendo la cadena a mayusculas con toupper, hubiera preferido usar ascii, y sobretodo hubiera preferido tener while disponible
string aMayusculas(string s)
{for(int(i)=0;i<s.length();i++) {s[i] = toupper(s[i]);}
return s;}
// Veo por bool esValida que tengan las letras A,C,G,T unicamente
bool esValida(const string &s)
{for (int(i)=0;i<s.length();i++) 
{if (s[i] != 'A' && s[i] != 'C' && s[i] != 'G' && s[i] != 'T') 
{return false;}}
return true;}
// Veo distancia entre posiciones de a y b con int distanciaHamming(const string &a,const string &b)
int distanciaHamming(const string &a,const string &b)
{int distancia=0;for (int i = 0; i < a.length(); i++) {
if (a[i] != b[i]) {distancia++;}}
return distancia;}
// Meto double porcentajeIdentidad(int d, int L)  para que nos deje con (L-d)/L*100 sin que agarre divsion entera (con double)
double porcentajeIdentidad(int d, int L)
{if (L == 0) {return 0.0;}
return ((double)(L - d) / L) * 100.0;}
// -------------------------------------------------------------------
// PARTE B: Conteo de nucleótidos con arreglos y contenido GC
// -------------------------------------------------------------------

// contamos cuantas veces aparecen los nucleotidos, con oid contarNucleotidos(const string &s,int conteo[4]) 
// Metemos las posiciones del arreglo en 0 y realizamos conteo por la secuencia en la posicion que es
void contarNucleotidos(const string &s, int conteo[4])
{for (int i = 0; i < 4; i++)
{conteo[i] = 0;}
for (int i = 0; i < s.length(); i++)
{if (s[i] == 'A') {conteo[0]++;}
else if (s[i] == 'C') {conteo[1]++;}
else if (s[i] == 'G') {conteo[2]++;}
else if (s[i] == 'T') {conteo[3]++;}}}
// ahora aggaro con double contenidoGC(const int conteo[4], int L) que me devuelve (conteo de C + conteo de G) / L ×100.
double contenidoGC(const int conteo[4], int L)
{if (L == 0) {return 0.0;}
// La formula (conteo de C [conteo[1]] + conteo de G [conteo[2]]) / L * 100) (porque se usa double)
return ((double)(conteo[1] + conteo[2]) / L) * 100.0;}
// -------------------------------------------------------------------
// separarPareja
//
// Recibe una linea del archivo, por ejemplo "ATCGAT ATGGCT", y la
// divide en dos secuencias: seq1 = "ATCGAT" y seq2 = "ATGGCT".
//
// Parámetros:
//   linea  la linea leida del archivo. Se pasa como 'const string &'
//          para no copiarla y para garantizar que no se modifica.
//   seq1   aqui se guarda la primera secuencia.
//   seq2   aqui se guarda la segunda secuencia.
//          seq1 y seq2 se pasan por referencia ('string &') porque la
//          función debe "devolver" dos valores: los cambios que se les
//          hagan aqui dentro se ven en la variable original del main.
//
// Valor de retorno:
//   true   si la linea tiene exactamente un separador (un espacio o
//          una coma) con texto a ambos lados.
//   false  en cualquier otro caso; entonces seq1 y seq2 no se tocan.
// -------------------------------------------------------------------
bool separarPareja(const string &linea, string &seq1, string &seq2) {
    // 'pos' guardará la posición (el indice) del separador.
    // Empieza en -1, un valor imposible como indice, que aqui
    // significa "todavia no he encontrado ningun separador".
    int pos = -1;

    // Recorremos la linea carácter por carácter.
    // linea.length() es el número de caracteres de la linea, y
    // linea[i] es el carácter que está en la posición i (desde 0).
    for (int i = 0; i < linea.length(); i++) {
        if (linea[i] == ' ' || linea[i] == ',') {
            // Encontramos un separador. Si 'pos' ya no vale -1,
            // significa que este es el SEGUNDO separador de la linea,
            // por ejemplo en "AAA CCC GGG". Esa linea no es valida.
            if (pos != -1) {
                return false;
            }
            // Es el primer separador: recordamos dónde está.
            pos = i;
        }
    }

    // Al salir del ciclo hay tres situaciones inválidas:
    //   pos == -1                 no habia separador ("ATCG")
    //   pos == 0                  el separador es el primer carácter
    //                             (" ATCG"): no hay primera secuencia
    //   pos == linea.length() - 1 el separador es el último carácter
    //                             ("ATCG,"): no hay segunda secuencia
    // La condición 'pos <= 0' cubre a la vez los casos -1 y 0.
    if (pos <= 0 || pos == linea.length() - 1) {
        return false;
    }

    // substr(inicio, cantidad) extrae 'cantidad' caracteres desde
    // 'inicio'. Con la linea "ATCGAT ATGGCT", pos vale 6:
    //   linea.substr(0, 6) -> "ATCGAT"  (posiciones 0 a 5)
    //   linea.substr(7)    -> "ATGGCT"  (desde la posición 7 hasta
    //                                    el final de la linea)
    seq1 = linea.substr(0, pos);
    seq2 = linea.substr(pos + 1);
    return true;
}


int main() {
    // ---------------------------------------------------------------
    // 1. Pedir el nombre del archivo y abrirlo
    // ---------------------------------------------------------------
    string nombreArchivo;
    cout << "Nombre del archivo: ";
    cin >> nombreArchivo;   // lee una palabra (el nombre no debe
                            // tener espacios, p. ej. secuencias.txt)
    cout << endl;

    // 'ifstream' (input file stream) es un flujo de entrada asociado
    // a un archivo. Se usa igual que 'cin', pero los datos vienen del
    // archivo en lugar del teclado. Al crearlo con el nombre, intenta
    // abrir el archivo. El archivo debe estar en la misma carpeta
    // desde donde se ejecuta el programa (o escribir la ruta completa).
    ifstream archivo(nombreArchivo);

    // is_open() devuelve false si el archivo no se pudo abrir: no
    // existe, el nombre está mal escrito o no hay permisos.
    // En ese caso no tiene sentido continuar: terminamos el programa.
    // 'return 1' le indica al sistema operativo que hubo un error
    // ('return 0', al final, indica que todo salió bien).
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo." << endl;
        return 1;
    }

    // 'fixed' y 'setprecision(1)' hacen que todos los números con
    // decimales (double) que se impriman de aqui en adelante muestren
    // exactamente un decimal: 66.7 en lugar de 66.6667.
    cout << fixed << setprecision(1);

    // ---------------------------------------------------------------
    // 2. Leer el archivo linea por linea
    // ---------------------------------------------------------------
    string linea;        // aqui se guarda cada linea leida
    int numPareja = 0;   // cuántas parejas con formato correcto van

    // getline(archivo, linea) lee la linea completa siguiente del
    // archivo (hasta el salto de linea, sin incluirlo) y la guarda en
    // 'linea'. Devuelve un valor que se evalúa como verdadero si logró
    // leer algo, y como falso cuando ya no quedan lineas. Por eso el
    // ciclo 'while' se repite una vez por cada linea y se detiene
    // solo al llegar al final del archivo.
    //
    // Se usa getline y no 'archivo >> linea' porque >> se detiene en
    // el primer espacio, y aqui necesitamos la linea entera.
    while (getline(archivo, linea)) {

        // Los archivos creados en Windows terminan cada linea con dos
        // caracteres: '\r' y '\n'. getline quita el '\n' pero deja el
        // '\r' al final, invisible en pantalla, y haria que "ATGGCT\r"
        // tuviera un carácter inválido. Si el último carácter es '\r',
        // lo borramos con erase(posición).
        if (!linea.empty() && linea[linea.length() - 1] == '\r') {
            linea.erase(linea.length() - 1);
        }

        // Una linea vacia no tiene datos: 'continue' salta el resto
        // del cuerpo del ciclo y pasa directamente a leer la siguiente.
        if (linea.empty()) {
            continue;
        }

        // Intentamos dividir la linea en dos secuencias. Si el formato
        // es incorrecto, avisamos y pasamos a la siguiente linea sin
        // contarla como pareja.
        string seq1, seq2;
        if (!separarPareja(linea, seq1, seq2)) {
            cout << "Aviso: linea ignorada por formato incorrecto: "
                 << linea << endl;
            continue;
        }

        // Si llegamos aqui, la linea tiene formato correcto: es una
        // pareja nueva y le asignamos su número.
        numPareja++;

// Pasamos el texto a mayuscula
seq1 = aMayusculas(seq1);seq2 = aMayusculas(seq2);
// Vemos si alguna tiene letras distintas a A, C, G, T
if (!esValida(seq1) || !esValida(seq2))
{cout << "Pareja " << numPareja << ": " << seq1 << " vs " << seq2 
<< " -> Error: caracteres no validos" << endl;}
// Vemos si tiene la misma longitud
else if (seq1.length() != seq2.length())
{cout << "Pareja " << numPareja << ": " << seq1 << " vs " << seq2 
<< " -> Error: longitudes diferentes" << endl;}
// Si no tienen errores entonces proseguimos
else {int distancia = distanciaHamming(seq1, seq2);
double identidad = porcentajeIdentidad(distancia, seq1.length());
cout << "Pareja " << numPareja << ": " << seq1 << " vs " << seq2 // Hacemos un header con cout
<< " -> Distancia: " << distancia << " | Identidad: " << identidad << "%" << endl;
// Muestra la primera secuencia (conteo y GC)
int conteo1[4];
contarNucleotidos(seq1, conteo1);
double gc1 = contenidoGC(conteo1, seq1.length());
cout << seq1 << " A=" << conteo1[0] << " C=" << conteo1[1] 
<< " G=" << conteo1[2] << " T=" << conteo1[3] << " GC=" << gc1 << "%" << endl;
// Muestra la segunda secuencia (conteo y GC)
int conteo2[4];
contarNucleotidos(seq2, conteo2);
double gc2 = contenidoGC(conteo2, seq2.length());
cout << seq2 << " A=" << conteo2[0] << " C=" << conteo2[1] 
<< " G=" << conteo2[2] << " T=" << conteo2[3] << " GC=" << gc2 << "%" << endl;}}

    // Cerramos el archivo: ya no lo necesitamos. (Se cerraria solo al
    // terminar el programa, pero cerrarlo explicitamente es buena
    // costumbre.)
    archivo.close();
    return 0;}
//usa g++ hamming.cpp -o hamming, luego ./hamming, y luego pones el nombre del archivo que quieres leer, por ejemplo pruebas.txt