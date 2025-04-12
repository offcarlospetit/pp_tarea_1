// evaluacion_bloom_refactor.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Año de uso: ";
        cin >> anioUso;
        cin.ignore();

        cout << "Cantidad de opciones (máx 10): ";
        cin >> cantidadOpciones;
        cin.ignore();

        for (int i = 0; i < cantidadOpciones; ++i)
        {
            cout << "Opción " << static_cast<char>('A' + i) << ": ";
            getline(cin, opciones[i]);
        }

        cout << "Índice de la opción correcta (0 a " << cantidadOpciones - 1 << "): ";
        cin >> indiceCorrecto;
        cin.ignore();

        if (indiceCorrecto >= 0 && indiceCorrecto < cantidadOpciones)
            respuestaCorrecta = opciones[indiceCorrecto];
    }

    void guardar(ofstream &out) const override
    {
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << cantidadOpciones << "," << indiceCorrecto;

        for (int i = 0; i < cantidadOpciones; ++i)
        {
            out << "," << opciones[i];
        }
        out << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, temp, ',');
        cantidadOpciones = stoi(temp);
        getline(ss, temp, ',');
        indiceCorrecto = stoi(temp);

        for (int i = 0; i < cantidadOpciones; ++i)
        {
            getline(ss, opciones[i], ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Año de uso: ";
        cin >> anioUso;
        cin.ignore();

        int valor;
        cout << "¿Es Verdadero (1) o Falso (0)? ";
        cin >> valor;
        cin.ignore();
        esVerdadero = (valor == 1);
        respuestaCorrecta = esVerdadero ? "Verdadero" : "Falso";

        cout << "Justificación de la respuesta: ";
        getline(cin, justificacion);
    }

    void guardar(ofstream &out) const override
    {
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << (esVerdadero ? "1" : "0") << "," << justificacion << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, temp, ',');
        esVerdadero = (temp == "1");
        getline(ss, justificacion, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Respuesta esperada: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso: ";
        cin >> anioUso;
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
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << contexto << "," << criteriosEvaluacion << "," << extensionMinima << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, contexto, ',');
        getline(ss, criteriosEvaluacion, ',');
        getline(ss, temp, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Respuesta Correcta: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso: ";
        cin >> anioUso;
        cin.ignore();
        cout << "Tipo de dato esperado (ej: palabra, número, fecha): ";
        getline(cin, tipoEsperado);
    }

    void guardar(ofstream &out) const override
    {
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << tipoEsperado << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, tipoEsperado, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Respuesta Correcta: ";
        getline(cin, respuestaCorrecta);
        cout << "Año de uso: ";
        cin >> anioUso;
        cin.ignore();
        cout << "Texto plantilla con espacio a completar (use ___ o ...): ";
        getline(cin, plantilla);
    }

    void guardar(ofstream &out) const override
    {
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << plantilla << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, plantilla, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Año de uso: ";
        cin >> anioUso;
        cin.ignore();
        cout << "Cantidad de pares (máximo 10): ";
        cin >> cantidadPares;
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
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
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
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, temp, ',');
        cantidadPares = stoi(temp);

        for (int i = 0; i < cantidadPares; ++i)
        {
            pares[i].id = i;
            getline(ss, pares[i].conceptoA, ',');
            getline(ss, pares[i].conceptoB, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Año de uso: ";
        cin >> anioUso;
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
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << parReferenciaA << "," << parReferenciaB << "," << parIncompletoA << ","
            << parEsperadoB << "," << tipoRelacion << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, parReferenciaA, ',');
        getline(ss, parReferenciaB, ',');
        getline(ss, parIncompletoA, ',');
        getline(ss, parEsperadoB, ',');
        getline(ss, tipoRelacion, ',');
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
        cout << "ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enunciado: ";
        getline(cin, enunciado);
        cout << "Nivel Bloom (0-5): ";
        cin >> nivel;
        cout << "Tiempo estimado (min): ";
        cin >> tiempoEstimado;
        cin.ignore();
        cout << "Año de uso: ";
        cin >> anioUso;
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
        out << id << "," << enunciado << "," << tipo() << "," << nivelToStr(nivel) << ","
            << tiempoEstimado << "," << respuestaCorrecta << "," << anioUso << ","
            << tipoGrafico << "," << descripcionRecurso << "," << preguntaRelacionada << "\n";
    }

    void cargar(stringstream &ss) override
    {
        string temp;
        getline(ss, enunciado, ',');
        getline(ss, temp, ',');
        nivel = strToNivel(temp);
        getline(ss, temp, ',');
        tiempoEstimado = stoi(temp);
        getline(ss, respuestaCorrecta, ',');
        getline(ss, temp, ',');
        anioUso = stoi(temp);
        getline(ss, tipoGrafico, ',');
        getline(ss, descripcionRecurso, ',');
        getline(ss, preguntaRelacionada, ',');
    }
};

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
            string temp, tipoStr;

            getline(ss, temp, ','); // ID
            int id = stoi(temp);
            getline(ss, temp, ','); // enunciado (lo volveremos a leer luego)
            getline(ss, tipoStr, ',');

            PreguntaBase *p = nullptr;
            if (tipoStr == "Opcion Multiple")
                p = new PreguntaMultiple();
            else if (tipoStr == "Verdadero/Falso")
                p = new PreguntaVF();
            else if (tipoStr == "Ensayo")
                p = new PreguntaEnsayo();
            else if (tipoStr == "Respuesta Corta")
                p = new PreguntaCorta();
            else if (tipoStr == "Texto Incompleto")
                p = new PreguntaTextoIncompleto();
            else if (tipoStr == "Emparejamiento")
                p = new PreguntaEmparejamiento();
            else if (tipoStr == "Analogías")
                p = new PreguntaAnalogias();
            else if (tipoStr == "Grafico")
                p = new PreguntaGrafico();

            if (p)
            {
                ss.clear();
                ss.str(linea);
                getline(ss, temp, ',');
                p->id = stoi(temp);
                p->cargar(ss);
                banco[cantidad++] = p;
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

        cout << "Tipo de pregunta:\n"
             << "1. Opción Múltiple\n"
             << "2. Verdadero/Falso\n"
             << "3. Ensayo\n"
             << "4. Respuesta Corta\n"
             << "5. Texto Incompleto\n"
             << "6. Emparejamiento\n"
             << "7. Analogías\n"
             << "8. Gráfico\n> ";
        int tipo;
        cin >> tipo;
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
        cout << "Pregunta creada correctamente.\n";
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
    cin >> eval.anio;
    cout << "Cantidad de preguntas a incluir: ";
    int num;
    cin >> num;

    for (int i = 0; i < num; ++i)
    {
        cout << "Nivel Bloom de la pregunta " << (i + 1) << " (0-5): ";
        int nivelInt;
        cin >> nivelInt;

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
    do
    {
        cout << "\n1. Crear pregunta\n2. Listar preguntas\n3. Buscar por nivel\n4. Editar pregunta\n5. Eliminar pregunta\n6. Generar evaluación\n0. Salir\n> ";
        cin >> opcion;
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
            cout << "Nivel Bloom (0-5): ";
            cin >> p->nivel;
            cout << "Tiempo estimado (min): ";
            cin >> p->tiempoEstimado;
            cin.ignore();
            cout << "Respuesta Correcta: ";
            getline(cin, p->respuestaCorrecta);
            cout << "Año de uso: ";
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
    } while (opcion != 0);
    return 0;
}
