#include "yocto_api.h"
#include "yocto_altitude.h"
#include "yocto_temperature.h"
#include "yocto_pressure.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

static void usage(void)
{
    cout << "usage: demo <serial_number> " << endl;
    cout << "       demo <logical_name>" << endl;
    cout << "       demo any                 (use any discovered device)" << endl;
    u64 now = yGetTickCount();
	while (yGetTickCount()-now<3000){
        // wait 3 sec to show the message
    }
    exit(1);
}

int main(int argc, const char * argv[])
{
    string errmsg,target;
    YAltitude    *asensor;
    YTemperature *tsensor;
    YPressure    *psensor;     

    if(argc < 2) {
        usage();
    }
    target = (string) argv[1];
    
    // Setup the API to use local USB devices
    if(yRegisterHub("usb", errmsg) != YAPI_SUCCESS) {
        cerr << "RegisterHub error: " << errmsg << endl;
        return 1;
    }

    if(target == "any"){
        asensor = yFirstAltitude();
        tsensor = yFirstTemperature();
        psensor = yFirstPressure();
        if (asensor==NULL || tsensor==NULL || psensor==NULL) {
            cout << "No module connected (check USB cable)" << endl;
            return 1;
        }
    } else {
        asensor = yFindAltitude(target + ".altitude");
        tsensor = yFindTemperature(target + ".temperature");
        psensor = yFindPressure(target + ".pressure");        
    }

    
    
    while(1) {
        if(!asensor->isOnline()) {
            cout << "Module not connected (check identification and USB cable)";
            break;
        }
        
        cout << "Current altitude: "    << asensor->get_currentValue() << " m (QNH= "<< asensor->get_qnh() <<" hPa)" <<endl;
        cout << "Current temperature: " << tsensor->get_currentValue() << " °C"<<endl;
        cout << "Current pressure: "    << psensor->get_currentValue() << " hPa"<<endl;

        cout << "  (press Ctrl-C to exit)" << endl;
        ySleep(1000,errmsg);
    };
        
    return 0;
}