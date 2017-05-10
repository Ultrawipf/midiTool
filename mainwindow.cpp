#include "mainwindow.h"
#include <QFileDialog>
#include <qmath.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->progressBar->setVisible(false);
    this->ui->checkBox_drumkit->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this, tr("Open Midi"), "", tr("Midi Files (*.mid *.midi)"));
    if(fileName.isEmpty())
        return;
    else {

       this->midifile = MidiFile();
       midifile.setFilename("");
       midifile.clear();

       midifile.read(fileName.toStdString());
        if (!midifile.status()) {
            QMessageBox::information(this, tr("Unable to open file"), "Error while opening file");
            this->ui->statusLabel->setText("Error");
            return;
        }
        this->ui->statusLabel->setText("Open: " + fileName);
    }
    midifile.linkNotePairs();
    midifile.doTimeAnalysis();
    midifile.sortTracks();
    midifile.joinTracks();
    this->processFile();
}

qreal MainWindow::noteToFrequency(int note){
    qreal base = 440.0;

    return (base / 32) * pow(2,((note - 9) / 12.0));

}
void MainWindow::setProgress(int progress){
    this->ui->progressBar->setValue(progress);
}

void MainWindow::processFile(){


    //File opened
    if(midifile.getNumEvents(0) < 1 || !midifile.status())
        return;
    int pauseTime = this->ui->pausespinBox->value();
    QString output="";
    int count=0;
    int track = 0;
    float pause=0;
    int waitTime=0;
    int curTrack=0;
    MidiEvent* lastEvent=nullptr;
    this->ui->progressBar->setVisible(true);
    this->ui->progressBar->setMaximum(midifile[track].size());

    if(mode==2){ //factorio mode
       bool drumkit=this->ui->checkBox_drumkit->isChecked();
       FactorioGenerator *fg = new FactorioGenerator(this);
       output = (fg->midiToBlueprint(midifile,drumkit));
       delete(fg);

    }else{
        for (int i=0; i<midifile[track].size(); i++) {
          if(midifile[track][i].isNoteOff()){

          }
          if (!midifile[track][i].isNoteOn()) {
             continue;
          }
          MidiEvent* event = &midifile[track][i];
          curTrack = event->track;

          int noteNum=event->getKeyNumber();
          float freq = this->noteToFrequency(noteNum);
          float duration= event->getDurationInSeconds() * 1000.0;
          if(duration < 1) //failsafe
              continue;
          if(lastEvent!=nullptr)
            pause=(event->seconds - lastEvent->getLinkedEvent()->seconds)*1000.0;


          if(mode==1){ //tts mode
              this->ui->pausespinBox->setValue(5);
              //[:t(tone height 5512=max),(tone length)]
              //[:t0,1000] pause
              //Sanctum tts
              if(lastEvent && lastEvent->seconds == event->seconds) //filter polyphony
                  continue;
              if(pause > pauseTime){
                  count++;
                  output+="[:t0,"+QString::number((int)pause)+"]";
              }
              output += "[:t"+QString::number((int)freq)+","+QString::number((int)duration)+"]";

              waitTime+=duration+(pause>pauseTime?pause:0);
              if(++count>9){
                  count=0;
                  output += '|'+QString::number(waitTime) + "\n";
                  waitTime=0;
              }

            }else if(mode==0){ //note info mode
              output +="Note: "+ QString::number(noteNum) +" on "+QString::number(curTrack)+ "\t f= " + QString::number(freq) + "hz\t at " + QString::number(midifile.getTimeInSeconds(track,i)) + " s\t Duration: " + QString::number(duration) + "ms \t"+QString::number(pause)+" ms to prev.\n";
           }



             lastEvent=event;
             this->setProgress(i);
        }
    }
    this->ui->progressBar->setVisible(false);
    this->ui->textBrowser->setText(output);
}


void MainWindow::on_radioButton_Details_toggled(bool checked)
{
    if(!checked)
        return;
    this->mode=0;
    this->ui->pausespinBox->setEnabled(false);
    processFile();
}

void MainWindow::on_radioButton_TTS_toggled(bool checked)
{
    if(!checked)
        return;
    this->mode=1;
    this->ui->pausespinBox->setEnabled(true);
    this->ui->pausespinBox->setValue(5);
    this->ui->checkBox_drumkit->setVisible(false);
    processFile();
}

void MainWindow::on_pausespinBox_editingFinished()
{
    this->ui->checkBox_drumkit->setVisible(false);
    processFile();
}

void MainWindow::on_radioButton_fact_toggled(bool checked)
{
    if(!checked)
        return;
    this->mode=2;
    this->ui->checkBox_drumkit->setVisible(true);
    this->ui->pausespinBox->setValue(2);
    this->ui->pausespinBox->setEnabled(false);
    processFile();
}

void MainWindow::on_checkBox_drumkit_toggled(bool checked)
{
    processFile();
}
