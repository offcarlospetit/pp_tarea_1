classDiagram
    class PreguntaBase {
        +int id
        +string enunciado
        +int nivel
        +int tiempoEstimado
        +string respuestaCorrecta
        +int anioUso
        +mostrar() : void
        +tipo() : string
        +crearDesdeConsola() : void
        +guardar(out) : void
        +cargar(ss) : void
    }

    class PreguntaMultiple
    class PreguntaVF
    class PreguntaEnsayo
    class PreguntaCorta
    class PreguntaTextoIncompleto
    class PreguntaEmparejamiento
    class PreguntaAnalogias
    class PreguntaGrafico

    class GestorPreguntas {
        -PreguntaBase* banco[1000]
        -int cantidad
        -string archivo
        +crearPregunta() : void
        +guardar() : void
        +cargar() : void
        +listar() : void
    }

    class Evaluacion {
        +string asignatura
        +int anio
        +PreguntaBase* preguntas[100]
        +agregarPregunta(p: PreguntaBase) : void
        +tiempoTotal() : int
        +mostrar() : void
    }

    PreguntaBase <|-- PreguntaMultiple
    PreguntaBase <|-- PreguntaVF
    PreguntaBase <|-- PreguntaEnsayo
    PreguntaBase <|-- PreguntaCorta
    PreguntaBase <|-- PreguntaTextoIncompleto
    PreguntaBase <|-- PreguntaEmparejamiento
    PreguntaBase <|-- PreguntaAnalogias
    PreguntaBase <|-- PreguntaGrafico

    GestorPreguntas --> PreguntaBase
    Evaluacion --> PreguntaBase