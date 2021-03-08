class Module {
  public:
    virtual void setup() = 0;
    virtual void loop(unsigned long millis) = 0;
};
