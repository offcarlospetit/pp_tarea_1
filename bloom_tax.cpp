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