// evaluacion_bloom_refactor.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// Para verificar que las opciones sean numeros y dentro de los rangos permitidos (Benjamin)
#include <limits>


using namespace std;

#define DEBUG_STOI(var, name)                          \
    cout << "Leyendo " << name << ": " << var << endl; \
    stoi(var);

string nivelToStr(int nivel)
{
    switch (nivel)
    {
    case 0:
        return "Recordar";
    case 1:
        return "Entender";
    case 2:
        return "Aplicar";
    case 3:
        return "Analizar";
    case 4:
        return "Evaluar";
    case 5:
        return "Crear";
    default:
        return "Desconocido";
    }
}

int strToNivel(const string &s)
{
    if (s == "Recordar")
        return 0;
    if (s == "Entender")
        return 1;
    if (s == "Aplicar")
        return 2;
    if (s == "Analizar")
        return 3;
    if (s == "Evaluar")
        return 4;
    return 5;
}

// Verifica que las opciones sean válidas y devuelve el valor ingresado. (Benjamin)
int leerEnteroSeguro(const string& mensaje, int min = 0, int max = 100) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min && valor <= max)
                return valor;
            else
                cout << "⚠️ Valor fuera del rango permitido (" << min << "-" << max << "). Intenta de nuevo.\n";
        } else {
            cout << "❌ Entrada inválida. Por favor ingresa un número.\n";
            cin.clear(); // Limpia el error
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta el resto de la línea
    }
}

class PreguntaBase
{
public:
    int id;
    string enunciado;
    int nivel;
    int tiempoEstimado;
    string respuestaCorrecta;
    int anioUso;

    virtual void mostrar() const = 0;
    virtual string tipo() const = 0;

    // Nueva interfaz polimórfica
    virtual void crearDesdeConsola() = 0;
    virtual void guardar(ofstream &out) const = 0;
    virtual void cargar(stringstream &ss) = 0;

    virtual ~PreguntaBase() {}
};

class PreguntaMultiple : public PreguntaBase
{
public:
    string opciones[10]; // máximo 10 opciones
    int cantidadOpciones;
    int indiceCorrecto;

    string tipo() const override { return "Opcion Multiple"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Opción Múltiple, " << nivelToStr(nivel) << "]\n";
        for (int i = 0; i < cantidadOpciones; ++i)
        {
            cout << "  " << static_cast<char>('A' + i) << ") " << opciones[i];
            if (i == indiceCorrecto)
                cout << "  ✅";
            cout << "\n";
        }
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: "; // Para que sea mas claro que se ingresa el ID. (Benjamin)
        //cin >> id;
        id = leerEnteroSeguro("> ", 0, 1000); // Lo desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        nivel = leerEnteroSeguro("> ", 0, 5);        
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso; // desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();
        cout << "Cantidad de opciones (máx 10): ";
        //cin >> cantidadOpciones; // desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        cantidadOpciones = leerEnteroSeguro("> ", 0, 10);
        cin.ignore();
        for (int i = 0; i < cantidadOpciones; ++i)
        {
            cout << "Opción " << static_cast<char>('A' + i) << ": ";
            getline(cin, opciones[i]);
        }

        cout << "Índice de la opción correcta (alternativa 1 a " << cantidadOpciones << "): ";
        // cin >> indiceCorrecto; // desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        indiceCorrecto = leerEnteroSeguro("> ", 1, cantidadOpciones) - 1;
        cin.ignore();

        if (indiceCorrecto >= 0 && indiceCorrecto < cantidadOpciones)
            respuestaCorrecta = opciones[indiceCorrecto];
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << cantidadOpciones << ","
            << indiceCorrecto;

        for (int i = 0; i < cantidadOpciones; ++i)
        {
            out << "," << opciones[i];
        }
        out << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;

        if (!getline(ss, temp, ','))
            return;
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "cantidadOpciones");
        cantidadOpciones = stoi(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "indiceCorrecto");
        indiceCorrecto = stoi(temp);

        for (int i = 0; i < cantidadOpciones; ++i)
        {
            if (!getline(ss, opciones[i], ','))
                opciones[i] = "";
        }
    }
};

class PreguntaVF : public PreguntaBase
{
public:
    bool esVerdadero;
    string justificacion;

    string tipo() const override { return "Verdadero/Falso"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Verdadero/Falso, " << nivelToStr(nivel) << "]\n";
        cout << "→ Valor: " << (esVerdadero ? "Verdadero" : "Falso") << "\n";
        cout << "→ Justificación: " << justificacion << "\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: "; // Para que sea mas claro que se ingresa el ID. (Benjamin)
        // cin >> id; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).}
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Año de uso (1900 - 2030): ";
        //cin >> anioUso;
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();

        int valor;
        cout << "¿Es Verdadero (1) o Falso (0)? ";
        //cin >> valor;
        valor = leerEnteroSeguro("> ", 0, 1);
        cin.ignore();
        esVerdadero = (valor == 1);
        respuestaCorrecta = esVerdadero ? "Verdadero" : "Falso";

        cout << "Justificación de la respuesta: ";
        getline(cin, justificacion);
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << (esVerdadero ? "1" : "0") << ","
            << justificacion << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;

        if (!getline(ss, temp, ','))
            return;
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, temp, ','))
            return;
        esVerdadero = (temp == "1");

        if (!getline(ss, justificacion, ','))
            return;
    }
};

class PreguntaEnsayo : public PreguntaBase
{
public:
    string criteriosEvaluacion;
    int extensionMinima;
    string contexto;

    string tipo() const override { return "Ensayo"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Ensayo, " << nivelToStr(nivel) << "]\n";
        cout << "→ Contexto: " << contexto << "\n";
        cout << "→ Respuesta esperada: " << respuestaCorrecta << "\n";
        cout << "→ Criterios: " << criteriosEvaluacion << "\n";
        cout << "→ Extensión mínima: " << extensionMinima << " palabras\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        // cin >> id; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado;
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Respuesta esperada: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();
        cout << "Contexto del ensayo: ";
        getline(cin, contexto);
        cout << "Criterios de evaluación: ";
        getline(cin, criteriosEvaluacion);
        cout << "Extensión mínima (palabras): ";
        cin >> extensionMinima;
        cin.ignore();
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << contexto << ","
            << criteriosEvaluacion << ","
            << extensionMinima << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;

        if (!getline(ss, temp, ','))
            return;
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, contexto, ','))
            return;
        if (!getline(ss, criteriosEvaluacion, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "extensionMinima");
        extensionMinima = stoi(temp);
    }
};

class PreguntaCorta : public PreguntaBase
{
public:
    string tipoEsperado; // ej: "palabra", "número", "fecha"

    string tipo() const override { return "Respuesta Corta"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Respuesta Corta, " << nivelToStr(nivel) << "]\n";
        cout << "→ Respuesta esperada: " << respuestaCorrecta << "\n";
        cout << "→ Tipo esperado: " << tipoEsperado << "\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Respuesta Correcta: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso; // Desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();
        cout << "Tipo de dato esperado (ej: palabra, número, fecha): ";
        getline(cin, tipoEsperado);
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << tipoEsperado << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "nivel");
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, tipoEsperado, ','))
            return;
    }
};

class PreguntaTextoIncompleto : public PreguntaBase
{
public:
    string plantilla; // Texto base con espacio vacío

    string tipo() const override { return "Texto Incompleto"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Texto Incompleto, " << nivelToStr(nivel) << "]\n";
        cout << "→ Plantilla: " << plantilla << "\n";
        cout << "→ Palabra clave esperada: " << respuestaCorrecta << "\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        // cin >> id;
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel;
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado;
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Respuesta Correcta: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso;
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();
        cout << "Texto plantilla con espacio a completar (use ___ o ...): ";
        getline(cin, plantilla);
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << plantilla << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "nivel");
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, plantilla, ','))
            return;
    }
};

struct ParEmparejamiento
{
    int id;
    string conceptoA;
    string conceptoB;
};

class PreguntaEmparejamiento : public PreguntaBase
{
    ParEmparejamiento pares[10];
    int cantidadPares;

    string tipo() const override { return "Emparejamiento"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Emparejamiento, " << nivelToStr(nivel) << "]\n";

        cout << "\nLista A:\n";
        for (int i = 0; i < cantidadPares; ++i)
        {
            cout << i + 1 << ") " << pares[i].conceptoA << "\n";
        }

        cout << "\nLista B (desordenada):\n";
        // Copia para desordenar sin afectar el orden lógico
        string desordenada[10];
        for (int i = 0; i < cantidadPares; ++i)
            desordenada[i] = pares[i].conceptoB;

        for (int i = 0; i < cantidadPares; ++i)
        {
            int j = (i * 3 + 2) % cantidadPares;
            string tmp = desordenada[i];
            desordenada[i] = desordenada[j];
            desordenada[j] = tmp;
        }

        for (int i = 0; i < cantidadPares; ++i)
        {
            cout << static_cast<char>('A' + i) << ") " << desordenada[i] << "\n";
        }
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        // cin >> id;
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel;
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado;
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso;
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();
        cout << "Cantidad de pares (máximo 10): ";
        // cin >> cantidadPares;
        cantidadPares = leerEnteroSeguro("> ", 0, 10);
        cin.ignore();

        for (int i = 0; i < cantidadPares; ++i)
        {
            pares[i].id = i;
            cout << "Concepto A" << i + 1 << ": ";
            getline(cin, pares[i].conceptoA);
            cout << "Concepto B" << i + 1 << ": ";
            getline(cin, pares[i].conceptoB);
        }

        respuestaCorrecta = "Pares definidos";
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << cantidadPares;

        for (int i = 0; i < cantidadPares; ++i)
        {
            out << "," << pares[i].conceptoA << "," << pares[i].conceptoB;
        }

        out << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "nivel");
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "cantidadPares");
        cantidadPares = stoi(temp);

        for (int i = 0; i < cantidadPares; ++i)
        {
            pares[i].id = i;
            if (!getline(ss, pares[i].conceptoA, ','))
                return;
            if (!getline(ss, pares[i].conceptoB, ','))
                return;
        }
    }

    int evaluar() const
    {
        cout << "\n--- Evaluación de pregunta de emparejamiento ---\n";
        string opcionesDesordenadas[10];
        int indicesDesordenados[10];

        // Copiar valores
        for (int i = 0; i < cantidadPares; ++i)
        {
            opcionesDesordenadas[i] = pares[i].conceptoB;
            indicesDesordenados[i] = i;
        }

        // Mezclar los índices (sencilla permutación determinista)
        for (int i = 0; i < cantidadPares; ++i)
        {
            int j = (i * 3 + 1) % cantidadPares;
            swap(opcionesDesordenadas[i], opcionesDesordenadas[j]);
            swap(indicesDesordenados[i], indicesDesordenados[j]);
        }

        // Mostrar columnas
        cout << "\nConceptos A:\n";
        for (int i = 0; i < cantidadPares; ++i)
        {
            cout << i + 1 << ") " << pares[i].conceptoA << "\n";
        }

        cout << "\nConceptos B:\n";
        for (int i = 0; i < cantidadPares; ++i)
        {
            cout << static_cast<char>('A' + i) << ") " << opcionesDesordenadas[i] << "\n";
        }

        // Entrada del usuario
        int correctas = 0;
        for (int i = 0; i < cantidadPares; ++i)
        {
            cout << "\n¿Con qué letra emparejas el concepto A" << i + 1 << "? ";
            char letra;
            cin >> letra;
            int seleccion = letra - 'A';

            int realIndexB = indicesDesordenados[seleccion];
            if (pares[i].conceptoB == pares[realIndexB].conceptoB)
            {
                cout << "✅ Correcto.\n";
                correctas++;
            }
            else
            {
                cout << "❌ Incorrecto. Correcto era: " << pares[i].conceptoB << "\n";
            }
        }

        cout << "\n✔️ Obtuviste " << correctas << " de " << cantidadPares << " emparejamientos correctos.\n";
        return correctas;
    }
};

class PreguntaAnalogias : public PreguntaBase
{
public:
    string parReferenciaA;
    string parReferenciaB;
    string parIncompletoA;
    string parEsperadoB;
    string tipoRelacion;

    string tipo() const override { return "Analogías"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Analogías, " << nivelToStr(nivel) << "]\n";
        cout << "→ " << parReferenciaA << " : " << parReferenciaB << " :: " << parIncompletoA << " : ?\n";
        cout << "→ Tipo de relación: " << tipoRelacion << "\n";
        cout << "→ Respuesta esperada: " << parEsperadoB << "\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        // cin >> id;
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel;
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado;
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Año de uso (1900 - 2030): ";
        //cin >> anioUso;
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();

        cout << "Primer par de referencia (A : B)\n";
        cout << "  A: ";
        getline(cin, parReferenciaA);
        cout << "  B: ";
        getline(cin, parReferenciaB);

        cout << "Par incompleto a completar (C : ?)\n";
        cout << "  C: ";
        getline(cin, parIncompletoA);
        cout << "Respuesta esperada (D): ";
        getline(cin, parEsperadoB);

        cout << "Tipo de relación (ej: función, causa-efecto): ";
        getline(cin, tipoRelacion);

        respuestaCorrecta = parEsperadoB; // Para compatibilidad
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << parReferenciaA << ","
            << parReferenciaB << ","
            << parIncompletoA << ","
            << parEsperadoB << ","
            << tipoRelacion << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "nivel");
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, parReferenciaA, ','))
            return;
        if (!getline(ss, parReferenciaB, ','))
            return;
        if (!getline(ss, parIncompletoA, ','))
            return;
        if (!getline(ss, parEsperadoB, ','))
            return;
        if (!getline(ss, tipoRelacion, ','))
            return;
    }
};

class PreguntaGrafico : public PreguntaBase
{
public:
    string tipoGrafico;
    string descripcionRecurso;
    string preguntaRelacionada;

    string tipo() const override { return "Grafico"; }

    void mostrar() const override
    {
        cout << id << ". " << enunciado << " [Interpretación Gráfica, " << nivelToStr(nivel) << "]\n";
        cout << "→ Tipo de gráfico: " << tipoGrafico << "\n";
        cout << "→ Descripción del recurso: " << descripcionRecurso << "\n";
        cout << "→ Pregunta a responder: " << preguntaRelacionada << "\n";
        cout << "→ Respuesta esperada: " << respuestaCorrecta << "\n";
    }

    void crearDesdeConsola() override
    {
        cout << "Ingrese el ID: ";
        // cin >> id;
        id = leerEnteroSeguro("> ", 0, 1000);
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
        // cin >> nivel;
        nivel = leerEnteroSeguro("> ", 0, 5);
        cout << "Tiempo estimado (min): ";
        // cin >> tiempoEstimado;
        tiempoEstimado = leerEnteroSeguro("> ", 0, 100);
        cin.ignore();
        cout << "Año de uso (1900 - 2030): ";
        // cin >> anioUso;
        anioUso = leerEnteroSeguro("> ", 1900, 2030);
        cin.ignore();

        cout << "Tipo de gráfico (barras, circular, línea, etc.): ";
        getline(cin, tipoGrafico);
        cout << "Descripción del recurso: ";
        getline(cin, descripcionRecurso);
        cout << "Pregunta relacionada a partir del gráfico: ";
        getline(cin, preguntaRelacionada);
        cout << "Respuesta esperada: ";
        getline(cin, respuestaCorrecta);
    }

    void guardar(ofstream &out) const override
    {
        out << nivelToStr(nivel) << ","
            << tiempoEstimado << ","
            << respuestaCorrecta << ","
            << anioUso << ","
            << tipoGrafico << ","
            << descripcionRecurso << ","
            << preguntaRelacionada << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "nivel");
        nivel = strToNivel(temp);

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "tiempoEstimado");
        tiempoEstimado = stoi(temp);

        if (!getline(ss, respuestaCorrecta, ','))
            return;

        if (!getline(ss, temp, ','))
            return;
        DEBUG_STOI(temp, "anioUso");
        anioUso = stoi(temp);

        if (!getline(ss, tipoGrafico, ','))
            return;
        if (!getline(ss, descripcionRecurso, ','))
            return;
        if (!getline(ss, preguntaRelacionada, ','))
            return;
    }
};

PreguntaBase *crearPreguntaDesdeTipo(const string &tipoStr)
{
    if (tipoStr == "Opcion Multiple")
        return new PreguntaMultiple();
    if (tipoStr == "Verdadero/Falso")
        return new PreguntaVF();
    if (tipoStr == "Ensayo")
        return new PreguntaEnsayo();
    if (tipoStr == "Respuesta Corta")
        return new PreguntaCorta();
    if (tipoStr == "Texto Incompleto")
        return new PreguntaTextoIncompleto();
    if (tipoStr == "Emparejamiento")
        return new PreguntaEmparejamiento();
    if (tipoStr == "Analogías")
        return new PreguntaAnalogias();
    if (tipoStr == "Grafico")
        return new PreguntaGrafico();
    return nullptr;
}

// El resto de la implementación de las clases PreguntaBase y sus derivados deben actualizar sus métodos guardar() y cargar()
// para que trabajen en conjunto con este formato, separando los campos comunes y específicos.

class GestorPreguntas
{
public:
    static const int MAX = 1000;
    PreguntaBase *banco[MAX];
    int cantidad;
    string archivo;

    GestorPreguntas() : cantidad(0), archivo("preguntas.csv") { cargar(); }

    ~GestorPreguntas()
    {
        for (int i = 0; i < cantidad; ++i)
            delete banco[i];
    }

    void guardar()
    {
        ofstream file(archivo);
        for (int i = 0; i < cantidad; ++i)
        {
            file << banco[i]->id << ","
                 << banco[i]->enunciado << ","
                 << banco[i]->tipo() << ",";
            banco[i]->guardar(file);
        }
    }

    void cargar()
    {
        ifstream file(archivo);
        string linea;
        while (getline(file, linea) && cantidad < MAX)
        {
            stringstream ss(linea);
            string idStr, enunciadoTmp, tipoStr;

            if (!getline(ss, idStr, ','))
                continue;
            if (!getline(ss, enunciadoTmp, ','))
                continue;
            if (!getline(ss, tipoStr, ','))
                continue;

            PreguntaBase *p = crearPreguntaDesdeTipo(tipoStr);
            if (p)
            {
                try
                {
                    p->id = stoi(idStr);
                    p->enunciado = enunciadoTmp;
                    p->cargar(ss);
                    banco[cantidad++] = p;
                }
                catch (exception &e)
                {
                    cout << "Error al cargar pregunta: " << e.what() << endl;
                    delete p;
                }
            }
        }
    }

    int buscarIndicePorId(int id)
    {
        for (int i = 0; i < cantidad; ++i)
            if (banco[i]->id == id)
                return i;
        return -1;
    }

    void crearPregunta()
    {
        if (cantidad >= MAX)
        {
            cout << "Límite de preguntas alcanzado.\n";
            return;
        }

        cout << "\nTipo de pregunta:\n"
             << "1. Opcion Multiple\n"
             << "2. Verdadero/Falso\n"
             << "3. Ensayo\n"
             << "4. Respuesta Corta\n"
             << "5. Texto Incompleto\n"
             << "6. Emparejamiento\n"
             << "7. Analogías\n"
             << "8. Grafico\n> ";
        int tipo;
        //cin >> tipo; // Lo desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        tipo = leerEnteroSeguro("> ", 1, 8);
        cin.ignore();

        PreguntaBase *p = nullptr;
        switch (tipo)
        {
        case 1:
            p = new PreguntaMultiple();
            break;
        case 2:
            p = new PreguntaVF();
            break;
        case 3:
            p = new PreguntaEnsayo();
            break;
        case 4:
            p = new PreguntaCorta();
            break;
        case 5:
            p = new PreguntaTextoIncompleto();
            break;
        case 6:
            p = new PreguntaEmparejamiento();
            break;
        case 7:
            p = new PreguntaAnalogias();
            break;
        case 8:
            p = new PreguntaGrafico();
            break;
        default:
            cout << "Tipo inválido.\n";
            return;
        }

        if (!p)
            return;
        p->crearDesdeConsola();

        for (int i = 0; i < cantidad; ++i)
        {
            if (banco[i]->id == p->id)
            {
                cout << "Ya existe una pregunta con ese ID.\n";
                delete p;
                return;
            }
            if (banco[i]->anioUso == p->anioUso - 1 && banco[i]->enunciado == p->enunciado)
            {
                cout << "No se puede reutilizar una pregunta de un año consecutivo.\n";
                delete p;
                return;
            }
        }

        banco[cantidad++] = p;
        guardar();
        cout << "\nPregunta creada correctamente.\n\n";
    }

    void listar()
    {
        for (int i = 0; i < cantidad; ++i)
            banco[i]->mostrar();
    }

    PreguntaBase *getPregunta(int idx) const
    {
        if (idx >= 0 && idx < cantidad)
            return banco[idx];
        return nullptr;
    }

    int getCantidad() const { return cantidad; }
};

struct Evaluacion
{
    string asignatura;
    int anio;
    PreguntaBase *preguntas[100];
    int cantidad;

    Evaluacion() : cantidad(0) {}

    void agregarPregunta(PreguntaBase *p)
    {
        if (cantidad < 100)
            preguntas[cantidad++] = p;
    }

    int tiempoTotal() const
    {
        int total = 0;
        for (int i = 0; i < cantidad; ++i)
            total += preguntas[i]->tiempoEstimado;
        return total;
    }

    void mostrar() const
    {
        cout << "\nEvaluación: " << asignatura << " (" << anio << ")\n";
        for (int i = 0; i < cantidad; ++i)
            preguntas[i]->mostrar();
        cout << "Tiempo total estimado: " << tiempoTotal() << " minutos\n";
    }
};

void generarEvaluacion(GestorPreguntas &gestor)
{
    Evaluacion eval;
    cout << "Nombre de la asignatura: ";
    cin.ignore();
    getline(cin, eval.asignatura);
    cout << "Año de la evaluación: ";
    // cin >> eval.anio;
    eval.anio = leerEnteroSeguro("> ", 1900, 2030);
    cout << "Cantidad de preguntas a incluir: ";
    int num;
    // cin >> num;
    num = leerEnteroSeguro("> ", 0, 100);

    for (int i = 0; i < num; ++i)
    {
        cout << "Nivel Bloom de la pregunta " << (i + 1) << " (0-5)\n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n> "; // Para mas claridad sobre los niveles. (Benjamin)"
        int nivelInt;
        // cin >> nivelInt;
        nivelInt = leerEnteroSeguro("> ", 0, 5);

        for (int j = 0; j < gestor.getCantidad(); ++j)
        {
            PreguntaBase *p = gestor.getPregunta(j);
            if (p && p->nivel == nivelInt && p->anioUso != eval.anio && p->anioUso != eval.anio - 1)
            {
                eval.agregarPregunta(p);
                break;
            }
        }
    }
    eval.mostrar();
}

int main()
{
    GestorPreguntas gestor;
    int opcion;
    cout << "\n\n- Bienvenido al sistema de gestión de preguntas." << endl; // Mensaje de bienvenida. (Benjamin)
    do
    {
        cout << "\n- Ingrese una opcion:\n1. Crear pregunta\n2. Listar preguntas\n3. Buscar por nivel\n4. Editar pregunta\n5. Eliminar pregunta\n6. Generar evaluación\n0. Salir\n> "; // Ajuste un poco el texto para que sea más claro. (Benjamin).
        //cin >> opcion; // Lo desmarque mientras veia si funcionaba el leerEnteroSeguro. (Benjamin).
        opcion = leerEnteroSeguro("> ", 0, 6);
        switch (opcion)
        {
        case 1:
            gestor.crearPregunta();
            break;
        case 2:
            gestor.listar();
            break;
        case 3:
        {
            int nivel;
            cout << "Nivel a buscar (0-5): ";
            cin >> nivel;
            for (int i = 0; i < gestor.getCantidad(); ++i)
            {
                PreguntaBase *p = gestor.getPregunta(i);
                if (p && p->nivel == nivel)
                    p->mostrar();
            }
            break;
        }
        case 4:
        {
            int id;
            cout << "ID de la pregunta a editar: ";
            cin >> id;
            cin.ignore();
            PreguntaBase *p = nullptr;
            for (int i = 0; i < gestor.getCantidad(); ++i)
            {
                if (gestor.getPregunta(i)->id == id)
                {
                    p = gestor.getPregunta(i);
                    break;
                }
            }
            if (!p)
            {
                cout << "Pregunta no encontrada.\n";
                break;
            }
            cout << "Nuevo enunciado: ";
            getline(cin, p->enunciado);
            cout << "Nivel Bloom (0-5): \n> 0. Recordar\n> 1. Entender\n> 2. Aplicar\n> 3. Analizar\n> 4. Evaluar\n> 5. Crear\n "; // Para mas claridad sobre los niveles. (Benjamin)
            cin >> p->nivel;
            cout << "Tiempo estimado (min): ";
            cin >> p->tiempoEstimado;
            cin.ignore();
            cout << "Respuesta Correcta: ";
            getline(cin, p->respuestaCorrecta);
            cout << "Año de uso (1900 - 2030): ";
            cin >> p->anioUso;
            gestor.guardar();
            cout << "Pregunta actualizada.\n";
            break;
        }
        case 5:
        {
            int id;
            cout << "ID de la pregunta a eliminar: ";
            cin >> id;
            int idx = gestor.buscarIndicePorId(id);
            if (idx == -1)
            {
                cout << "Pregunta no encontrada.\n";
                break;
            }
            delete gestor.banco[idx];
            for (int i = idx; i < gestor.cantidad - 1; ++i)
                gestor.banco[i] = gestor.banco[i + 1];
            --gestor.cantidad;
            gestor.guardar();
            cout << "Pregunta eliminada.\n";
            break;
        }
        case 6:
            generarEvaluacion(gestor);
            break;
        }
    } while (opcion != 0); cout << "\n- Se ha finalizado el programa." << endl; // Agregue un mensaje para dejar claridad que se cerró el programa. (Benjamin)
    return 0;
}
