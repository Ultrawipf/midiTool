#include "factoriogenerator.h"
FactorioGenerator::FactorioGenerator(MainWindow *mw)
{
    this->mw = mw;

    //Make some icons for the blueprint
    QJsonArray icons;
    QJsonObject signal{{"name","constant-combinator"},{"type","item"}};
    QJsonObject icon{{"index",1},{"signal",signal}};
    icons.append(icon);

    QJsonObject signal2{{"name","programmable-speaker"},{"type","item"}};
    QJsonObject icon2{{"index",2},{"signal",signal2}};
    icons.append(icon2);

    blueprint["item"]="blueprint";
    blueprint["version"]=64424902656; //Found in decoded blueprints...
    blueprint["icons"]=icons;

}

QJsonObject FactorioGenerator::makeConnection(int cID, int eID){
    return QJsonObject {{"circuit_id",cID},{"entity_id",eID}};
}

QJsonObject FactorioGenerator::makeConnections(int cID, QVector<int> red_eIDs,QVector<int> green_eIDs){
    QJsonArray redConns;
    QJsonArray greenConns;
    for(int i=0;i<red_eIDs.length();i++)
        redConns.append(QJsonObject{{"entity_id",red_eIDs[i]},{"circuit_id",cID}});
    for(int i=0;i<green_eIDs.length();i++)
        greenConns.append(QJsonObject{{"entity_id",green_eIDs[i]},{"circuit_id",cID}});

    QJsonObject conns;
    conns["red"]=redConns;
    conns["green"]=greenConns;
    return conns;
}
QJsonObject FactorioGenerator::makeRedConnections(int cID, QVector<int> red_eIDs){
    QJsonArray redConns;
    for(int i=0;i<red_eIDs.length();i++)
        redConns.append(QJsonObject{{"entity_id",red_eIDs[i]},{"circuit_id",cID}});
    return QJsonObject{{"red",redConns}};
}
QJsonObject FactorioGenerator::makeGreenConnections(int cID, QVector<int> green_eIDs){

    QJsonArray greenConns;
    for(int i=0;i<green_eIDs.length();i++)
        greenConns.append(QJsonObject{{"entity_id",green_eIDs[i]},{"circuit_id",cID}});

    return QJsonObject{{"green",greenConns}};
}

QJsonObject makePos(float x,float y){
    return QJsonObject{{"x",x},{"y",y}};
}
QJsonObject FactorioGenerator::makeDecider(float x, float y,int dir, int id, QString comp, int constant, bool copyFromInput, QString inSignalName, QString outSignalName){

    QJsonObject firstSignal{
        {"name",inSignalName},
        {"type","virtual"}
    };
    QJsonObject outSignal{
        {"name",outSignalName},
        {"type","virtual"}
    };
    QJsonObject conditions{
        {"comparator", comp},
        {"constant", constant},
        {"copy_count_from_input", copyFromInput?"true":"false"},
        {"first_signal",firstSignal},
        {"output_signal",outSignal}
        };

    QJsonObject behavior{{"decider_conditions",conditions}};

    return QJsonObject{{"control_behavior",behavior},
        {"position",makePos(x,y)},
        {"direction",dir},
        {"name","decider-combinator"},
        {"entity_number",id}};
}
QJsonObject FactorioGenerator::makeFilter(QString name, int count,int index){

    QJsonObject signal{
        {"name",name},
        {"type","virtual"}
    };
    return QJsonObject{
        {"count",count},
        {"index",index},
        {"signal",signal}
    };
}

QJsonObject FactorioGenerator::makeConstant(float x, float y, int dir, int id, bool isOn, QJsonArray filters){


    QJsonObject filter{
        {"is_on",isOn?"true":"false"},
        {"filters",filters}
    };

    QJsonObject combinator{
        {"direction", dir},
        {"entity_number", id},
        {"name", "constant-combinator"},
        {"position",makePos(x,y)},
        {"control_behavior",filter}
    };

    return combinator;
}

QJsonObject FactorioGenerator::makePole(float x,float y,int id,QJsonObject connection){
    QJsonObject pole{
        {"name","medium-electric-pole"},
        {"position",makePos(x,y)},
        {"entity_number",id},
        {"connections",QJsonObject{{"1",connection}}}
    };
    return pole;
}
QJsonObject makeLamp(float x, float y,int id, QString signalName, QJsonObject connection){
    QJsonObject signal{
        {"name",signalName},
        {"type","virtual"}
    };
    QJsonObject circuitCondition{
        {"comparator",">"},
        {"constant",0}
    };
    QJsonObject condition{
        {"first_signal",signal},
        {"circuit_condition",circuitCondition}
    };
    QJsonObject control{
        {"circuit_condition", condition}
    };


    return QJsonObject{
        {"name","small-lamp"},
        {"position",makePos(x,y)},
        {"entity_number",id},
        {"connections",QJsonObject{{"1",connection}}},
        {"control_behavior",control}
    };
}


QJsonObject FactorioGenerator::makePole(float x,float y,int id){
    QJsonObject pole{
        {"name","medium-electric-pole"},
        {"position",makePos(x,y)},
        {"entity_number",id}
    };
    return pole;
}

QJsonObject FactorioGenerator::makeSpeaker(float x, float y, int id,int instId,QString signalName,float vol){
    QJsonObject circuitParameters{
        {"instrument_id",instId},
        {"note-id",0},//no note
        {"signal_value_is_pitch","true"}
    };
    QJsonObject parameters{
        {"allow_polyphony",true},
        {"playback_globally",this->globalPlayback},//set true to annoy everyone ;)
        {"playback_volume",vol}
    };
    QJsonObject signal{
        {"name",signalName},
        {"type","virtual"}
    };
    QJsonObject circuitCondition{
        {"comparator",">"},
        {"constant",0}
    };
    QJsonObject condition{
        {"first_signal",signal},
        {"circuit_condition",circuitCondition}
    };
    QJsonObject control{
        {"circuit_parameters",circuitParameters},
        {"circuit_condition", condition}
    };
    QJsonObject alert{
        {"alert_message",""},
        {"show_alert","false"},
        {"show_on_map","false"}
    };
    QJsonObject speaker{
        {"entity_number",id},
        {"name","programmable-speaker"},
        {"position",makePos(x,y)},
        {"control_behavior",control},
        {"parameters",parameters},
        {"alert_parameters",alert}

    };
    return speaker;
}
//Generate the clock/counter
void FactorioGenerator::generateClock(float x, float y,int val){

    int decId=this->id++;
    int conId=this->id++;
    this->lastConPoint=this->lastPole;
    this->lastPole=this->id++;
    QJsonObject decider = makeDecider(x+1.5,y,2,decId,"<",val,true,"signal-T","signal-T");
    QJsonObject constant = makeConstant(x,y,2,conId,false,QJsonArray{makeFilter("signal-T",1,1)});
    QJsonObject connections{{"1",makeRedConnections(1,QVector<int>{conId,decId})},{"2",makeGreenConnections(1,QVector<int>{decId})}};
    decider["connections"]=connections;
    constant["connections"]=makeRedConnections(1,QVector<int>{decId});
    QJsonObject pole = makePole(x+3,y,lastPole,makeRedConnections(2,QVector<int>{decId}));
    lastConPoint=lastPole;

    entities.append(constant);
    entities.append(decider);
    entities.append(pole);
}

int FactorioGenerator::generateMemCell(float x, float y,int adr,QVector<QPair<QString,int>> currentSignal){
    int decId=this->id++;
    int conId=this->id++;
    int decToCon=1;
    int count=1;
    QJsonArray filters;
    for(QPair<QString,int>& sig : currentSignal){
        filters.append(makeFilter(sig.first,sig.second,count++));
    }
    QJsonObject constant = makeConstant(x,y,2,conId,true,filters);
    constant["connections"]=QJsonObject{{"1",makeGreenConnections(decToCon,QVector<int>{decId})}};
    QJsonObject decider = makeDecider(x+1.5,y,2,decId,"=",adr,true,"signal-T","signal-everything");
    QJsonObject connections;
    connections["1"]=(makeRedConnections(1,QVector<int>{lastPole}));
    connections["2"]=(makeGreenConnections(1,QVector<int>{lastPole}));


    decider["connections"]=connections;
    entities.append(constant);
    entities.append(decider);
    return decId;
}

//Trying to map some Midi instruments to almost fitting factorio sounds
int FactorioGenerator::drumToFacId(int note){
    switch(note){
    case 41:
    case 43:
    case 35:
        return 2;
    case 36:
        return 1;
    case 37:
    case 76:
        return 11;
    case 38:
        return 3;
    case 39:
        return 14;
    case 40:
        return 4;
    case 42:
        return 7;
    case 48:
        return 5;
    case 46:
    case 44:
    case 51:
    case 59:
        return 6;
    case 49:
    case 57:
    case 55:
        return 12; //crash
    case 84:
        //case 59:
        return 13;
    case 96:
    case 70:
        return 15;
    case 77:
        return 11;
    case 80:
    case 81:
        return 17;
    case 88: //88 is normally not used
        return 13;//special reverse cymbal.
    default:
        return 11;
    }
}

//Generate memory cells for all signals in Vector
void FactorioGenerator::appendMemCells(QVector<QPair<QString,int>> currentSignals,int signalsPerRow,int tick){
    while(!currentSignals.empty()){
        if(++lastY >= signalsPerRow){
            lastY=0;
            lastX+=4;
            poleCount=0;

            //make top pole
            int poleId=this->id++;
            entities.append(makePole(lastX+3,lastY,poleId,makeConnections(1,QVector<int>{lastPole,lastConPoint},QVector<int>{lastPole,lastConPoint})));
            lastPole=poleId;
            lastConPoint=poleId;
        }

        if(++poleCount % 4 == 0){//make Pole
            int poleId=this->id++;
            entities.append(makePole(lastX+3,lastY,poleId,makeConnections(1,QVector<int>{lastPole},QVector<int>{lastPole})));
            lastPole=poleId;
        }

        QVector<QPair<QString,int>> ts;
        for(int i=0;i<18 && !currentSignals.empty();i++){
            ts.append(currentSignals.front());
            currentSignals.pop_front();
        }
        generateMemCell(this->lastX,this->lastY,tick,ts);
    }
}

QString FactorioGenerator::midiToBlueprint(MidiFile &midifile,bool drumkit){
    //Max 112, Lowest 41
    this->usedSignals.clear();
    int noteCount=0;
    int track = 0;

    int lastTick=0;
    for (int i=0; i<midifile[track].size(); i++){
        int tick=midifile[track][i].tick;
        if(midifile[track][i].isNoteOn() && midifile[track][i].tick != lastTick){
            noteCount++;
            lastTick=tick;
        }

    }
    int cellsPerRow = std::sqrt(noteCount)*2;


    int numTicks = midifile.getTotalTimeInSeconds() * 60;

    int drumChannel=-1;
    if(drumkit){
        drumChannel = 9; //GM1 Midi Standard, Channel 10 is Drumkit
    }

    QVector<QVector<QString>> channelSignals;

    QVector<QVector<QPair<QString,int>>> currentChanSignals;
    QVector<QVector<int>> channelData{{0,0,2},{0,0,1}};//first: last Tick, second: parallel notes this tick, third: instrument number. initialize with drumkit and piano


    generateClock(0.0,lastY,numTicks);
    int firstPoleId=lastConPoint;
    int tick=0;
    for (int i=0; i<midifile[track].size(); i++) { // Main Loop
        MidiEvent note = midifile[track][i];
        if(midifile[track][i].isNoteOff()){
            continue;
        }
        if (!midifile[track][i].isNoteOn()) {
            continue;
        }
        if((note.getKeyNumber()>112 || note.getKeyNumber()<41) && note.getChannel() != drumChannel) //out of range?
            continue;

        tick = 2 + midifile.getTimeInSeconds(track,i)*60; //60 ticks per second

        int chanId=0;
        int noteId=note.getKeyNumber()-40;
        if(note.getChannel()==drumChannel){//drumkit
            chanId=1;
            noteId=drumToFacId(note.getKeyNumber());
        }

        if(channelData[chanId][0]==tick){
            if(channelData[chanId][1] < (channelSignals[chanId].length()+this->signalNames.length())-3){
                ++channelData[chanId][1];//increment parallel

            }else
                continue;
        }else{
            if(channelData[chanId][1]>0)
                channelData[chanId][1]=0;//no more notes this tick
        }

        while(channelSignals.length()<=chanId){
            channelSignals.push_back(QVector<QString>(0));
            currentChanSignals.push_back(QVector<QPair<QString,int>>());
        }
        if(channelData[chanId][1]>=channelSignals[chanId].length()){//we need more speakers, allocate new signal
            channelSignals[chanId].push_back(signalNames.first());
            signalNames.pop_front(); //Remove used signal from available signal list
            //Insert signal with instrument and volume into signal map to generate speakers later
            usedSignals[channelSignals[chanId][channelData[chanId][1]]]=QPair<int,float>(channelData[chanId][2],chanId==1?0.5:1.0); //signalname,instrument id, volume (1.0 for piano, 0.5 for drums)
        }
        //Generate mem cells per channel and empty signal list
        if(tick!=channelData[chanId][0]){
            if(!currentChanSignals[chanId].empty()){
                appendMemCells(currentChanSignals[chanId],cellsPerRow,channelData[chanId][0]); //write memory cells
                currentChanSignals[chanId].clear();
            }
        }

        if(channelSignals[chanId].length()>channelData[chanId][1]){
            currentChanSignals[chanId].append(QPair<QString,int>(channelSignals[chanId][channelData[chanId][1]],noteId));
            lastTick=tick;
            channelData[chanId][0]=tick;
        }




        this->mw->setProgress(i);
    }
    for(int i=0;i<currentChanSignals.length();i++)
        if(!currentChanSignals[i].empty()){//last notes
            appendMemCells(currentChanSignals[i],cellsPerRow,lastTick);
            currentChanSignals[i].clear();
        }

    //Main generation finished
    //make Speakers
    QMapIterator<QString, QPair<int,float>> i(usedSignals);
    lastConPoint=firstPoleId;
    float speakerX=4.0;
    int lampId=this->id++;
    entities.append(makeLamp(speakerX-2,-2.0,lampId,"signal-T",makeGreenConnections(1,QVector<int>{lastConPoint})));
    while (i.hasNext()) {
        i.next();
        int speakerId=this->id++;
        lampId=this->id++;
        QJsonObject lamp = makeLamp(speakerX,-2.0,lampId,i.key(),makeGreenConnections(1,QVector<int>{lastConPoint}));
        QJsonObject speaker = makeSpeaker(speakerX++,-1.0,speakerId,i.value().first,i.key(),i.value().second);
        speaker["connections"]=QJsonObject{{"1",makeGreenConnections(1,QVector<int>{lastConPoint})}};
        lastConPoint=speakerId;
        entities.append(speaker);
        entities.append(lamp);
    }

    //Finish json and generate blueprint
    std::string out;
    blueprint["label"]=midifile.getFilename();
    blueprint["entities"]=entities;
    jsonDoc.setObject(QJsonObject{{"blueprint",blueprint}});
    if(BlueprintFromJSON(out,this->jsonDoc.toJson(QJsonDocument::Compact).toStdString()))
        return QString::fromStdString(out);
    else
        return "Error";

}
