#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc7 : public cSimpleModule
{
  private:
    cMessage *event;
    cMessage *tictocMsg;
    int counter;            // Counter to limit messages
    simtime_t reset = 0;    // Used to calculate rtt
    cOutVector rttVector;   // Used to display rtt values on graph

  public:
    Txc7();
    virtual ~Txc7();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc7);

Txc7::Txc7()
{
    event = tictocMsg = nullptr;
}

Txc7::~Txc7()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc7::initialize()
{
    counter = 20;       // Set counter for multiple packets
    WATCH(counter);     // Monitor counter during simulation
    event = new cMessage("event");
    tictocMsg = nullptr;
    rttVector.setName("RTT_Count");
    if (strcmp("tic", getName()) == 0) {
        EV << "Scheduling first send to t=5.0s\n";
        //scheduleAt(5.0, event);
        tictocMsg = new cMessage("tictocMsg");
        send(tictocMsg, "out");
    }
}

void Txc7::handleMessage(cMessage *msg)
{
    counter --;
    if (strcmp("toc", getName()) == 0) {
    if (msg == event) {
        EV << "Wait period is over, sending back message\n";
        send(tictocMsg, "out");
        tictocMsg = nullptr;
    }
    else {
              simtime_t procdelay = par("delayTime");

            EV << "Message arrived, starting to wait " << procdelay << " secs...\n";
            tictocMsg = msg;
            scheduleAt(simTime()+procdelay, event);

    }
  }
    else{
               // Calculate Round Trip time
               simtime_t rtt = simTime() - reset;  //msg->getCreationTime();
               EV << "Round Trip Time = " << rtt << " seconds\n";
               rttVector.record(rtt);          // Store the rtt values to plot graph

               if (counter == 0)        // Check packet counter limit
               {
                   EV << getName() << "'s counter reached zero, deleting message\n";
                   delete msg;         // Delete message after counter is zero
               }
               else
               {
                   // Sending packet again for next rtt readings
                   EV << getName() << "'s counter is " << counter << ", sending back message\n";
                   send(tictocMsg, "out");
                   //tictocMsg = nullptr;
                   reset = simTime();      // Get transmission time to calculate rtt
               }

    }

}
