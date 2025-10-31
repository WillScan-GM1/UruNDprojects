#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
using namespace std;

#ifndef nullptr
#define nullptr NULL
#endif

struct HistorialMedico {
    int id;
    char fecha[11];
    char diagnostico[100];
    char tratamiento[100];
    int idDoctor;
    float costo;
};

struct Paciente {
    int id;
    char nombre[50];
    char cedula[20];
    int edad;
    HistorialMedico * historial;
    int numConsultas;
    int capacidadHistorial;
    bool activo;
};

struct Doctor {
    int id;
    char nombre[50];
    char especialidad[30];
    float costoConsulta;
    bool disponible;
};

struct Cita {
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char estado[20];
    bool atendida;
};

struct Hospital {
    char nombre[100];
    Paciente * pacientes;
    Doctor * doctores;
    Cita * citas;
    int cantPacientes, cantDoctores, cantCitas;
    int maxPacientes, maxDoctores, maxCitas;
    int sigIdPaciente, sigIdDoctor, sigIdCita, sigIdConsulta;
};

void redimensionarPacientes(Hospital * h);
void redimensionarDoctores(Hospital * h);
void redimensionarCitas(Hospital * h);
void redimensionarHistorial(Paciente * p);

Hospital * crearHospital(const char * nombre);
void destruirHospital(Hospital * h);
void registrarPaciente(Hospital * h);
void registrarDoctor(Hospital * h);
void agendarCita(Hospital * h);
void atenderCita(Hospital * h);

Paciente * buscarPacientePorId(Hospital * h, int id);
Doctor * buscarDoctorPorId(Hospital * h, int id);

void mostrarHistorial(Paciente * p);
void listarCitasPendientes(Hospital * h);
void menuPacientes(Hospital * h);
void menuDoctores(Hospital * h);
void menuCitas(Hospital * h);

void redimensionarPacientes(Hospital * h) {
    int nuevaCapacidad = h->maxPacientes * 2;
    Paciente * nuevoArray = new Paciente[nuevaCapacidad];
    for (int i = 0; i < h->cantPacientes; i++){
        nuevoArray[i] = h->pacientes[i];
    }

    delete [] h->pacientes;

    h->pacientes = nuevoArray;
    h->maxPacientes = nuevaCapacidad;

    cout << "Array de pacientes redimensionado a " << nuevaCapacidad << " espacios.\n";
}

void redimensionarDoctores(Hospital * h) {
    int nuevaCapacidad = h->maxDoctores * 2;
    Doctor * nuevoArray = new Doctor[nuevaCapacidad];
    for (int i = 0; i < h->cantDoctores; i++) {
        nuevoArray[i] = h->doctores[i];
    }
    delete [] h->doctores;
    h->doctores = nuevoArray;
    h->maxDoctores = nuevaCapacidad;
    cout << "Array de doctores redimensionado a " << nuevaCapacidad << " espacios.\n";
}

void redimensionarCitas(Hospital * h) {
    int nuevaCapacidad = h->maxCitas * 2;
    Cita * nuevoArray = new Cita[nuevaCapacidad];
    for (int i = 0; i < h->cantCitas; i++) {
        nuevoArray[i] = h->citas[i];
    }
    delete [] h->citas;
    h->citas = nuevoArray;
    h->maxCitas = nuevaCapacidad;
    cout << "Array de citas redimensionado a " << nuevaCapacidad << " espacios.\n";
}

void redimensionarHistorial(Paciente * p){
    int nuevaCapacidad = p->capacidadHistorial * 2;
    HistorialMedico * nuevoArray = new HistorialMedico[nuevaCapacidad];

    for (int i = 0; i < p->numConsultas; i++) {
        nuevoArray[i] = p->historial[i];
    }
    
    delete [] p->historial;
    p->historial = nuevoArray;
    p->capacidadHistorial = nuevaCapacidad;

    cout << "Historial medico redimensionado a " << nuevaCapacidad << " consultas.\n";
}

Hospital * crearHospital(const char * nombre){
    Hospital * h = new Hospital;
    strcpy(h->nombre, nombre);

    h->maxPacientes = 10;
    h->maxDoctores = 10;
    h->maxCitas = 20;

    h->pacientes = new Paciente[h->maxPacientes];
    h->doctores = new Doctor[h->maxDoctores];
    h->citas = new Cita[h->maxCitas];

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
    for (int i = 0; i < h->cantPacientes; i++) {
        delete [] h->pacientes[i].historial;
    }
    delete [] h->pacientes;
    delete [] h->doctores;
    delete [] h->citas;
    delete h;
}

void registrarPaciente(Hospital * h) {
    if (h->cantPacientes >= h->maxPacientes){
        redimensionarPacientes(h);
    }

    Paciente p;
    p.id = h->sigIdPaciente++;
    cout << "Nombre: "; cin.ignore(); cin.getline(p.nombre, 50);
    cout << "Cedula: "; cin.getline(p.cedula, 20);
    cout << "Edad: "; cin >> p.edad;
    p.activo = true;
    p.numConsultas = 0;
    p.capacidadHistorial = 5;
    p.historial = new HistorialMedico[5];

    h->pacientes[h->cantPacientes] = p;
    h->cantPacientes++;
    cout << "Paciente registrado con ID: " << p.id << "\n";
}

void registrarDoctor(Hospital * h) {
    if (h->cantDoctores >= h->maxDoctores) {
        redimensionarDoctores(h);
    }

    Doctor d;
    d.id = h->sigIdDoctor++;
    cout << "Nombre: "; cin.ignore(); cin.getline(d.nombre, 50);
    cout << "Especialidad: "; cin.getline(d.especialidad, 30);
    cout << "Costo consulta: "; cin >> d.costoConsulta;
    d.disponible = true;

    h->doctores[h->cantDoctores] = d;
    h->cantDoctores++;
    cout << "Doctor registrado con ID: " << d.id << "\n";
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
        cout << "Paciente o doctor no encontrado.\n";
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
    cout << "Cita agendada con ID: " << c.id << "\n";
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
                cout << "Error al cargar datos.\n";
                return;
            }
            
            if (p->numConsultas >= p->capacidadHistorial) {
                redimensionarHistorial(p);
            }
            
            HistorialMedico hist;
            hist.id = h->sigIdConsulta++;
            strcpy(hist.fecha, c->fecha);
            cout << "Diagnostico: "; cin.ignore(); cin.getline(hist.diagnostico, 100);
            cout << "Tratamiento: "; cin.getline(hist.tratamiento, 100);
            hist.idDoctor = d->id;
            hist.costo = d->costoConsulta;
            
            p->historial[p->numConsultas] = hist;
            p->numConsultas++;
            
            strcpy(c->estado, "Atendida");
            c->atendida = true;
            cout << "Cita atendida y registrada en historial.\n";
            return;
        }
    }
    cout << "Cita no encontrada o ya atendida.\n";
}

Paciente * buscarPacientePorId(Hospital * h, int id) {
    for (int i = 0; i < h->cantPacientes; i++) {
        if (h->pacientes[i].id == id){
            return &h->pacientes[i];
        }
    }
    return nullptr;
}

Doctor * buscarDoctorPorId(Hospital * h, int id) {
    for (int i = 0; i < h->cantDoctores; i++) {
        if (h->doctores[i].id == id){
            return &h->doctores[i];
        }
    }
    return nullptr;
}

void mostrarHistorial(Paciente * p) {
    if (p->numConsultas == 0) {
        cout << "No hay historial medico.\n";
        return;
    }
    cout << "\nHISTORIAL DE " << p->nombre << ":\n";
    cout << "ID | Fecha       | Diagnostico\n";
    cout << "-------------------------------\n";
    for (int i = 0; i < p->numConsultas; i++) {
        cout << p->historial[i].id << "  | "
             << p->historial[i].fecha << " | "
             << p->historial[i].diagnostico << "\n";
    }
}

void listarCitasPendientes(Hospital * h) {
    cout << "\nCITAS PENDIENTES:\n";
    cout << "ID | Paciente | Doctor | Fecha       \n";
    cout << "---------------------------------------------\n";
    for (int i = 0; i < h->cantCitas; i++) {
        if (strcmp(h->citas[i].estado, "Agendada") == 0) {
            cout << h->citas[i].id << "  | "
                 << h->citas[i].idPaciente << "        | "
                 << h->citas[i].idDoctor << "      | "
                 << h->citas[i].fecha << "\n";
        }
    }
}

void menuPacientes(Hospital* h) {
    int op;
    do {
        cout << "\n--- GESTION DE PACIENTES ---\n";
        cout << "1. Registrar paciente\n";
        cout << "2. Ver historial medico\n";
        cout << "3. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        if (op == 1) registrarPaciente(h);
        else if (op == 2) {
            int id; cout << "ID: "; cin >> id;
            Paciente* p = buscarPacientePorId(h, id);
            if (p) mostrarHistorial(p);
            else cout << "No encontrado.\n";
        }
    } while (op != 3);
}

void menuDoctores(Hospital* h) {
    int op;
    do {
        cout << "\n--- GESTION DE DOCTORES ---\n";
        cout << "1. Registrar doctor\n";
        cout << "2. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        if (op == 1) registrarDoctor(h);
    } while (op != 2);
}

void menuCitas(Hospital* h) {
    int op;
    do {
        cout << "\n--- GESTION DE CITAS ---\n";
        cout << "1. Agendar cita\n";
        cout << "2. Atender cita\n";
        cout << "3. Ver pendientes\n";
        cout << "4. Volver\n";
        cout << "Opcion: ";
        cin >> op;
        if (op == 1) agendarCita(h);
        else if (op == 2) atenderCita(h);
        else if (op == 3) listarCitasPendientes(h);
    } while (op != 4);
}

int main() {
    Hospital* h = crearHospital("Hospital Urdaneta");
    int op;
    do {
        cout << "\n+----------------------------+\n";
        cout << "|     SISTEMA HOSPITALARIO   |\n";
        cout << "+----------------------------+\n";
        cout << "1. Pacientes\n2. Doctores\n3. Citas\n4. Salir\nOpcion: ";
        cin >> op;
        if (op == 1) menuPacientes(h);
        else if (op == 2) menuDoctores(h);
        else if (op == 3) menuCitas(h);
    } while (op != 4);
    
    destruirHospital(h);
    cout << "Sistema cerrado correctamente.\n";
    return 0;
}