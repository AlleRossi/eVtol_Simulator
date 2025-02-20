# eVtol_Simulator

1.The idea is to have a base class called vehicle that defines all of the common characteristics among the different EVTOLs and then 
to have a series of subclasses that statically define the different qualities of the aircrafts developed by the five companies.

2.Later created a EVTOL factory to dynamically create the instances of the vehicles that will be tested in the simulator, it's best to proceed in this way because of the unknown
number of vehicles of each kind that have to be deployed each times the code is run.

3.Next a Chargers class has to be implemented, each charger makes itself available to the first vehicle of a charging queue hence it needs attributes to determine availability and remaining charging time.
A factory class to create the Chargers isn't necessary, all chargers are exactly the same and can be generated with a simple for loop.

4.Lastly creating a simulator class with a loop that emulates the passing of time at an accelerated rate, here are implemented the functions to record the flight info, 
a handler function for the charging queues and the fault events.

Todo: write a simple python script to visualize the data retrieved from the simulator
