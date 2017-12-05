/*
    main.cpp

    Copyright (C) 2013 by Don Cross  -  http://cosinekitty.com/raytrace

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the author be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
       distribution.

    -------------------------------------------------------------------------

    Main source file for my demo 3D ray-tracing image maker.
*/
#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "algebra.h"
#include "block.h"
#include "chessboard.h"
#include "planet.h"
#include "polyhedra.h"

void RotateZ(Imager::SolidObject *obj, double angleInDegrees) {
    const double angleInRadians = Imager::RadiansFromDegrees(angleInDegrees);
    const double a = cos(angleInRadians);
    const double b = sin(angleInRadians);
    double x,y,z,xG,yG,zG;
    x = obj->Center().x;
    y = obj->Center().y;
    z = obj->Center().z;
    xG = a*x - b*y;
    yG = a*y + b*x;
    zG = z;
    obj->RotateZ(angleInDegrees);
    obj->Move(xG,yG,zG);
}

void RotateY(Imager::SolidObject *obj, double angleInDegrees) {
    const double angleInRadians = Imager::RadiansFromDegrees(angleInDegrees);
    const double a = cos(angleInRadians);
    const double b = sin(angleInRadians);
    double x,y,z,xG,yG,zG;
    x = obj->Center().x;
    y = obj->Center().y;
    z = obj->Center().z;
    xG = a*x + b*z;
    zG = a*z - b*x;
    yG = y;
    obj->RotateY(angleInDegrees);
    obj->Move(xG,yG,zG);
}

void RotateX(Imager::SolidObject *obj, double angleInDegrees) {
    const double angleInRadians = Imager::RadiansFromDegrees(angleInDegrees);
    const double a = cos(angleInRadians);
    const double b = sin(angleInRadians);
    double x,y,z,xG,yG,zG;
    x = obj->Center().x;
    y = obj->Center().y;
    z = obj->Center().z;
    xG = x;
    yG = a*y - b*z;
    zG = a*z + b*y;
    obj->RotateX(angleInDegrees);
    obj->Move(xG,yG,zG);
}

std::vector<std::string> split_getline(std::string str, char delim) {
    std::vector<std::string> resultado;
    std::istringstream isstream(str);
    std::string palabra;

    while(std::getline(isstream, palabra, delim)){
        resultado.push_back(palabra);
    }
    return resultado;
}



void Escena() {
    using namespace Imager;
    std::string lineaLeida; /**< Lectura del archivo de informacion ->linea */


    std::vector<Color> colores; /**< Lista de colores, que se utilizaran en la escena, cargados desde archivos. */
    std::ifstream infoColores("Colores.txt");//informacion de los colores.
    while(std::getline(infoColores, lineaLeida)) {
        std::vector<std::string> RGB;
        RGB = split_getline(lineaLeida, ',');

        double rojo = atof(RGB[0].c_str());
        double verde = atof(RGB[1].c_str());
        double azul = atof(RGB[2].c_str());
        colores.push_back(Color(rojo, verde, azul));
    }

    /**< Escena de la mesa */
    std::vector<Cylinder*> patasDeMesa; //Lista de los objetos de la mesa (patas)
    std::ifstream infoPatasDeMesa("infoPatasDeMesa.txt"); //Informacion de las patas de la mesa.
    while(std::getline(infoPatasDeMesa, lineaLeida)) {
        std::vector<std::string> pata;
        pata = split_getline(lineaLeida, ',');
        double radio = atof(pata[0].c_str());
        double tamanno = atof(pata[1].c_str());
        double x_p = atof(pata[2].c_str());
        double y_p = atof(pata[3].c_str());
        double z_p = atof(pata[4].c_str());
        double rotacionPrimaria = atof(pata[5].c_str());
        Cylinder* pataObjeto = new Cylinder(radio, tamanno);
        pataObjeto->SetFullMatte(colores[6]);
        pataObjeto->Move(x_p, y_p, z_p);
        pataObjeto->RotateX(rotacionPrimaria);
        patasDeMesa.push_back(pataObjeto);
    }

    Scene scene(colores[0]); //Escena de fondo

    const double innerSize   = 0.0;
    const double xBorderSize = 50.0;
    const double yBorderSize = 50.0;
    const double thickness   = 0.50;


    //Mesa
    ChessBoard* mesa = new ChessBoard(
        0.0, 50.0, 50.0, 0.50, Color(0.24,0.16,0.11), Color(0.24,0.16,0.11), colores[6]);
    mesa->Move(0.0, -18.0, -280.0);
    mesa->RotateX(-90.0);
    mesa->SetMatteGlossBalance(0.5, Color(1.0, 1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0, 1.0));


    /**< Escena de las esferas. */
    std::vector<Sphere*> esferas; //Lista de los objetos
    std::ifstream infoDeEsferas("infoDeEsferas.txt"); //Informacion de las patas de la mesa.
    int cont = 0;
    while(std::getline(infoDeEsferas, lineaLeida)) {
        std::vector<std::string> esfera;
        esfera = split_getline(lineaLeida, ',');
        Sphere* objetoEsfera;
        if (cont == 0) {
            objetoEsfera = new Sphere(Vector(atof(esfera[0].c_str()),
                                             atof(esfera[1].c_str()),
                                             atof(esfera[2].c_str())),
                                      atof(esfera[3].c_str()));
            objetoEsfera->SetFullMatte(colores[1]);

        } else if (0<cont && cont < 3) {
            objetoEsfera = new Sphere(Vector(atof(esfera[0].c_str()),
                                             atof(esfera[1].c_str()),
                                             atof(esfera[2].c_str())),
                                      atof(esfera[3].c_str()));
            objetoEsfera->SetFullMatte(colores[2]);
            objetoEsfera->SetOpacity(atof(esfera[4].c_str()));
            objetoEsfera->SetMatteGlossBalance(atof(esfera[5].c_str()),
                                               Color(atof(esfera[6].c_str()),
                                                     atof(esfera[6].c_str()),
                                                     atof(esfera[6].c_str())),
                                               Color(atof(esfera[6].c_str()),
                                                     atof(esfera[6].c_str()),
                                                     atof(esfera[6].c_str())));
        } else {
            objetoEsfera = new Sphere(Vector(atof(esfera[0].c_str()),
                                             atof(esfera[1].c_str()),
                                             atof(esfera[2].c_str())),
                                      atof(esfera[3].c_str()));
            objetoEsfera->SetFullMatte(colores[1]);
            objetoEsfera->SetMatteGlossBalance(atof(esfera[4].c_str()),
                                            Color(atof(esfera[5].c_str()),
                                                  atof(esfera[5].c_str()),
                                                  atof(esfera[5].c_str())),
                                               Color(atof(esfera[5].c_str()),
                                                     atof(esfera[5].c_str()),
                                                     atof(esfera[5].c_str())));
        }
        cont++;
        esferas.push_back(objetoEsfera);
    }

    /**< Escena cubo-esfera-cilindro */
    Cuboid* cuboDerecho = new Cuboid(3.0, 3.0, 3.0);
    cuboDerecho->Move(42.0, -15.8, -285.0);
    cuboDerecho->SetFullMatte(colores[4]);
    cuboDerecho->RotateX(-90.0);
    cuboDerecho->RotateY(-22.0);

    Sphere* esferaDerecha = new Sphere(Vector(0.0, 0.0, 0.0), 2.5);
    esferaDerecha->SetFullMatte(colores[1]);
    esferaDerecha->Move(38.0, -15.5, -280.0);

    Cylinder* cilindroDerecho = new Cylinder(2.5, 7.0);
    cilindroDerecho->SetFullMatte(colores[3]);
    cilindroDerecho->Move(36.0, -14.5, -284.0);
    cilindroDerecho->RotateX(-90.0);

    /**< Escena cubo-cono-cono */

    Cuboid* cuboIzquierdo = new Cuboid(3.0, 3.0, 3.0);
    cuboIzquierdo->Move(-38.0, -15.8, -295.0);
    cuboIzquierdo->RotateX(-90.0);
    cuboIzquierdo->RotateY(-22.0);
    cuboIzquierdo->SetFullMatte(colores[4]);

    Cone* conoIzquierdo1 = new Cone(2.0, 8.0);
    conoIzquierdo1->SetFullMatte(colores[5]);
    conoIzquierdo1->Move(-42.0, -14.0, -290.0);
    conoIzquierdo1->RotateX(-90.0);

    Cone* conoIzquierdo2 = new Cone(2.0, 7.0);
    conoIzquierdo2->SetFullMatte(colores[5]);
    conoIzquierdo2->Move(-39.0, -10.0, -295.0);
    conoIzquierdo2->RotateX(-90.0);


    /**< Escena cono-cono-cono */

    Cone* cono1 = new Cone(3.0, 9.0);
    cono1->SetFullMatte(colores[5]);
    cono1->Move(20.0, -14.0, -250.0);
    cono1->RotateX(-90.0);

    Cone* cono2 = new Cone(3.0, 9.0);
    cono2->SetFullMatte(colores[5]);
    cono2->Move(15.0, -17.0, -240.0);
    cono2->RotateX(-90.0);
    cono2->RotateZ(105);
    cono2->RotateY(50);

    Cone* cono3 = new Cone(3.0, 9.0);
    cono3->SetFullMatte(colores[5]);
    cono3->Move(14.0, -14.0, -250.0);
    cono3->RotateX(-90.0);

    /**< Escena de los poligonos Concavos */

    Cylinder* cilindroConvexo = new Cylinder(2.5, 7.0);
    cilindroConvexo->SetFullMatte(colores[3]);
    cilindroConvexo->Move(0.0, 0.0, 0.0);//36.0, -19.5, -284.0
    cilindroConvexo->RotateX(-90.0);

    Cuboid* cuboConvexo = new Cuboid(3.0, 3.0, 3.0);
    cuboConvexo->Move(2.0, 0.0, 0.0);//
    cuboConvexo->SetFullMatte(colores[2]);
    cuboConvexo->RotateX(-90.0);

    SetIntersection* poligonoConvexo1 = new SetDifference(Vector(0.0, 0.0, 0.0), cilindroConvexo, cuboConvexo);
    poligonoConvexo1->Move(4.0, -14.25, -260.0);
    poligonoConvexo1->RotateY(-10.0);


    Cylinder* cilindroConvexo2 = new Cylinder(2.5, 7.0);
    cilindroConvexo2->SetFullMatte(colores[3]);
    cilindroConvexo2->Move(0.0, 0.0, 0.0);
    cilindroConvexo2->RotateX(-90.0);

    Cuboid* cuboConvexo2 = new Cuboid(3.0, 3.0, 3.0);
    cuboConvexo2->Move(2.0, 0.0, 0.0);//
    cuboConvexo2->SetFullMatte(colores[2]);
    cuboConvexo2->RotateX(-90.0);

    Sphere* esferaConvexa2 = new Sphere(Vector(0.0, 0.0, 0.0), 2.5);
    esferaConvexa2->SetFullMatte(colores[2]);
    esferaConvexa2->SetOpacity(0.20);
    esferaConvexa2->SetMatteGlossBalance(0.5, Color(1.0, 1.0, 1.0), Color(1.0, 1.0, 1.0));
    esferaConvexa2->Move(2.0, 0.5, 0.0);

    SetIntersection* poligonoConvexo2 = new SetDifference(Vector(0.0, 0.0, 0.0), cilindroConvexo2, esferaConvexa2);
    poligonoConvexo2->Move(-15.0, -27.25, -220.0);//
    poligonoConvexo2->RotateX(-90);
    poligonoConvexo2->RotateY(-50.0);


    /**< Se realiza la rotacion de los objetos */
    double rotacionEnX   = 3.0;
    double rotacionEnY   = 0.0;

    //Se rota la mesa
    for(int i = 0; i < 4; i++) {
        RotateY(patasDeMesa[i], rotacionEnY);
        RotateX(patasDeMesa[i], rotacionEnX);
    }
    RotateY(mesa, rotacionEnY);
    RotateX(mesa, rotacionEnX);

    //Se rota la mesa
    for(int i = 0; i < 4; i++) {
        RotateY(esferas[i], rotacionEnY);
        RotateX(esferas[i], rotacionEnX);
    }
    RotateY(esferaDerecha,rotacionEnY);
    RotateX(esferaDerecha, rotacionEnX);

    //Se rotan los poligonos
    RotateX(cuboDerecho, rotacionEnX);
    RotateY(cuboDerecho, rotacionEnY);

    RotateX(cuboIzquierdo, rotacionEnX);
    RotateY(cuboIzquierdo, rotacionEnY);

     //Se rotan los cilindros
    RotateX(cilindroDerecho, rotacionEnX);
    RotateY(cilindroDerecho, rotacionEnY);

     //Se rotan los conos
    RotateX(conoIzquierdo1, rotacionEnX);
    RotateY(conoIzquierdo1, rotacionEnY);

    RotateX(conoIzquierdo2, rotacionEnX);
    RotateY(conoIzquierdo2, rotacionEnY);

    RotateX(cono1, rotacionEnX);
    RotateY(cono1, rotacionEnY);

    RotateX(cono2, rotacionEnX);
    RotateY(cono2, rotacionEnY);

    RotateX(cono3, rotacionEnX);
    RotateY(cono3, rotacionEnY);

     //Se rotan los poligonos convexos
    RotateX(poligonoConvexo1, rotacionEnX);
    RotateY(poligonoConvexo1, rotacionEnY);

    RotateX(poligonoConvexo2, rotacionEnX);
    RotateY(poligonoConvexo2, rotacionEnY);


    /**< Se meten a la escena los objetos */

    //Se agrega la mesa
    for(int i = 0; i < 4; i++) {
        scene.AddSolidObject(patasDeMesa[i]);
    }
    scene.AddSolidObject(mesa);

    for(int i = 0; i < 4; i++) {
        scene.AddSolidObject(esferas[i]);
    }

    //Escena cubo-cilindro-esfera
    scene.AddSolidObject(esferaDerecha);
    scene.AddSolidObject(cilindroDerecho);
    scene.AddSolidObject(cuboDerecho);

    //Escena cubo-cono-cono
    scene.AddSolidObject(cuboIzquierdo);
    scene.AddSolidObject(conoIzquierdo1);
    scene.AddSolidObject(conoIzquierdo2);

    //Escena cono-cono-cono
    scene.AddSolidObject(cono1);
    scene.AddSolidObject(cono2);
    scene.AddSolidObject(cono3);

    //Escena poligonos convexos
    scene.AddSolidObject(poligonoConvexo1);
    scene.AddSolidObject(poligonoConvexo2);

    /**< Se ilumina la mesa de trabajo */

    scene.AddLightSource(LightSource(Vector(0.0, 300.0, -300.0), Color(1.0, 1.0, 1.0, 1.0)));

    //Agregamos 3 luces, izquierda, derecha y la del centro...
    scene.AddLightSource(LightSource(Vector(-100.0, 50.0, 100.0), Color(1.0, 1.0, 1.0, 1.0)));
    scene.AddLightSource(LightSource(Vector(0.0, -20.0, 200.0),   Color(1.0, 1.0, 1.0, 1.0)));//Vector(0.0, -20.0, 200.0)
    scene.AddLightSource(LightSource(Vector(100.0, 50.0, 100.0), Color(1.0, 1.0, 1.0, 1.0)));//100.0, 50.0, 100.0)

    // Generate a JPG file that displays the scene...
    const char *filename = "Mesa_de_Trabajo.jpg";
    scene.SaveImage(filename, 1008, 567, 4.5, 1);
    std::cout << "Se escribio la escena: " << filename << std::endl;
}
//---------------------------------------------------------------------------
// Define a new type that is a pointer to a function
// with void return type and taking zero arguments.
typedef void (* COMMAND_FUNCTION) ();

struct CommandEntry
{
    const char* verb;           // the command line option
    COMMAND_FUNCTION command;   // function to call when option encountered
    const char* help;           // usage text that explains the option
};

const CommandEntry CommandTable[] =
{
    { "escena", Escena,
        "   Mesa de trabajo.\n"
    },
};

// Calculate the number of entries in CommandTable[]...
const size_t NUM_COMMANDS = sizeof(CommandTable) / sizeof(CommandTable[0]);


void PrintUsageText()
{
    using namespace std;

    cout <<
        "Ray Tracer demo - Copyright (C) 2013 by Don Cross.\n"
        "For more info, see: http://cosinekitty.com/raytrace\n"
        "\n"
        "The following command line options are supported:\n"
        "Reescrito por Carlos Mario Villafuerte, Esteban Blanco.";

    for (size_t i=0; i < NUM_COMMANDS; ++i)
    {
        cout << "\n";
        cout << CommandTable[i].verb << "\n";
        cout << CommandTable[i].help;
    }

    cout << endl;
}


int main(int argc, const char *argv[])
{
    using namespace std;

    int rc = 1;

    if (argc == 1)
    {
        PrintUsageText();
    }
    else
    {
        const string verb = argv[1];
        bool found = false;
        for (size_t i=0; i < NUM_COMMANDS; ++i)
        {
            if (verb == CommandTable[i].verb)
            {
                found = true;                   // we found a match!
                CommandTable[i].command();      // call the function
                rc = 0;                         // indicate success
                break;                          // stop searching
            }
        }

        if (!found)
        {
            cerr << "ERROR:  Unknown command line option '" << verb << "'" << endl;
        }
    }
    return rc;
}
