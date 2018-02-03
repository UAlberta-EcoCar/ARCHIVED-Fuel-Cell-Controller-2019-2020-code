#ifndef FUELCELL_H
#define FUELCELL_H

#include "IO.h"

#define START_STATE 1
#define SHUTDOWN_STATE 3
#define RUN_STATE 2
#define ALARM_STATE 4
#define OVERVOLTAGE 1
#define OVERCURR 2
#define OVERPRESS 3
#define OVERTEMP 4
#define UNDERPRESS 5
#define UNDERTEMP 6

class FuelCell: public IO{
    private:
    int fc_status;
    int error_status;
    int num_purges;

    public:
    FuelCell(string name="Fuel Cell"):IO(name){
        this->lock();
        this->fc_status = 0;
        this->error_status = 0;
        this->num_purges = 0;
        this->unlock();
    };

    static float query_optimal_temp(float curr){
        return (0.5295*curr + 26.032);
    }

    static float query_min_temp(float curr){
        return (0.6057*curr + 4.4979);
    }

    static float query_max_temp(float curr){
        return (0.287*curr + 53.218);
    }
    
    void set_fc_status(int status){
        this->lock();
        this->fc_status = status;
        this->unlock();
    };

    void set_error_status(int status){
        this->lock();
        this->error_status = status;
        this->unlock();
    };

    int get_fc_status(){
        return this->fc_status;
    };

    int get_error_status(){
        return this->error_status;
    };

    int get_num_purges(){
        return this->num_purges;
    };

    void increment_purge(){
        this->lock();
        this->num_purges++;
        this->unlock();
    };

    string toString(){
        stringstream ss;
        this->lock();
        ss << "FC_Status:";
        ss << this-> get_fc_status();
        ss << " Error_Status:";
        ss << this->get_error_status();
        ss << " ";
        this->unlock();
        return ss.str();
    };

    string toStringInfo(){
        stringstream ss;
        this->lock();
        ss << this-> get_fc_status();
        ss << " ";
        ss << this->get_error_status();
        ss << " ";
        this->unlock();
        return ss.str();
    };
};



#endif