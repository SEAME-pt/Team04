#ifndef RPM_H
#define RPM_H

class Rpm {
   private:
    int value;

   public:

    Rpm(int initial = 0);

    void setRPM(int rpm);

    int getRPM() const;

    ~Rpm() = default;
};

#endif
