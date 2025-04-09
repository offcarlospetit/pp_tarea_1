classDiagram

class PreguntaBase {
  +int id
  +string enunciado
  +int nivel
  +int tiempoEstimado
  +string respuestaCorrecta
  +int anioUso
  +mostrar() void
  +tipo() string
}

class PreguntaMultiple {
  +mostrar() void
  +tipo() string
  +opciones() [string]
}

class PreguntaVF {
  +mostrar() void
  +tipo() string
}

class PreguntaEnsayo {
    +mostrar() void
    +tipo() string
}

class PreguntaCorta {
    +mostrar() void
    +tipo() string
}

class PreguntaIncompleta {
    +mostrar() void
    +tipo() string
}

class PreguntaEmparejamiento{
    +mostrar() void
    +tipo() string
    +correspondencia() [string]
}

class PreguntasAnalogiasDiferencias{
    +mostrar() void
    +tipo() string
}

class PreguntasInterpretacion{
    +mostrar() void
    +tipo() string
}

class GestorPreguntas {
  +PreguntaBase* banco[1000]
  +int cantidad
  +string archivo
  +crearPregunta() void
  +listar() void
  +guardar() void
  +cargar() void
  +buscarIndicePorId(int) int
  +getPregunta(int) PreguntaBase*
  +getCantidad() int
}

class Evaluacion {
  +string asignatura
  +int anio
  +PreguntaBase* preguntas[100]
  +agregarPregunta(PreguntaBase*) void
  +mostrar() void
  +tiempoTotal() int
}

PreguntaEnsayo --|> PreguntaBase
PreguntaCorta --|> PreguntaBase
PreguntasInterpretacion --|> PreguntaBase
PreguntasAnalogiasDiferencias --|> PreguntaBase
PreguntaEmparejamiento --|> PreguntaBase
PreguntaEnsayo --|> PreguntaBase
PreguntaMultiple --|> PreguntaBase
PreguntaVF --|> PreguntaBase

GestorPreguntas o-- "1..*" PreguntaBase
Evaluacion o-- "1..*" PreguntaBase
