#include <States.h>
#include <Arduino.h>

void Initialization::on_do()
{
    //on_entry();
}
void Initialization::on_entry()
{
    Serial.println("Init_entry");
    //stateVars.led.init(1000.0);
    //stateVars.led.set(0.001);
    stateVars.pre = false;
    stateVars.op = true;
    stateVars.flt = false;

    stateVars.encoder.init();
    stateVars.motorIN2.init();
    stateVars.motorIN1.init(10);  // ms

    this->context_->command_go();
    
}
void Initialization::on_exit()
{

}
void Initialization::on_command_go()
{
    Serial.println("Automatically go to PreOperational state");
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
    stateVars.motorIN1.set(0.00001);
    if (stateVars.c == 'o' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_stop();
    }
}
void PreOperational::on_entry()
{
    Serial.println("PreOperational entry");
    Serial.println("Blinking LED at 1 Hz");
    //stateVars.led.set(0.5);
    stateVars.pre = true;
}
void PreOperational::on_exit()
{

}
void PreOperational::on_command_go()
{
    if (stateVars.c == 'o'){
        Serial.println("Going to OP");
        this->context_->transition_to(new Operational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
    if (stateVars.flt == true) {
        stateVars.pre = true;
        Serial.println("PRE:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }
}

void PreOperational::on_command_stop()
{
}

void PreOperational::on_timeout(){
}

void Operational::on_do() 
{
        if (stateVars.faultPin.is_lo()){
            if (!stateVars.flt) {
                stateVars.faultStartTime = millis();
                stateVars.flt = true;
            }
        } else {
            stateVars.flt = false;
        }

        if (stateVars.flt && (millis() - stateVars.faultStartTime >= 100)) {
            Serial.println("fault");
            stateVars.flt = false;
        }


    if (stateVars.c == 'p' || stateVars.c == 'r'){
        this->context_->command_go();
    }
    if (stateVars.flt){
        this->context_->command_stop();
    }
    stateVars.ref = (analogRead(stateVars.analogPin)/1023.0)*120;
    stateVars.actual = abs(stateVars.encoder.speed());
    stateVars.u = stateVars.controller.update(stateVars.ref, stateVars.actual);
    stateVars.u = constrain(stateVars.u, 0.0, 0.999); // Ensure pwmValue is within [0, 1]
    stateVars.pwmValue = stateVars.u;
    stateVars.motorIN1.set(stateVars.pwmValue);
    if (millis() - stateVars.lastPrintTime >= 250) {  
        Serial.print("speed: (");
        Serial.print("Ref: ");
        Serial.print(stateVars.ref);
        Serial.print(" - Act: ");
        Serial.print(stateVars.actual);
        Serial.print(") [RPM], ");
        Serial.print(" duty cycle: ");
        Serial.print(stateVars.pwmValue);
        stateVars.lastPrintTime = millis();  
    }
    stateVars.encoder.update();
}
void Operational::on_entry() 
{
    Serial.println("Operational entry");
    Serial.println("Turning on LED");
    //stateVars.led.set(0.9999);
    stateVars.pre = false;
    stateVars.timer.init(0.1); // ms
    stateVars.encoder.init();
    stateVars.motorIN2.init();
    stateVars.motorIN1.init(10);  // ms
    stateVars.faultPin.init();
    stateVars.motorIN1.set(0.0001);  // duty cycle
    stateVars.motorIN2.set_lo();
}
void Operational::on_exit() 
{

}
void Operational::on_command_go()
{
    if (stateVars.c == 'p'){
        Serial.println("Going to PRE");
        this->context_->transition_to(new PreOperational);
    } 
    if (stateVars.c == 'r'){
        Serial.println("Going to IN");
        this->context_->transition_to(new Initialization);
    }
    if (stateVars.flt == true) {
        stateVars.pre = false;
        Serial.println("OP:Stop command detected - Stopping");
        this->context_->transition_to(new Stopped);
    }
}

void Operational::on_command_stop()
{
}

void Operational::on_timeout()
{
    stateVars.flt = true;
}

void Stopped::on_do() 
{
    if (stateVars.c == 'c'){
        this->context_->command_go();
    }
}
void Stopped::on_entry() 
{
    stateVars.motorIN1.set(0.00001);
    Serial.println("Stopped! waiting for 'c'");
    Serial.println("Blinking LED at 2 Hz");
    //stateVars.led.set(0.2);
}
void Stopped::on_exit() 
{
    Serial.println("Setting flt = false");
    stateVars.flt = false;
}
void Stopped::on_command_go() 
{
    Serial.println("STO:Continue command detected - Continue");
    if (stateVars.pre){
        this->context_->transition_to(new PreOperational);
    } else{
        this->context_->transition_to(new Operational);
    }
}

void Stopped::on_command_stop() 
{
}

void Stopped::on_timeout()
{

}