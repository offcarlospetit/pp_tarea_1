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


