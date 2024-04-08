#include "shopQueue.h"
#include "customer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

int main (int argc, char *argv[]) {

    //Check for the number of command line arguments
    if (argc != 7) {
        puts("Error: Wrong number of arguments!");
        puts("Usage: ./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>");
        return 1;
    }

    //Save the command line arguments in variables
    int seed = atoi(argv[1]);
    int total_min = atoi(argv[2]);
    float arrive_prob = stof(argv[3]);
    float order_prob = stof(argv[4]);
    int num_Queues = atoi(argv[5]);
    string outputFile = argv[6];

    //Create the empty queue
    vector <ShopQueue> queues(num_Queues);

    //Sets random seed
    srand(seed);

    //Creates id variable
    int id = 0;

    //Create a vector to keep track of # customers a cashier serves
    vector<int> customerServed (num_Queues);

    //Create a vector of Customers pointers to store the completed customer data
    vector<shared_ptr<Customer> > completed;

    //Simulation
    for(int time = 0; time < total_min; time++){
    
        //Store opens
        if (time == 0) {
            puts("Welcome to Gompei Coffee Shop!");
            cout << "-- # of Checkout Queues: " << num_Queues << " --\n";
            cout << "New customer at t = 0." << endl;
            queues[0].addNewCustomer(0,0,0,id);
            id++;
        }

        //Calculate a random value between 0 and 1
        float randomNum = ((float) rand() / (float) (RAND_MAX));

        //Add a customer to the shortest queue
        if (randomNum < arrive_prob) {
            int currentLow = 0;
            for (int num = 0; num < num_Queues; num++) {
                if (queues[num].getLength() < queues[currentLow].getLength()) {
                    currentLow = num;
                }
            }
            queues[currentLow].addNewCustomer(time, time, 0, id);
            cout << "New customer at t = " << time << "."<< endl;
            id++;
        }
    
        //Check to see if the front customer is done
        for(int numQueue = 0; numQueue < num_Queues; numQueue++){
            //Check that the queue has customers in it
            if(!queues[numQueue].isEmpty()){
                //Calculate a random value between 0 and 1
                float randomNum2 = ((float) rand() / (float)(RAND_MAX));
                if(randomNum2 < order_prob){
                    //Remove the customer from the queue
                    shared_ptr<Customer> c = queues[numQueue].removeFrontCustomer();
                    //Add to the completed vector
                    completed.push_back(c);
                    //Modify the completedTime variable
                    c->completedTime = time;
                    cout<< "Customer #" << c -> ID << " was served from t = "<< c -> startTime << " to "<< c -> completedTime<<"."<<endl;
                    //Start serving the next customer in line
                        if(!queues[numQueue].isEmpty()){
                            shared_ptr<Customer> nextCustomer = queues[numQueue].peekFrontCustomer();
                            nextCustomer -> startTime = time;
                        }
                    //Keep track of customer that was just served
                    customerServed[numQueue] = customerServed[numQueue] + 1;
                }
            }                 
        }
    }

    //Outputs remaining customers to terminal
    for(int num = 0; num < num_Queues; num++){
        cout<<"After "<< total_min <<" minutes, "<< queues[num].getLength() <<" customers remain in queue #"<< num << "\n";
    }

    //Outputs # customers that cashier served
    for(int num = 0; num < num_Queues; num++){
        cout << "Cashier " << num << " served " << customerServed[num] << " customers\n";
    }

    ofstream file;
    file.open(outputFile);
    //Print the data in the compeleted vector to a file
    if(file.is_open()){
        file << setw(8) << "ID" << setw(8) << "Arrive" << setw(8) << "Start" << setw(8) << "Leave" << endl;
        for(auto& customer : completed){
        file << setw(8) << customer -> ID << setw(8) << customer -> arrivalTime << setw(8) << customer -> startTime << setw(8) << customer -> completedTime << endl;
        }
    } 
    else {
        cout << "Error!" << "\n";
    }
    file.close();

    return 0;
}