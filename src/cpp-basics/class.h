class Operations {
  private:
    double a;
    double b;

  public:
    void set_values(double a, double b) {
        this->a = a;
        this->b = b;
    }

    double operate();
};
