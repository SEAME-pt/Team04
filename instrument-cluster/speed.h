#ifndef SPEED_H
#define SPEED_H
class Speed {
private:
    double current;
public:
    Speed(double initial);
    void increase();
    void decrease();
    double getCurrent();
};
#endif  // SPEED_H
