#ifndef RPM_H
#define RPM_H

class RPM {
   private:
    int value;

   public:

    RPM(int initial = 0);

    void setRPM(int rpm);

    int getRPM() const;

    ~RPM();
};

#endif
