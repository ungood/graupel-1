
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

class Component {
  public:
    virtual 
}

class State {
  public:
    virtual void enter() {}
    virtual void loop() {}
    virtual void exit() {}
};

class StateMachine {
  private:
    State& currentState;
  
  public:
    StateMachine(State& initialState) :
      currentState(initialState)
    {
    }

    void setup() {
        currentState.enter();
    }

    void loop() {
        currentState.loop();
    }
};

#endif