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

void destruirHospital(Hospital * h) {
    if (h->cantPacientes >= h->maxPacientes){
        redimensionarPacientes (h);
    }

    Paciente p ;
    p. id = h->sigIdPaciente ++;
    cout << "Nombre: " ; cin >> p.nombre ;
    cout << "Cedula: " ; cin >> p.cedula ;
    cout << "Edad: " ; cin >> p.edad;
    p.activo = true;
    p.numConsultas = 0;
    p.capacidadHistorial = 5;
    p.historial = new HistorialMedico [5];

    h->pacientes [h->cantPacientes ] = p;
    h->cantPacientes ++;
    cout << "? Paciente registrado con ID: " << p.id << "\n";
}

void agendarCita(Hospital* h) {
    if (h->cantCitas >= h->maxCitas) {
        redimensionarCitas(h);
    }
    int idP, idD;
    cout << "ID Paciente: "; cin >> idP;
    cout << "ID Doctor: "; cin >> idD;
    
    Paciente* p = buscarPacientePorId(h, idP);
    Doctor* d = buscarDoctorPorId(h, idD);
    if (!p || !d) {
        cout << "? Paciente o doctor no encontrado.\n";
        return;
    }
    
    Cita c;
    c.id = h->sigIdCita++;
    c.idPaciente = idP;
    c.idDoctor = idD;
    cout << "Fecha (YYYY-MM-DD): "; cin >> c.fecha;
    cout << "Hora (HH:MM): "; cin >> c.hora;
    strcpy(c.estado, "Agendada");
    c.atendida = false;
    
    h->citas[h->cantCitas] = c;
    h->cantCitas++;
    cout << "? Cita agendada con ID: " << c.id << "\n";
}

void atenderCita(Hospital* h) {
    int id;
    cout << "ID de la cita a atender: "; cin >> id;
    
    for (int i = 0; i < h->cantCitas; i++) {
        if (h->citas[i].id == id && strcmp(h->citas[i].estado, "Agendada") == 0) {
            Cita* c = &h->citas[i];
            Paciente* p = buscarPacientePorId(h, c->idPaciente);
            Doctor* d = buscarDoctorPorId(h, c->idDoctor);
            
            if (!p || !d) {
                cout << "? Error al cargar datos.\n";
                return;
            }
            
            if (p->numConsultas >= p->capacidadHistorial) {
                redimensionarHistorial(p);
            }
            
            HistorialMedico hist;
            hist.id = h->sigIdConsulta++;
            strcpy(hist.fecha, c->fecha);
            cout << "Diagnostico: "; cin >> hist.diagnostico;
            cout << "Tratamiento: "; cin >> hist.tratamiento;
            hist.idDoctor = d->id;
            hist.costo = d->costoConsulta;
            
            p->historial[p->numConsultas] = hist;
            p->numConsultas++;
            
            strcpy(c->estado, "Atendida");
            c->atendida = true;
            cout << "? Cita atendida y registrada en historial.\n";
            return;
        }
    }
    cout << "? Cita no encontrada o ya atendida.\n";
}

Paciente * buscarPacientePorId (Hospital * h, int id) {
    for (int i = 0; i < h->cantPacientes; i++) {
        if (h->pacientes [i].id == id){
            return &h->pacientes [i];
        }
    }
    return nullptr ;
}

Doctor * buscarDoctorPorId (Hospital * h, int id) {
    for (int i = 0; i < h->cantDoctores; i++) {
        if (h->doctores [i].id == id){
            return &h->doctores [i];
        }
    }
    return nullptr ;
}

void mostrarHistorial (Paciente * p) {
    if (p->numConsultas == 0) {
        cout << " No hay historial medico.\n";
        return;
    }
    cout << "\n?? HISTORIAL DE " << p->nombre << ":\n";
    cout << "ID | Fecha       | Diagnostico\n";
    cout << "-------------------------------\n";
    for (int i = 0; i < p->numConsultas; i++) {
        cout << p->historial[i].id << "  | "
             << p->historial[i].fecha << " | "
             << p->historial[i].diagnostico << "\n";
    }
}