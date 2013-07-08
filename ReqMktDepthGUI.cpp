/*
 * File:   GUI.cpp
 * Author: Piotr Gregor  postmaster@cf16.eu
 *
 * Created on June 28, 2013, 6:57 PM
 */

#include "ReqMktDepthGUI.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <typeinfo>
#include <ql/patterns/observable.hpp>
#include <EClientSocketBaseImpl.h>
    
ReqMktDepthGUI::ReqMktDepthGUI(boost::shared_ptr<IB::PosixClient> client_ptr):client(client_ptr),thisGUIReqActive(false){
    widget.setupUi(this);
    QObject::connect(widget.requestButton, SIGNAL(clicked()), this, SLOT(requestClicked()));
    QObject::connect(widget.cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(widget.guiRequestButton, SIGNAL(clicked()), this, SLOT(guiRequestClicked()));
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this,SIGNAL(newMktDepth(int,rec_ptr)),this,SLOT(displayData(int,rec_ptr)));
}

ReqMktDepthGUI::~ReqMktDepthGUI() {
   #ifdef DEBUG 
        printf( "I am dead!\n");
   #endif
        if(thisGUIReqActive){
            cancelClicked();
        }
}

void ReqMktDepthGUI::myTickPriceUpdate(int tickerId, rec_ptr record_ptr){
    try{
        //const IB::TickPriceRecord* tickPriceRecord = dynamic_cast<const IB::TickPriceRecord*>(record.get());
        tickPriceRec_ptr tickPriceRecord_ptr(boost::dynamic_pointer_cast<IB::TickPriceRecord>(record_ptr));
        #ifdef DEBUG 
           printf( "myTickPriceUpdate! Id: %d, price: %f, tickType: %d\n",tickerId,tickPriceRecord_ptr->price_,tickPriceRecord_ptr->tickType_);
        #endif
//        QString qs=QString("myTickPriceUpdate! Id: %1, price: %2, tickType: %3").arg(tickerId).arg(tickPriceRecord_ptr->price_).arg(tickPriceRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
           printf( "myTickPriceUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}
void ReqMktDepthGUI::myTickSizeUpdate(int tickerId, rec_ptr record_ptr){
    try{
        tickSizeRec_ptr tickSizeRecord_ptr(boost::dynamic_pointer_cast<IB::TickSizeRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "myTickSizeUpdate! Id: %d, size: %d, tickType: %d\n",tickerId,tickSizeRecord_ptr->size_,tickSizeRecord_ptr->tickType_);
    #endif
//        QString qs=QString("myTickSizeUpdate! Id: %1, size: %2, tickType: %3").arg(tickerId).arg(tickSizeRecord_ptr->size_).arg(tickSizeRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
        //widget.textEdit_dataFeed->append("myTickSizeUpdate something...");
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "myTickSizeUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}
void ReqMktDepthGUI::myTickStringUpdate(int tickerId, rec_ptr record_ptr){
        try{
        tickStringRec_ptr tickStringRecord_ptr(boost::dynamic_pointer_cast<IB::TickStringRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "myTickStringUpdate! Id: %d, string: %s, tickType: %d\n",tickerId,tickStringRecord_ptr->string.c_str(),tickStringRecord_ptr->tickType_);
    #endif
//        QString qs=QString("myTickStringUpdate! Id: %1, string: ").arg(tickerId)+QString::fromStdString(tickStringRecord_ptr->string);
//        qs+=QString(" tickType: %1").arg(tickStringRecord_ptr->tickType_);
//        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "myTickStringUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

void ReqMktDepthGUI::myTickPriceGUIUpdate(int tickerId, rec_ptr record_ptr){
    try{
        //const IB::TickPriceRecord* tickPriceRecord = dynamic_cast<const IB::TickPriceRecord*>(record.get());
        tickPriceRec_ptr tickPriceRecord_ptr(boost::dynamic_pointer_cast<IB::TickPriceRecord>(record_ptr));
        #ifdef DEBUG 
           printf( "myTickPriceGUIUpdate! Id: %d, price: %f, tickType: %d\n",tickerId,tickPriceRecord_ptr->price_,tickPriceRecord_ptr->tickType_);
        #endif
        QString qs=QString("TickPriceGUIUp! Id:%1, price:%2, tickType:%3").arg(tickerId).arg(tickPriceRecord_ptr->price_).arg(tickPriceRecord_ptr->tickType_);
        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
           printf( "myTickPriceGUIUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

void ReqMktDepthGUI::myTickSizeGUIUpdate(int tickerId, rec_ptr record_ptr){
    try{
        tickSizeRec_ptr tickSizeRecord_ptr(boost::dynamic_pointer_cast<IB::TickSizeRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "myTickSizeGUIUpdate! Id: %d, size: %d, tickType: %d\n",tickerId,tickSizeRecord_ptr->size_,tickSizeRecord_ptr->tickType_);
    #endif
        QString qs=QString("TickSizeGUIUp! Id:%1, size:%2, tickType:%3").arg(tickerId).arg(tickSizeRecord_ptr->size_).arg(tickSizeRecord_ptr->tickType_);
        widget.textEdit_dataFeed->append(qs);
        //widget.textEdit_dataFeed->append("myTickSizeGUIUpdate something...");
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "myTickSizeGUIUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}

void ReqMktDepthGUI::myTickStringGUIUpdate(int tickerId, rec_ptr record_ptr){
        try{
        tickStringRec_ptr tickStringRecord_ptr(boost::dynamic_pointer_cast<IB::TickStringRecord>(record_ptr));
    #ifdef DEBUG 
        printf( "myTickStringGUIUpdate! Id: %d, string: %s, tickType: %d\n",tickerId,tickStringRecord_ptr->string.c_str(),tickStringRecord_ptr->tickType_);
    #endif
        QString qs=QString("TickStringGUIUp! Id:%1, string:").arg(tickerId)+QString::fromStdString(tickStringRecord_ptr->string);
        qs+=QString(" tickType:%1").arg(tickStringRecord_ptr->tickType_);
        widget.textEdit_dataFeed->append(qs);
    }catch(std::bad_cast& e){
        #ifdef DEBUG 
            printf( "myTickStringGUIUpdate: badCast for tickerId: %d\n",tickerId);
        #endif
    }
}
//public slots
void ReqMktDepthGUI::requestClicked(){
    
}

void ReqMktDepthGUI::cancelClicked() {
    if (thisGUIReqActive) {
        if(client->isConnected()){
            guiMarketDataFeedDelete();
            marketDataFeedDelete();
        }
//        if (totalGUIReqActive == 1) {
//            endProcessMessages();
//        }
        thisGUIReqActive = false;
        totalGUIReqActive--;
    }
} 

void ReqMktDepthGUI::marketDataFeedDelete(void){

}

void ReqMktDepthGUI::guiMarketDataFeedDelete(void){

}

void ReqMktDepthGUI::displayData(int tickerId, rec_ptr record_ptr){

}

void ReqMktDepthGUI::guiRequestClicked(){
    contract_ptr contract(new IB::Contract());
    contract->symbol = widget.lineEdit_Symbol->text().toStdString();
    contract->secType = widget.lineEdit_Type->text().toStdString();
    contract->strike = ::atof(widget.lineEdit_Strike->text().toStdString().c_str());
    contract->exchange = widget.lineEdit_Exchange->text().toStdString();
    contract->primaryExchange = widget.lineEdit_PrimaryExchange->text().toStdString();
    contract->currency = widget.lineEdit_Currency->text().toStdString();
    contract->includeExpired = widget.lineEdit_IncludeExpired->text().toInt();
    
    // register for tickPrice updates
    // map MarketData to event, tickerId and contractDescription
    boost::shared_ptr<GUIMarketData> tickPriceGUIMktData(new GUIMarketData(IB::TICK_PRICE,widget.lineEdit_Id->text().toInt(),contract));    
    // connect slot to signal
    QObject::connect(tickPriceGUIMktData.get(), SIGNAL(newRecord(int, rec_ptr)), this, SLOT(myTickPriceGUIUpdate(int, rec_ptr)), Qt::QueuedConnection);
    // put this connection into tickerIdGUIMarketDataMap, it will be stored in tickPriceGUIMarketDataFeed
    client->guiMarketDataFeedInsert(tickPriceGUIMktData);
    
   
    //TODO: client->reqMktData has to take all parameters of contract specified in GUI
    // now we process only few of them
    client->reqMktDepth((IB::TickerId)widget.lineEdit_Id->text().toInt(),contract, widget.lineEdit_MaxNumberOfRows->text().toInt());
    guiObservedContracts.insert(std::pair<int, contract_ptr >(widget.lineEdit_Id->text().toInt(), contract));
    
    thisGUIReqActive=true;
    totalGUIReqActive++;
//    if(totalGUIReqActive==1){
//        processMessages();
//    }
}
