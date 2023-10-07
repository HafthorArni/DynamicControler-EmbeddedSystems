#include <States.h>

void Initialization::on_do()
{
    on_entry();
}
void Initialization::on_entry()
{
    Serial.println("Turning off LED");
    stateVars.led.init(10000.0);
    stateVars.led.pin.set_lo();
    Serial.println("Initializing c");
    Serial.println("Setting pre = false");
    stateVars.pre = false;
    Serial.println("Setting op = true");
    stateVars.op = true;
    Serial.println("Setting FLT = false");
    stateVars.flt = false;
    this->context_->command_go();
    
}
void Initialization::on_exit()
{

}
void Initialization::on_command_go()
{
    Serial.println("IN:op = true - We are operational");
    this->context_->transition_to(new PreOperational);
}

void Initialization::on_command_stop()
{

}

void Initialization::on_timeout()
{

}
// void on_event1();
// void on_event2();

void PreOperational::on_do()
{
    Serial.println("Doing fault check");

    Serial.println("Setting c = '0'");
    stateVars.c = '0';
    Serial.println("Reading into c from keyboard");
    stateVars.c = Serial.read();
    if (stateVars.c == 'o' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_stop();
    }
}
void PreOperational::on_entry()
{
    Serial.println("Blinking LED at 1 Hz");
    stateVars.led.set(0.1);
    Serial.println("Setting pre = true");
    stateVars.pre = true;
}
void PreOperational::on_exit()
{

}
void PreOperational::on_command_go()
{
    Serial.println("PRE:Leaving pre");
    if (stateVars.c == 'o'){
        Serial.println("Going to OP");
        this->context_->transition_to(new Operational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
}

void PreOperational::on_command_stop()
{
    Serial.println("PRE:Stop command detected - Stopping");
    this->context_->transition_to(new Stopped);
}

void PreOperational::on_timeout(){
    stateVars.flt = true;
}

void Operational::on_do() 
{
    Serial.println("Doing fault check");

    Serial.println("Setting c = '0'");
    stateVars.c = '0';
    Serial.println("Reading into c from keyboard");
    stateVars.c = Serial.read();
    if (stateVars.c == 'p' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_stop();
    }
}
void Operational::on_entry() 
{
    Serial.println("Turning on LED");
    stateVars.led.pin.set_hi();
    Serial.println("Setting pre = false");
    stateVars.pre = false;
}
void Operational::on_exit() 
{

}
void Operational::on_command_go()
{
    Serial.println("OP:Leaving OP");
    if (stateVars.c == 'p'){
        Serial.println("Going to PRE");
        this->context_->transition_to(new PreOperational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
}

void Operational::on_command_stop()
{
    Serial.println("OP:Stop command detected - Stopping");
    this->context_->transition_to(new Stopped);
}

void Operational::on_timeout()
{
    stateVars.flt = true;
}

void Stopped::on_do() 
{
    Serial.println("Setting c = '0'");
    stateVars.c = '0';
    Serial.println("Reading into c from keyboard");
    stateVars.c = Serial.read();
    if (stateVars.c == 'c'){
        this->context_->command_go();
    }
}
void Stopped::on_entry() 
{
    Serial.println("Blinking LED at 2 Hz");
    stateVars.led.set(0.2);
}
void Stopped::on_exit() 
{
    Serial.println("Setting flt = false");
    stateVars.flt = false;
}
void Stopped::on_command_go() 
{
}

void Stopped::on_command_stop() 
{
    Serial.println("STO:Continue command detected - Continue");
    if (stateVars.pre){
        this->context_->transition_to(new PreOperational);
    } else{
        this->context_->transition_to(new Operational);
    }
}

void Stopped::on_timeout()
{

}