#ifndef STATES_H
#define STATES_H

class State {
  public:
    virtual void enter() {}
    virtual void loop() {}
};

class BootstrapState: public State {
  public:
    virtual void enter();
    virtual void loop();
};
extern BootstrapState bootstrapState;

class CalibratingState: public State {};
extern CalibratingState calibratingState;

class TransmittingState: public State {};
extern TransmittingState transmittingState;

class FailureState: public State {};
extern FailureState failureState;

#endif