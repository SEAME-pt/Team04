#ifndef SPEED_H
#define SPEED_H

class Speed {
   private:
    double value;

   public:
    Speed(double initial = 0.0);

    void setSpeed(double speed);

    double getSpeed() const;

    ~Speed() = default;
};

#endif
