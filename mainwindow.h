#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QFile>
#include "midiParser/MidiFile.h"
#include "midiParser/MidiEvent.h"
#include "factoriogenerator.h"


class FactorioGenerator;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MidiFile midifile;
    void setProgress(int progress);
    void processFile();
    qreal noteToFrequency(int note);

private slots:
    void on_openFileButton_clicked();
    void on_radioButton_Details_toggled(bool checked);

    void on_radioButton_TTS_toggled(bool checked);

    void on_pausespinBox_editingFinished();

    void on_radioButton_fact_toggled(bool checked);

    void on_checkBox_drumkit_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    int mode=0;
    //FactorioGenerator *fg;
};

#endif // MAINWINDOW_H
