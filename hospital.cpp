#include <iostream>
#include <cstring>
#include <ctime>
#include <iosmanip>
using namespace std;

#ifndef nullptr
#define nullptr NULL
#endif

struct HistorialMedico {
    int id;
    char fecha [11];
    char diagnostico [100];
    char tratamiento [100];
    int idDoctor:
    float costo;
};

struct Paciente {
    int id;
    char nombre [50];
    char cedula [20];
    int edad;
    historialMedico * historial ;
    int numConsultas;
    int capacidadHistorial ;
    bool activo;
};

struct Doctor {
    int id;
    char nombre [50];
    char especialidad [30];
    float costoConsulta ;
    bool disponible ;
};

struct hospital {
    char nombre [100];
    Paciente * pacientes ;
    Doctor * doctores ;
    Cita * citas ;
    int cantPacientes , cantDoctores , cantCitas ;
    int maxPacientes , maxDoctores , maxCitas ;
    int sigIdPaciente , sigIdDoctor , sigIdCita , sigIdConsulta;
};

void redimensionarPacientes (Hospital * h);
void redimensionarDoctores (Hospital * h);
void redimensionarCitas (Hospital * h);
void redimensionarHistorial (Paciente * p);

Hospital * crearHospital (const char * nombre );
void destruirHospital (Hospital * h);
void registrarPaciente (Hospital * h);
void registrarDoctor (Hospital * h);
void agendarCita (Hospital * h);
void atenderCita (Hospital * h);

Paciente * buscarPacientePorId (Hospital * h, int id);
Doctor * buscarDoctorPorId (Hospital * h, int id);

void mostrarHistorial (Paciente * p);
void listarCitasPendientes (Hospital * h);
void menuCitas (Hospital * h);

void redimensionarPacientes (Hospital * h) {
    int nuevaCapacidad = h->maxPacientes * 2;
    paciente * nuevoArray = new Paciente [nuevaCapacidad];
    for (int i = 0; i < h->cantPacientes ; i++){
        nuevoArray [i] = h->pacientes [i];
    }

    delete [] h->pacientes ;

    h->pacientes = nuevoArray ;
    h->maxPacientes = nuevoCapacidad;

    cout << "?? Array de pacientes redimensionado a " << nuevaCapacidad << " espacios.\n";
}
void redimensionarDoctores (Hospital * h) {
    int nuevaCapacidad = h->maxDoctores * 2;
    Doctor * nuevoArray = new Doctor [nuevaCapacidad ];
    for (int i = 0; i < h->cantDoctores ; i++) {
        nuevoArray [i] = h->doctores [i];
    }
    delete [] h->doctores ;
    h->doctores = nuevoArray ;
    h->maxDoctores = nuevaCapacidad ;
    cout << "?? Array de doctores redimensionado a " << nuevaCapacidad << " espacios.\n";
}

void redimensionarHistorial (Paciente * p){
    int nuevaCapacidad = p->capacidadHistorial * 2;
    HistorialMedico * nuevoArray = new HistorialMedico [nuevaCapacidad];

    for (int i = 0; i < p->numConsultas ; i++) {
        nuevoArray [i] = p->historial [i];
    }
    
    delete [] p->historial ;
    p->historial = nuevoArray ;
    p->capacidadHistorial = nuevaCapacidad ;

    cout << "?? Historial medico redimensionado a " << nuevaCapacidad << " consultas.\n";
}

Hospital * crearHospital(const chr * nombre ){
    Hospital * h = new Hospital ;
    strcpy(h->nombre, nombre);

    h->maxPacientes = 10;
    h->maxDoctores = 10;
    h->maxCitas = 20;

    h->pacientes = new Paciente[h->maxPacientes ];
    h->doctores = new Doctor[h->maxDoctores ];
    h->citas = new cita[h->maxCitas ];

    h->cantPacientes = 0;
    h->cantDoctores = 0;
    h->cantCitas = 0;
    h->sigIdPaciente = 1;
    h->sigIdDoctor = 1;
    h->sigIdCita = 1;
    h->sigIdConsulta = 1;

    return h;
}