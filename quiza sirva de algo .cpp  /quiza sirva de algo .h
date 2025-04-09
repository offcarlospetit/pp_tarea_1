// Pregunta.h
#ifndef PREGUNTA_H
#define PREGUNTA_H

#include <string>

class Pregunta {
protected:
    int id;
    std::string enunciado;
    std::string nivelBloom;
    int tiempoEstimado;

public:
    Pregunta(int id, std::string enunciado, std::string nivelBloom, int tiempo);
    virtual void mostrar() const = 0;
    virtual void editar();
    std::string getNivelBloom() const;
    int getTiempoEstimado() const;
    int getId() const;
    virtual ~Pregunta() {}
};

#endif

// Pregunta.cpp
#include "Pregunta.h"
#include <iostream>

Pregunta::Pregunta(int id, std::string enunciado, std::string nivelBloom, int tiempo)
    : id(id), enunciado(enunciado), nivelBloom(nivelBloom), tiempoEstimado(tiempo) {}

void Pregunta::editar() {
    std::cout << "Nuevo enunciado: ";
    std::getline(std::cin >> std::ws, enunciado);
    std::cout << "Nuevo nivel Bloom: ";
    std::getline(std::cin >> std::ws, nivelBloom);
    std::cout << "Nuevo tiempo estimado: ";
    std::cin >> tiempoEstimado;
}

std::string Pregunta::getNivelBloom() const {
    return nivelBloom;
}

int Pregunta::getTiempoEstimado() const {
    return tiempoEstimado;
}

int Pregunta::getId() const {
    return id;
}

// PreguntaOM.h
#ifndef PREGUNTAOM_H
#define PREGUNTAOM_H

#include "Pregunta.h"
#include <vector>

class PreguntaOM : public Pregunta {
private:
    std::vector<std::string> opciones;
    std::string respuestaCorrecta;

public:
    PreguntaOM(int id, std::string enunciado, std::string nivel, int tiempo,
               std::vector<std::string> opciones, std::string respuesta);
    void mostrar() const override;
};

#endif

// PreguntaOM.cpp
#include "PreguntaOM.h"
#include <iostream>

PreguntaOM::PreguntaOM(int id, std::string enunciado, std::string nivel, int tiempo,
                       std::vector<std::string> opciones, std::string respuesta)
    : Pregunta(id, enunciado, nivel, tiempo), opciones(opciones), respuestaCorrecta(respuesta) {}

void PreguntaOM::mostrar() const {
    std::cout << "[OM] ID: " << id << " | " << enunciado << "\n";
    for (size_t i = 0; i < opciones.size(); ++i)
        std::cout << char('A' + i) << ") " << opciones[i] << "\n";
    std::cout << "Respuesta correcta: " << respuestaCorrecta << "\n";
}

// PreguntaVF.h
#ifndef PREGUNTAVF_H
#define PREGUNTAVF_H

#include "Pregunta.h"

class PreguntaVF : public Pregunta {
private:
    bool respuestaCorrecta;

public:
    PreguntaVF(int id, std::string enunciado, std::string nivel, int tiempo, bool respuesta);
    void mostrar() const override;
};

#endif

// PreguntaVF.cpp
#include "PreguntaVF.h"
#include <iostream>

PreguntaVF::PreguntaVF(int id, std::string enunciado, std::string nivel, int tiempo, bool respuesta)
    : Pregunta(id, enunciado, nivel, tiempo), respuestaCorrecta(respuesta) {}

void PreguntaVF::mostrar() const {
    std::cout << "[VF] ID: " << id << " | " << enunciado << "\n";
    std::cout << "Respuesta: " << (respuestaCorrecta ? "Verdadero" : "Falso") << "\n";
}

// BancoPreguntas.h
#ifndef BANCOPREGUNTAS_H
#define BANCOPREGUNTAS_H

#include "Pregunta.h"
#include "PreguntaOM.h"
#include "PreguntaVF.h"
#include <vector>
#include <memory>

class BancoPreguntas {
private:
    std::vector<std::shared_ptr<Pregunta>> preguntas;
    int contadorId;

public:
    BancoPreguntas();
    void agregarPreguntaInteractiva();
    void eliminarPregunta();
    void actualizarPregunta();
    void buscarPorNivel();
    void mostrarTodas() const;
    std::vector<std::shared_ptr<Pregunta>> obtenerPorNivel(std::string nivel, int cantidad);
};

#endif

// BancoPreguntas.cpp
#include "BancoPreguntas.h"
#include <iostream>

BancoPreguntas::BancoPreguntas() : contadorId(1) {}

void BancoPreguntas::agregarPreguntaInteractiva() {
    int tipo, tiempo;
    std::string enunciado, nivel, respuesta;
    std::cout << "Tipo (1 = OM, 2 = VF): "; std::cin >> tipo;
    std::cin.ignore();
    std::cout << "Enunciado: "; std::getline(std::cin, enunciado);
    std::cout << "Nivel Bloom: "; std::getline(std::cin, nivel);
    std::cout << "Tiempo estimado (min): "; std::cin >> tiempo;

    if (tipo == 1) {
        std::vector<std::string> opciones(4);
        std::cin.ignore();
        for (int i = 0; i < 4; ++i) {
            std::cout << "Opcion " << i+1 << ": "; std::getline(std::cin, opciones[i]);
        }
        std::cout << "Respuesta correcta: "; std::getline(std::cin, respuesta);
        preguntas.push_back(std::make_shared<PreguntaOM>(contadorId++, enunciado, nivel, tiempo, opciones, respuesta));
    } else {
        bool vf;
        std::cout << "Respuesta correcta (1 = Verdadero, 0 = Falso): "; std::cin >> vf;
        preguntas.push_back(std::make_shared<PreguntaVF>(contadorId++, enunciado, nivel, tiempo, vf));
    }
}

void BancoPreguntas::eliminarPregunta() {
    int id;
    std::cout << "ID a eliminar: "; std::cin >> id;
    preguntas.erase(std::remove_if(preguntas.begin(), preguntas.end(),
        [id](const std::shared_ptr<Pregunta>& p) { return p->getId() == id; }), preguntas.end());
}

void BancoPreguntas::actualizarPregunta() {
    int id;
    std::cout << "ID a actualizar: "; std::cin >> id;
    for (auto& p : preguntas) {
        if (p->getId() == id) {
            p->editar();
            break;
        }
    }
}

void BancoPreguntas::buscarPorNivel() {
    std::string nivel;
    std::cin.ignore();
    std::cout << "Nivel Bloom: "; std::getline(std::cin, nivel);
    for (const auto& p : preguntas) {
        if (p->getNivelBloom() == nivel)
            p->mostrar();
    }
}

void BancoPreguntas::mostrarTodas() const {
    for (const auto& p : preguntas) p->mostrar();
}

std::vector<std::shared_ptr<Pregunta>> BancoPreguntas::obtenerPorNivel(std::string nivel, int cantidad) {
    std::vector<std::shared_ptr<Pregunta>> seleccionadas;
    for (const auto& p : preguntas) {
        if (p->getNivelBloom() == nivel && seleccionadas.size() < (size_t)cantidad)
            seleccionadas.push_back(p);
    }
    return seleccionadas;
}

// Evaluacion.h
#ifndef EVALUACION_H
#define EVALUACION_H

#include "BancoPreguntas.h"

class Evaluacion {
private:
    BancoPreguntas& banco;

public:
    Evaluacion(BancoPreguntas& banco);
    void generarEvaluacion();
};

#endif

// Evaluacion.cpp
#include "Evaluacion.h"
#include <iostream>

Evaluacion::Evaluacion(BancoPreguntas& banco) : banco(banco) {}

void Evaluacion::generarEvaluacion() {
    std::string nivel;
    int cantidad;
    std::cin.ignore();
    std::cout << "Nivel Bloom a evaluar: "; std::getline(std::cin, nivel);
    std::cout << "Cantidad de preguntas: "; std::cin >> cantidad;
    auto preguntas = banco.obtenerPorNivel(nivel, cantidad);

    int totalTiempo = 0;
    for (const auto& p : preguntas) {
        p->mostrar();
        totalTiempo += p->getTiempoEstimado();
    }

    std::cout << "Tiempo estimado total: " << totalTiempo << " minutos\n";
}

// main.cpp
#include "BancoPreguntas.h"
#include "Evaluacion.h"
#include <iostream>

int main() {
    BancoPreguntas banco;
    Evaluacion eval(banco);
    int op;
    do {
        std::cout << "\n--- MENU ---\n1. Agregar\n2. Actualizar\n3. Eliminar\n4. Buscar por nivel\n5. Mostrar todas\n6. Generar evaluación\n7. Salir\nOpcion: ";
        std::cin >> op;
        switch (op) {
            case 1: banco.agregarPreguntaInteractiva(); break;
            case 2: banco.actualizarPregunta(); break;
            case 3: banco.eliminarPregunta(); break;
            case 4: banco.buscarPorNivel(); break;
            case 5: banco.mostrarTodas(); break;
            case 6: eval.generarEvaluacion(); break;
        }
    } while (op != 7);
    return 0;
}
