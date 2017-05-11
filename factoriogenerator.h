#ifndef FACTORIOGENERATOR_H
#define FACTORIOGENERATOR_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Blueprinter/blueprint.h"
#include "midiParser/MidiFile.h"
#include "midiParser/MidiEvent.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class MainWindow;
class FactorioGenerator
{
public:

    bool globalPlayback=true; //Set Speakers to global mode

    FactorioGenerator(MainWindow *mw);
    MainWindow *mw;
    QString midiToBlueprint(MidiFile &midifile, bool drumkit);
    MidiFile *midifile;
private:

    QJsonObject makeDecider(float x, float y,int dir, int id, QString comp, int constant, bool copyFromInput, QString inSignalName, QString outSignalName);
    QJsonObject makeConnection(int cID, int eID);
    QJsonObject makeFilter(QString name, int count,int index);
    QJsonObject makeConstant(float x, float y, int dir, int id, bool isOn, QJsonArray filters);
    QJsonObject makeConnections(int cID, QVector<int> red_eIDs,QVector<int> green_eIDs);
    QJsonObject makeRedConnections(int cID, QVector<int> red_eIDs);
    QJsonObject makeGreenConnections(int cID, QVector<int> green_eIDs);
    QJsonObject makePole(float x,float y,int id);
    QJsonObject makePole(float x,float y,int id,QJsonObject connection);
    QJsonObject makeSpeaker(float x, float y, int id, int instId, QString signalName, float vol);
    int drumToFacId(int note);

    int generateMemCell(float x, float y, int adr, QString signal, int value); //returns decider id
    void generateClock(float x, float y, int val);


    int id=1;
    int lastPole=0;
    int lastConPoint=0;
    int lastX=0;
    int lastY=0;
    QVector<QPair<QString,int>> currentNotes;
    QVector<QString> signalNames={"signal-A","signal-B","signal-C","signal-D","signal-E","signal-F","signal-G","signal-H","signal-I","signal-J","signal-K","signal-L","signal-M","signal-N","signal-O","signal-P","signal-Q","signal-R","signal-S","signal-U","signal-V","signal-W","signal-X","signal-Y","signal-Z","signal-0","signal-1","signal-2","signal-3","signal-4","signal-5","signal-6","signal-7","signal-8","signal-9"};
    QMap<QString,QPair<int,float>> usedSignals; //instrument and volume

    QJsonObject genMemCell(int x, int y);
    QJsonObject blueprint;
    QJsonArray entities;
    QJsonDocument jsonDoc;

};

#endif // FACTORIOGENERATOR_H
